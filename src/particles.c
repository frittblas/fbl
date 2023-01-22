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
 *	Hans Strömquist 2017 - 2022
 *
 */


#include <math.h>

#include "engine.h"


/* only used by particles.c */

int emit_particle(int tag, void* emit, void* dummy);
int  update_particle_logic(int tag, void* emit, void* dummy);
int  render_particles(int tag, void* emitter, void* dummy);
int  free_particle_mem(int tag, void* emitter, void* dummy);
DLLIST* get_emitter_item_at_id(int id);
float _lerp(float a, float b, float amount);

extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

extern SDL_Texture* fbl_texture;

DLLIST* fbl_emitter_list = NULL;

unsigned int current_emitter = 0;

float gravity = 1;


/*
 * Create a particle emitter by adding an area where
 * particles spawn from.
 */
int fbl_create_emitter(int num_particles)
{

	FBL_PARTICLE_EMITTER* fbl_emitter = NULL;
	int i;

	fbl_emitter = malloc(sizeof(FBL_PARTICLE_EMITTER));

	if (!fbl_emitter)
	{
		fprintf(FBL_ERROR_OUT, "No memory for particle emitter!\n");
		return -1;
	}


	/* limit to some reasonable values */

	if (num_particles < 1) num_particles = 1;
	if (num_particles > 5000) num_particles = 5000;

	fbl_emitter->particle = malloc(num_particles * sizeof(PARTICLE));

	if (!fbl_emitter->particle)
	{
		fprintf(FBL_ERROR_OUT, "No memory for particles!\n");
		return -1;
	}


	fbl_emitter->type = FBL_EMITTER_FLOWER;	// default to flower
	fbl_emitter->shape = FBL_NORMAL_RECT;	// default to filled rect (FBL_RECT = unfilled)

	fbl_emitter->spawn_area.x = 0;
	fbl_emitter->spawn_area.y = 0;
	fbl_emitter->spawn_area.w = 10;
	fbl_emitter->spawn_area.h = 10;

	fbl_emitter->img_rect.x = 0;
	fbl_emitter->img_rect.y = 0;
	fbl_emitter->img_rect.w = 5;	/* default to 5x5 rect */
	fbl_emitter->img_rect.h = 5;

	fbl_emitter->scale_start = 1.0;
	fbl_emitter->scale_end = 5.0;

	fbl_emitter->angle_start = 0;
	fbl_emitter->angle_end = 255;

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
	fbl_emitter->life_total = 100;
	fbl_emitter->emit_rate = 1;	// example: 30 would be twice every second at 60 fps. Don't allow division by 0!
	fbl_emitter->density = 3;	// how many emits at once
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

		fbl_emitter_list = DLCreate(current_emitter, fbl_emitter, sizeof(FBL_PARTICLE_EMITTER));


	}
	else
	{

		DLAppend(&fbl_emitter_list, current_emitter, fbl_emitter, sizeof(FBL_PARTICLE_EMITTER));

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

		if (item != DLGetFirst(fbl_emitter_list))	//  this could be: if(id > 0) because the emitters can't be sorted
		{

			/* free particles first */

			free(emitter->particle);

			DLDelete(item);
		}
		else {
			fbl_set_emitter_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
#ifdef FBL_DEBUG
			fprintf(FBL_ERROR_OUT, "Will not delete the first emitter in list! (id: %d) It's just deactivated!\nUse destroy_all to get rid of it..\n", id);
#endif
		}

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

void fbl_set_emitter_color(int id, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool start)
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

void fbl_set_emitter_particle_shape(int id, int shape, int x, int y, int w, int h)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);

		emitter->shape = shape;
		emitter->img_rect.x = x;
		emitter->img_rect.y = y;
		emitter->img_rect.w = w;
		emitter->img_rect.h = h;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set particle shape for particle emitter %d, that does not exist!\n", id);
#endif

}

void fbl_set_emitter_params(int id, int type, int spawn_w, int spawn_h, int life, int rate, int density, float scale_start, float scale_end)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(id);

	if (item != NULL)
	{

		emitter = ((FBL_PARTICLE_EMITTER*)item->Object);


		/* limit to some reasonable values */

		if (life < 1) life = 1;
		if (life > 500) life = 500;

		if (rate < 1) rate = 1;		// avoid division by 0, 1 = one emit every frame
		if (rate > 300) rate = 300;	// one emit every 5 seconds (60 fps) is da limit

		if (density < 1) density = 1;	// min 1 particle / emit
		if (density > 10) density = 10;	// max 10 particles / emit

		if (scale_start < 0.1) scale_start = 0.1f;
		if (scale_end > 20.0) scale_start = 20.0f;


		emitter->type = type;
		emitter->spawn_area.w = spawn_w;
		emitter->spawn_area.h = spawn_h;
		emitter->life_total = life;
		emitter->emit_rate = rate;
		emitter->scale_start = scale_start;
		emitter->scale_end = scale_end;


	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set parameters for particle emitter %d, that does not exist!\n", id);
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

int  fbl_get_num_active_particles(int emitter_id)
{

	FBL_PARTICLE_EMITTER* emitter = NULL;
	DLLIST* item = get_emitter_item_at_id(emitter_id);
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
	else fprintf(FBL_ERROR_OUT, "Tried to get number of active particles for emitter %d, that does not exist!\n", emitter_id);
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
	float val_x_100, rand_amount;

	emitter = (FBL_PARTICLE_EMITTER *)emit;

	//if (emitter->active)
	//{

		if (fbl_engine.current_frame % emitter->emit_rate == 0)
		{

			for (emits = 0; emits < emitter->density; emits++)
			{

				for (i = 0; i < emitter->num_particles; i++)
				{

					// find a free particle

					if (emitter->particle[i].life < 1)
					{

						emitter->particle[i].x = (float)(rand() % emitter->spawn_area.w) + (float)emitter->spawn_area.x;
						emitter->particle[i].y = (float)(rand() % emitter->spawn_area.h) + (float)emitter->spawn_area.y;

						// set velocity based on presets
						switch (emitter->type) {
							case FBL_EMITTER_FLOWER :
								// NOTE: fix this so you can use < 1.0 values
								if (emitter->vel_x_start < 1.0) emitter->vel_x_start = 1.0;
								if (emitter->vel_y_start < 1.0) emitter->vel_y_start = 1.0;
								emitter->particle[i].vel_x = ((float)(rand() % ((int)emitter->vel_x_start * 10)) / 10) - (emitter->vel_x_start / 2);
								emitter->particle[i].vel_y = ((float)(rand() % ((int)emitter->vel_y_start * 10)) / 10) - (emitter->vel_y_start / 2);

								// rotation
								emitter->particle[i].angle = 0.0;
								break;
							case FBL_EMITTER_RAIN:
								emitter->particle[i].vel_x = emitter->vel_x_start;
								emitter->particle[i].vel_y = emitter->vel_y_start;

								// rotation
								emitter->particle[i].angle = 0.0;
								break;
							case FBL_EMITTER_SNOW:
								val_x_100 = emitter->vel_x_start * 100;
								rand_amount = rand() % (int)val_x_100;
								rand_amount /= 100;
								rand_amount -= emitter->vel_x_start / 2;
								emitter->particle[i].vel_x = rand_amount;
								emitter->particle[i].vel_y = emitter->vel_y_start;

								// rotation
								emitter->particle[i].angle = rand() % 255;
								break;

						}

						emitter->particle[i].scale = emitter->scale_start;
						emitter->particle[i].color = emitter->color_start;
						emitter->particle[i].life = emitter->life_total;	// give it life (activate particle)

						i = emitter->num_particles;	// break out of the loop

					}

				}

			}

		}

	//}

	return 0;

}


int update_particle_logic(int tag, void* emit, void* dummy)
{

	FBL_PARTICLE_EMITTER* emitter;
	int i;

	emitter = (FBL_PARTICLE_EMITTER*)emit;


	//if (emitter->active)
	//{

		switch (emitter->type)
		{

			case FBL_EMITTER_FLOWER:

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

					
						// experiment with this velocity thing!
						//emitter->particle[i].vel_x = _lerp(emitter->vel_x_start, emitter->vel_x_end, (float)emitter->particle[i].life / (float)emitter->life_total);
						//emitter->particle[i].vel_y = _lerp(emitter->vel_y_start, emitter->vel_y_end, (float)emitter->particle[i].life / (float)emitter->life_total);



						emitter->particle[i].life--;

					}

				}

			break;

			case FBL_EMITTER_RAIN:

				for (i = 0; i < emitter->num_particles; i++)
				{

					if (emitter->particle[i].life > 0)
					{

						emitter->particle[i].x += emitter->particle[i].vel_x;
						emitter->particle[i].y += emitter->particle[i].vel_y;

						emitter->particle[i].color.a = (Uint8)_lerp((float)emitter->color_start.a, (float)emitter->color_end.a, (float)emitter->particle[i].life / (float)emitter->life_total);

						emitter->particle[i].life--;

					}

				}

			break;
			case FBL_EMITTER_SNOW:

				for (i = 0; i < emitter->num_particles; i++)
				{

					if (emitter->particle[i].life > 0)
					{

						emitter->particle[i].x += emitter->particle[i].vel_x;
						emitter->particle[i].y += emitter->particle[i].vel_y;

						// fade out
						emitter->particle[i].color.a = (Uint8)_lerp((float)emitter->color_start.a, (float)emitter->color_end.a, (float)emitter->particle[i].life / (float)emitter->life_total);

						// spin 360 degrees
						emitter->particle[i].angle = _lerp((float)emitter->angle_start, (float)emitter->angle_end, (float)emitter->particle[i].life / (float)emitter->life_total);

						emitter->particle[i].life--;

					}

				}

				break;
			case FBL_EMITTER_EXPLOSION:
				// add a different particle effect here!
			break;
			case FBL_EMITTER_SMOKE:
				// and here!
				break;

		}

	//}

    
    return 0;

}

void engine_update_all_particles()
{

	DLWalk(fbl_emitter_list, emit_particle, NULL);
	DLWalk(fbl_emitter_list, update_particle_logic, NULL);

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
	int cam_x = 0;
	int cam_y = 0;
	int i;

	emitter = (FBL_PARTICLE_EMITTER*)emit;

	//if (emitter->active)
	//{

		if (!emitter->fix_to_screen) {
			cam_x = fbl_camera.x;
			cam_y = fbl_camera.y;
		}

		switch (emitter->shape) {

			case FBL_NORMAL_RECT :

				SDL_SetRenderDrawBlendMode(fbl_engine.renderer, SDL_BLENDMODE_ADD);

				for (i = 0; i < emitter->num_particles; i++)
				{

					if (emitter->particle[i].life > 0)
					{

						tmp_rect.x = (int)emitter->particle[i].x - cam_x;
						tmp_rect.y = (int)emitter->particle[i].y - cam_y;

						// if prim (with images, scale in the  drawing op)
						tmp_rect.w = (int)(emitter->img_rect.w * emitter->particle[i].scale);
						tmp_rect.h = (int)(emitter->img_rect.h * emitter->particle[i].scale);

						/* culling */

						//if (tmp_rect.x > -tmp_rect.w && tmp_rect.x < (int)fbl_engine.w &&
							//tmp_rect.y > -tmp_rect.h && tmp_rect.y < (int)fbl_engine.h)
						//{

							SDL_SetRenderDrawColor(fbl_engine.renderer, emitter->particle[i].color.r, emitter->particle[i].color.g, emitter->particle[i].color.b, emitter->particle[i].color.a);
							SDL_RenderFillRect(fbl_engine.renderer, &tmp_rect);

						//}

					}

				}
				break;

			case FBL_NO_PRIM :

				SDL_SetTextureBlendMode(fbl_texture, SDL_BLENDMODE_ADD);

				for (i = 0; i < emitter->num_particles; i++)
				{

					if (emitter->particle[i].life > 0)
					{

						tmp_rect.x = (int)emitter->particle[i].x - cam_x;
						tmp_rect.y = (int)emitter->particle[i].y - cam_y;

						// if prim (with images, scale in the  drawing op)
						tmp_rect.w = (int)(emitter->img_rect.w * emitter->particle[i].scale);
						tmp_rect.h = (int)(emitter->img_rect.h * emitter->particle[i].scale);

						SDL_SetTextureAlphaMod(fbl_texture, emitter->particle[i].color.a);
						SDL_SetTextureColorMod(fbl_texture, emitter->particle[i].color.r, emitter->particle[i].color.g, emitter->particle[i].color.b);

						SDL_RenderCopyEx(fbl_engine.renderer, fbl_texture, &emitter->img_rect,
							&tmp_rect, (double)emitter->particle[i].angle, NULL, SDL_FLIP_NONE);

					}

				}
				break;

		}

	//}

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

/* end of particles.c */
