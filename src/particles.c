/*
 *
 *	FBL ENGINE
 *
 *	particles.c
 *
 *	Contains particles related functions including setting
 *	up emitters and stuff. The function for updating
 *	particle positions can be run on a separate thread
 *	on all platforms except HTML5 (maybe in the future).
 * 
 *
 *	Hans Strömquist 2017 - 2020
 *
 */


#include <math.h>

#include "engine.h"


/* only used by particles.c */

int emit_particle(int tag, void* emit, void* dummy);
//void set_thread_loop(bool ok);
//static int test_func(void *ptr);
int  update_particle_logic(int tag, void* emit, void* dummy);
int  render_particles(int tag, void* emitter, void* dummy);
int  free_particle_mem(int tag, void* emitter, void* dummy);
DLLIST* get_emitter_item_at_id(int id);
float _lerp(float a, float b, float amount);
void attract_particles(PARTICLE *p, int num_p);
double angle_to_target(double x1, double y1, double x2, double y2);


/*
SDL_Thread *thread;
SDL_mutex  *mutex;

bool thread_running = false;
bool do_thread_loop = false;
*/

extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

DLLIST* fbl_emitter_list = NULL;

unsigned int current_emitter = 0;


float gravity = 1;


/*
 * Create a particle emitter by adding an area where
 * particles spawn from.
 */
int fbl_create_emitter(int w, int h, int num_particles, int life, int rate, int density, float scale_start, float scale_end)
{

	FBL_PARTICLE_EMITTER* fbl_emitter = NULL;
	int i;

	/*
	if (!fbl_particle_texture)
	{
		fprintf(FBL_ERROR_OUT, "Load particle sheet before creating emitter!\n");
		return -1;
	}
	*/

	fbl_emitter = malloc(sizeof(FBL_PARTICLE_EMITTER));

	if (!fbl_emitter)
	{
		fprintf(FBL_ERROR_OUT, "No memory for particle emitter!\n");
		return -1;
	}

	/*

	thread_running = true;
	do_thread_loop = true;


	thread = SDL_CreateThread(update_particle_logic, "update_particle_logic", (void *)NULL);

	//thread = SDL_CreateThread(test_func, "test_func", (void *)NULL);

	if (NULL == thread) {
		fprintf(FBL_ERROR_OUT, "SDL_CreateThread failed: %s\n", SDL_GetError());
	}

	mutex = SDL_CreateMutex();

	if (!mutex) {
		fprintf(FBL_ERROR_OUT, "Couldn't create mutex!\n");
		return -1;
	}
	*/

	/* limit to some reasonable values */

	if (num_particles < 1) num_particles = 1;
	if (num_particles > 10000) num_particles = 10000;

	if (life < 1) life = 1;
	if (life > 500) life = 500;

	if (rate < 1) rate = 1;		// avoid division by 0, 1 = one emit every frame
	if (rate > 300) rate = 300;	// one emit every 5 seconds (60 fps) is da limit

	if (density < 1) density = 1;	// min 1 particle / emit
	if (density > 10) density = 10;	// max 10 particles / emit

	if (scale_start < 0.1) scale_start = 0.1f;
	if (scale_end > 20.0) scale_start = 20.0f;


	fbl_emitter->particle = malloc(num_particles * sizeof(PARTICLE));

	if (!fbl_emitter->particle)
	{
		fprintf(FBL_ERROR_OUT, "No memory for particles!\n");
		return -1;
	}


	fbl_emitter->type = 1;

	fbl_emitter->spawn_area.x = 0;
	fbl_emitter->spawn_area.y = 0;
	fbl_emitter->spawn_area.w = w;
	fbl_emitter->spawn_area.h = h;

	fbl_emitter->scale_start = scale_start;
	fbl_emitter->scale_end = scale_end;

	fbl_emitter->vel_x_start = 2.0f;
	fbl_emitter->vel_y_start = 2.0f;
	fbl_emitter->vel_x_end = 2.0f;
	fbl_emitter->vel_y_end = 2.0f;

	fbl_emitter->color_start.r = 0;
	fbl_emitter->color_start.g = 10;
	fbl_emitter->color_start.b = 255;
	fbl_emitter->color_start.a = 255;

	fbl_emitter->color_end.r = 255;
	fbl_emitter->color_end.g = 100;
	fbl_emitter->color_end.b = 60;
	fbl_emitter->color_end.a = 0;

	fbl_emitter->num_particles = num_particles;
	fbl_emitter->life_total = life;
	fbl_emitter->emit_rate = rate;	// example: 30 would be twice every second at 60 fps NOTE: don't let there be division by 0!
	fbl_emitter->density = density;
	fbl_emitter->rand_amount = 1.0;
	fbl_emitter->fix_to_screen = false;
	fbl_emitter->active = true;


	for (i = 0; i < num_particles; i++)
	{

		fbl_emitter->particle[i].life = 0;	// start at 0 life (not active)

	}


	/* if it's the first emitter created start a new list, otherwise append */

	if (current_emitter == 0)
	{

		fbl_emitter_list = DLCreate(current_emitter, fbl_emitter, sizeof(FBL_SPRITE));


	}
	else
	{

		DLAppend(&fbl_emitter_list, current_emitter, fbl_emitter, sizeof(FBL_SPRITE));

	}


	free(fbl_emitter);

	current_emitter++;

	return current_emitter - 1;

}

void fbl_delete_emitter(int id)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		/* security so that the first element can't be deleted, will crash! */

		if (item != DLGetFirst(fbl_emitter_list))	//  before: if(id > 0)
		{

			/* free particles first */

			free(emitter->particle);

			DLDelete(item);
		}
		else
			fbl_set_emitter_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
			//fprintf(FBL_ERROR_OUT, "Will not delete the first sprite in list! (id: %d) Use destroy_all..\n", id);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete particle emitter %d, that does not exist!\n", id);
#endif


}

void fbl_set_emitter_active(int id, bool active)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);
		emitter->active = active;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set active for particle emitter %d, that does not exist!\n", id);
#endif

}

void fbl_set_emitter_xy(int id, int x, int y)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		emitter->spawn_area.x = x;
		emitter->spawn_area.y = y;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set xy for particle emitter %d, that does not exist!\n", id);
#endif

}

void fbl_set_emitter_vel_xy(int id, float vel_x, float vel_y, bool start)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		if (start)
		{
			emitter->vel_x_start = vel_x;
			emitter->vel_y_start = vel_y;
		}
		else
		{
			emitter->vel_x_end = vel_x;
			emitter->vel_y_end = vel_y;
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set velocity xy for particle emitter %d, that does not exist!\n", id);
#endif

}

void fbl_set_emitter_color(int id, unsigned char r, unsigned char g, unsigned char b, unsigned char a, bool start)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		if (start)
		{
			emitter->color_start.r = r;
			emitter->color_start.g = g;
			emitter->color_start.b = b;
			emitter->color_start.a = a;
		}
		else
		{
			emitter->color_end.r = r;
			emitter->color_end.g = g;
			emitter->color_end.b = b;
			emitter->color_end.a = a;
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set start color for particle emitter %d, that does not exist!\n", id);
#endif

}

void fbl_fix_emitter_to_screen(int id, bool fix)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);
		emitter->fix_to_screen = fix;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set fix to screen for particle emitter %d, that does not exist!\n", id);
#endif

}

int  fbl_get_num_active_particles(int id)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);
	int active_particle_count = 0;
	int i;

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		if (emitter->active)
		{

			for (i = 0; i < emitter->num_particles; i++)
			{

				if (emitter->particle[i].life > 0) active_particle_count++;

			}

		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get number of active particles for emitter %d, that does not exist!\n", id);
#endif


	return active_particle_count;

}

int  fbl_get_num_emitters()
{

	return DLCount(fbl_emitter_list);

}

void fbl_destroy_all_emitters()
{

	DLWalk(fbl_emitter_list, free_particle_mem, NULL);
	DLDestroy(&fbl_emitter_list);

	current_emitter = 0;

}

int emit_particle(int tag, void* emit, void* dummy)
{

	FBL_PARTICLE_EMITTER *emitter;
	int i;
	int emits;

	emitter = (FBL_PARTICLE_EMITTER *)emit;

	if (emitter->active)
	{

		if (fbl_engine.current_frame % emitter->emit_rate == 0)
		{

			for (emits = 0; emits < emitter->density; emits++)
			{

				for (i = 0; i < emitter->num_particles; i++)
				{

					// find a free particle

					if (emitter->particle[i].life < 1)
					{

						if (emitter->fix_to_screen) {

							emitter->particle[i].x = (float)(rand() % emitter->spawn_area.w) + (float)emitter->spawn_area.x;
							emitter->particle[i].y = (float)(rand() % emitter->spawn_area.h) + (float)emitter->spawn_area.y;

						}
						else
						{

							emitter->particle[i].x = (float)(rand() % emitter->spawn_area.w) + (float)emitter->spawn_area.x - fbl_camera.x;
							emitter->particle[i].y = (float)(rand() % emitter->spawn_area.h) + (float)emitter->spawn_area.y - fbl_camera.y;

						}

						emitter->particle[i].vel_x = ((float)(rand() % ((int)emitter->vel_x_start * 10)) / 10) - (emitter->vel_x_start / 2);
						emitter->particle[i].vel_y = ((float)(rand() % ((int)emitter->vel_y_start * 10)) / 10) - (emitter->vel_y_start / 2);

						emitter->particle[i].scale = emitter->scale_start;
						emitter->particle[i].color = emitter->color_start;
						emitter->particle[i].life = emitter->life_total;	// give it life (activate particle)

						i = emitter->num_particles;	// break out of the loop

					}

				}

			}

		}

	}

	return 0;

}
/*
void set_thread_loop(bool ok)
{

	SDL_LockMutex(mutex);
	do_thread_loop = ok;
	SDL_UnlockMutex(mutex);

}

static int test_func(void *ptr)
{

	int count;

	for (count = 0; count < 20; ++cnt) {
		printf("Thread counter: %d\n", count);
		SDL_Delay(1000);
	}

	return count;

}
*/

int update_particle_logic(int tag, void* emit, void* dummy)
{

	FBL_PARTICLE_EMITTER* emitter;
	int i;

	emitter = (FBL_PARTICLE_EMITTER*)emit;

	//while(thread_running)
	//{

		//if(do_thread_loop)
		//{

			if (emitter->active)
			{

				switch (emitter->type)
				{

				case 0:

					attract_particles(emitter->particle, emitter->num_particles);

					for (i = 0; i < emitter->num_particles; i++)
					{

						emitter->particle[i].x += emitter->particle[i].vel_x;
						emitter->particle[i].y += emitter->particle[i].vel_y;

						if (emitter->particle[i].x <= 0 || emitter->particle[i].x >= fbl_get_screen_w())
						{
							emitter->particle[i].vel_x *= -1;
						}
						if ((emitter->particle[i].y <= 0) || (emitter->particle[i].y >= fbl_get_screen_h()))
						{
							emitter->particle[i].vel_y *= -1;
						}


					}

				break;

				case 1:


					for (i = 0; i < emitter->num_particles; i++)
					{

						if (emitter->particle[i].life > 0)
						{

							emitter->particle[i].x += emitter->particle[i].vel_x;
							emitter->particle[i].y += emitter->particle[i].vel_y;

							emitter->particle[i].color.r = (Uint8)_lerp((float)emitter->color_start.r, (float)emitter->color_end.r, (float)emitter->particle[i].life / (float)emitter->life_total);
							emitter->particle[i].color.g = (Uint8)_lerp((float)emitter->color_start.g, (float)emitter->color_end.g, (float)emitter->particle[i].life / (float)emitter->life_total);
							emitter->particle[i].color.b = (Uint8)_lerp((float)emitter->color_start.b, (float)emitter->color_end.b, (float)emitter->particle[i].life / (float)emitter->life_total);
							emitter->particle[i].color.a = (Uint8)_lerp((float)emitter->color_start.a, (float)emitter->color_end.a, (float)emitter->particle[i].life / (float)emitter->life_total);

							emitter->particle[i].scale = _lerp(emitter->scale_start, emitter->scale_end, (float)emitter->particle[i].life / (float)emitter->life_total);

							//emitter->particle[i].vel_x = _lerp(emitter->vel_x_start, emitter->vel_x_end, (float)emitter->particle[i].life / (float)emitter->life_total);
							//if (emitter->particle[i].life < emitter->life_total / 2)
								//emitter->particle[i].vel_y = _lerp(emitter->vel_y_start, emitter->vel_y_end, (float)emitter->particle[i].life / (float)emitter->life_total);

							emitter->particle[i].life--;

						}

					}

				break;

				}

			}

			//set_thread_loop(false);

		//}

		//SDL_Delay(10);

	//}

    
    return 0;

}

void engine_update_all_particles()
{

	DLWalk(fbl_emitter_list, update_particle_logic, NULL);
	DLWalk(fbl_emitter_list, emit_particle, NULL);

	SDL_Delay(10);

}

void engine_render_all_particles(void)
{

	DLWalk(fbl_emitter_list, render_particles, NULL);

}

/* NOTE: add more options to the drawing (particle sprites etc) */

int  render_particles(int tag, void* emit, void* dummy)
{

	FBL_PARTICLE_EMITTER* emitter;
	SDL_Rect tmp_rect;
	int i;

	emitter = (FBL_PARTICLE_EMITTER*)emit;

	if (emitter->active)
	{

		SDL_SetRenderDrawBlendMode(fbl_engine.renderer, SDL_BLENDMODE_ADD);

		for (i = 0; i < emitter->num_particles; i++)
		{

			if (emitter->particle[i].life > 0)
			{

				tmp_rect.x = (int)emitter->particle[i].x;
				tmp_rect.y = (int)emitter->particle[i].y;

				tmp_rect.w = (int)(5 * emitter->particle[i].scale);
				tmp_rect.h = (int)(5 * emitter->particle[i].scale);

				/* culling */

				if (tmp_rect.x > -tmp_rect.w && tmp_rect.x < (int)fbl_engine.w &&
					tmp_rect.y > -tmp_rect.h && tmp_rect.y < (int)fbl_engine.h)

				{

					SDL_SetRenderDrawColor(fbl_engine.renderer, emitter->particle[i].color.r, emitter->particle[i].color.g, emitter->particle[i].color.b, emitter->particle[i].color.a);
					SDL_RenderFillRect(fbl_engine.renderer, &tmp_rect);
				
				}


			}

		}

	}

	return 0;

}

/* free the particle data */

int free_particle_mem(int tag, void* emitter, void* dummy)
{

	free(((FBL_PARTICLE_EMITTER*)emitter)->particle);

	return 0;

}

DLLIST* get_emitter_item_at_id(int id)
{

	DLLIST* item;

	item = DLGetFirst(fbl_emitter_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;

		item = item->Next;

	}

	return NULL;

}


float _lerp(float a, float b, float amount)
{

	return (a * amount) + (b * (1 - amount));

}

void attract_particles(PARTICLE *p, int num_p)
{

	int i, j;
	float dvx;
	float dvy;

	for (i = 0; i < num_p; i++)
	{

		dvx = 0;
		dvy = 0;

		for (j = 0; j < num_p; j++)
		{
			if (i != j)
			{
				float angle = (float)angle_to_target(p[i].x, p[i].y, p[j].x, p[j].y);
				dvy += (float)(gravity * sin(angle)) / 1000;
				dvx += (float)(gravity * cos(angle)) / 1000;
			}

		}
		p[i].vel_x += dvx;
		p[i].vel_y += dvy;
	}


}

double angle_to_target(double x1, double y1, double x2, double y2)
{

	double delta_x = (x2 - x1);
	double delta_y = (y2 - y1);

	return atan2(delta_y, delta_x);

}


/* end of particles.c */
