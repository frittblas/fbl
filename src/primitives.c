/*
 *
 *	FBL ENGINE
 *
 *	primitives.c
 *
 *	Contains primitive-shape-related functions including drawing and setting up physics.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* check sprite version for info */

#define NUM_DIRECT_REF_PRIMS 2000

/* functions only used by primitives.c */

int destroy_prim_phys(int tag, void *prim, void *dummy);
DLLIST *get_prim_item_at_id(int id);
int  translate_phys_prim(int tag, void *prim, void *dummy);
int  render_prim(int tag, void *prim, void *dummy);
void draw_circle(int x0, int y0, int radius);
void draw_circle_(int x0, int y0, int radius);
void draw_circle_fill(int x, int y, int radius);
void translate_shape(FBL_PRIM *obj);
void translate_point(A_POINT *dest, A_POINT *src, double angle);
void draw_shape(FBL_PRIM *obj);

extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

DLLIST *fbl_prim_list = NULL;

DLLIST* direct_prim_ref[NUM_DIRECT_REF_PRIMS] = {NULL};

unsigned int current_prim = 0;


/*
 * Create primitive of specified type.
 */
int fbl_create_prim(uint8_t type, int x, int y, int w, int h, int r, bool thick, bool fill)
{

	FBL_PRIM *fbl_prim = NULL;

	fbl_prim = malloc(sizeof(FBL_PRIM));

	if (!fbl_prim)
	{
		fprintf(FBL_ERROR_OUT, "No memory for primitive!\n");
		return -1;
	}

	fbl_prim->type = type;
	
	fbl_prim->dest_rect.x = x;
	fbl_prim->dest_rect.y = y;
	fbl_prim->dest_rect.w = w;
	fbl_prim->dest_rect.h = h;

	fbl_prim->radius = r;


	switch (fbl_prim->type)
	{
		case FBL_TRI :
			fbl_prim->shape.num_points = 3;
			fbl_prim->shape.point[0].x = 0;
			fbl_prim->shape.point[0].y = -w;
			fbl_prim->shape.point[1].x = -h;
			fbl_prim->shape.point[1].y = h;
			fbl_prim->shape.point[2].x = h;
			fbl_prim->shape.point[2].y = h;
		break;
		case FBL_RECT :
			fbl_prim->shape.num_points = 4;
			fbl_prim->shape.point[0].x = -w;
			fbl_prim->shape.point[0].y = -h;
			fbl_prim->shape.point[1].x = -w;
			fbl_prim->shape.point[1].y = h;
			fbl_prim->shape.point[2].x = w;
			fbl_prim->shape.point[2].y = h;
			fbl_prim->shape.point[3].x = w;
			fbl_prim->shape.point[3].y = -h;
		break;
	}

	fbl_prim->angle = 0.0;
	fbl_prim->scale = 1.0;


	/* default to nice orange color */

	fbl_prim->color.r = 255;
	fbl_prim->color.g = 69;
	fbl_prim->color.b = 0;
	fbl_prim->color.a = 255;

	fbl_prim->fill = fill;
	fbl_prim->thick = thick;

	fbl_prim->fix_to_screen = false;

	fbl_prim->active = true;


	fbl_prim->physics_on = false;


	if (fbl_prim->type == FBL_TRI || fbl_prim->type == FBL_RECT) translate_shape(fbl_prim);

	/* if it's the first prim created start a new list, otherwise append */

	if (current_prim == 0)
	{

		fbl_prim_list = DLCreate(current_prim, fbl_prim, sizeof(FBL_PRIM));

	}
	else
	{

		DLAppend(&fbl_prim_list, current_prim, fbl_prim, sizeof(FBL_PRIM));

	}


	/* store the last list item in the direct access array */

	if (current_prim < NUM_DIRECT_REF_PRIMS)
		direct_prim_ref[current_prim] = DLGetLast(fbl_prim_list);


	free(fbl_prim);

	current_prim++;

	return current_prim - 1;

}

/*
 * Delete primitive by only removing the item on the list
 */
void fbl_delete_prim(int id)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		/* security so that the first element can't be deleted, will crash! */

		if (id > 0)
		{

			/* free physics body and shape first */

			if(prim->physics_on)
			{
				destroy_prim_phys(0, prim, NULL);
			}

			DLDelete(item);

			/* set the direct ref pointer to null */

			if (id < NUM_DIRECT_REF_PRIMS)
				direct_prim_ref[id] = NULL;

		}
		else {
			fbl_set_prim_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
#ifdef FBL_DEBUG
			fprintf(FBL_ERROR_OUT, "Will not delete the first prim in list! (id: %d) It's just deactivated!\nUse destroy_all to get rid of it..\n", id);
#endif
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete primitive %d, that does not exist!\n", id);
#endif

}


/*
* Self explanatory stuff. Just get and set parameters from the prim-struct.
*/
void fbl_set_prim_color(int id, int r, int g, int b, int a)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		prim->color.r = r;
		prim->color.g = g;
		prim->color.b = b;
		prim->color.a = a;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set color for primitive %d, that does not exist!\n", id);
#endif

}

void fbl_set_prim_size(int id, int w, int h, int r)
{

	FBL_PRIM* prim = NULL;
	DLLIST* item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM*)item->Object);

		prim->dest_rect.w = w;
		prim->dest_rect.h = h;

		prim->radius = r;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set size for primitive %d, that does not exist!\n", id);
#endif

}

void fbl_set_prim_xy(int id, int x, int y)
{
	
	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if(item != NULL)
	{
	
		prim = ((FBL_PRIM *)item->Object);
	
		prim->dest_rect.x = x;
		prim->dest_rect.y = y;

		if(prim->physics_on)
		{
			cpVect pos;
			pos.x = (float)x;
			pos.y = (float)y;
			cpBodySetPosition(prim->phys_body, pos);
		}

		if(prim->type == FBL_TRI || prim->type == FBL_RECT) translate_shape((FBL_PRIM *)prim);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set xy for primitive %d, that does not exist!\n", id);
#endif

}

int fbl_get_prim_x(int id)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{
		prim = ((FBL_PRIM *)item->Object);
		return prim->dest_rect.x;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get x for primitive %d, that does not exist!\n", id);
#endif

	return 0;

}

int fbl_get_prim_y(int id)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{
		prim = ((FBL_PRIM *)item->Object);
		return prim->dest_rect.y;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get y for primitive %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_prim_angle(int id, double angle)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		prim->angle = angle;

		if (prim->physics_on)
		{
			cpBodySetAngle(prim->phys_body, (cpFloat)angle);
		}

		if (prim->type == FBL_TRI || prim->type == FBL_RECT) translate_shape((FBL_PRIM *)prim);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set angle for primitive %d, that does not exist!\n", id);
#endif

}

double fbl_get_prim_angle(int id)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{
		prim = ((FBL_PRIM *)item->Object);
		return prim->angle;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get angle for primitive %d, that does not exist!\n", id);
#endif

	return 0.0;

}

/*
 * If prim is fixed to screen it ignores camera offset
 */
void fbl_fix_prim_to_screen(int id, bool fix)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		prim->fix_to_screen = fix;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set fix to screen for primitive %d, that does not exist!\n", id);
#endif

}

/*
 * Basic bounding box collision detection between 2 primitives.
 * Does not work well with angled prims but works with scale.
 */
bool fbl_get_prim_collision(int id_1, int id_2)
{

	FBL_PRIM *prim_1;
	FBL_PRIM *prim_2;
	DLLIST *item_1 = get_prim_item_at_id(id_1);
	DLLIST *item_2 = get_prim_item_at_id(id_2);

	if(item_1 != NULL && item_2 != NULL)
	{

		prim_1 = ((FBL_PRIM *)item_1->Object);
		prim_2 = ((FBL_PRIM *)item_2->Object);

		if(prim_1->active && prim_2->active)
		{

			if ((prim_1->dest_rect.x > prim_2->dest_rect.x + prim_1->dest_rect.w) ||
				(prim_2->dest_rect.x > prim_1->dest_rect.x + prim_1->dest_rect.w) ||
				(prim_1->dest_rect.y > prim_2->dest_rect.y + prim_1->dest_rect.h) ||
				(prim_2->dest_rect.y > prim_1->dest_rect.y + prim_1->dest_rect.h))
				return false;
			else return true;

		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get collision for primitives %d and %d, of one or both does not exist!\n", id_1, id_2);
#endif

	return false;

}

/*
 * Activate physics on a prim. Will get correct shape. Lines will get static thingy.
 */
void fbl_set_prim_phys(int id, bool on, int body_type, bool can_rotate)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		if(on)
		{

			/* set body and shape */

			if(!prim->physics_on)
			{

				switch (prim->type)
				{

					case FBL_LINE :
						prim->phys_body = engine_phys_add_body(prim->type, 0.0, 0, 0, 0, body_type, can_rotate);
						prim->phys_shape = engine_phys_add_shape(prim->phys_body, prim->type, prim->dest_rect.x,
																							  prim->dest_rect.y,
																							  prim->dest_rect.w,
																							  prim->dest_rect.h, 0);
					break;
					case FBL_RECT :
						prim->phys_body = engine_phys_add_body(FBL_RECT, 1.0, prim->dest_rect.w * 2, prim->dest_rect.h * 2, 0, body_type, can_rotate);
						prim->phys_shape = engine_phys_add_shape(prim->phys_body, FBL_RECT, 0, 0, prim->dest_rect.w * 2, prim->dest_rect.h * 2, 0);
					break;
					case FBL_CIRCLE:
						prim->phys_body = engine_phys_add_body(prim->type, 1.0, prim->radius, prim->radius, prim->radius, body_type, can_rotate);
						prim->phys_shape = engine_phys_add_shape(prim->phys_body, prim->type, 0, 0, 0, 0, prim->radius);
					break;

				}


				/* update position */

				if(prim->type != FBL_LINE && prim->type != FBL_RAY)
				{
					cpVect pos;
					pos.x = (float)prim->dest_rect.x;
					pos.y = (float)prim->dest_rect.y;
					cpBodySetPosition(prim->phys_body, pos);
				}

			}

		}
		else
		{

			/* free body and shape */

			if (prim->physics_on)
			{
				destroy_prim_phys(0, prim, NULL);
			}

		}

		if(prim->type != FBL_RAY) prim->physics_on = on;	// rays always have "physics on"

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set physics for primitive %d, that does not exist!\n", id);
#endif

}

/*
 * If prim is inactive it's not rendered and not checked for collisions.
 */
void fbl_set_prim_active(int id, bool active)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{

		prim = ((FBL_PRIM *)item->Object);

		prim->active = active;

		/* turn on and off physics, maybe fix this in the future so you don't have to
		 * set the physics params again after reactivate (i.e mass, friction etc).
		 * Same on sprite. It's fine for now. Physics get destroyed with deactivation of the prim/sprite.
		 * You have to fbl_set_prim_phys() again
		 */

		if (prim->physics_on && active == false) {
			destroy_prim_phys(0, prim, NULL);
			prim->physics_on = false;
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set active for primitive %d, that does not exist!\n", id);
#endif

}

bool fbl_get_prim_active(int id)
{

	FBL_PRIM *prim = NULL;
	DLLIST *item = get_prim_item_at_id(id);

	if (item != NULL)
	{
		prim = ((FBL_PRIM *)item->Object);
		return prim->active;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get active for primitive %d, that does not exist!\n", id);
#endif

	return false;

}

/*
 * Return total number of prims created.
 */
int fbl_get_num_prims()
{

	return DLCount(fbl_prim_list);

}

void fbl_destroy_all_prims()
{

	int i;

	DLWalk(fbl_prim_list, destroy_prim_phys, NULL);

	DLDestroy(&fbl_prim_list);

	/* null all direct ref pointers */

	for (i = 0; i < NUM_DIRECT_REF_PRIMS; i++)
		direct_prim_ref[i] = NULL;


	current_prim = 0;

}

int destroy_prim_phys(int tag, void *prim, void *dummy)
{

	if(((FBL_PRIM *)prim)->physics_on)
	{
		// lines have static body (no need to free), could have used cpBodyGetType(body) also, check for static body.

		engine_phys_remove_shape(((FBL_PRIM *)prim)->phys_shape);
        if(((FBL_PRIM *)prim)->type != FBL_LINE) engine_phys_remove_body(((FBL_PRIM *)prim)->phys_body);
		cpShapeFree(((FBL_PRIM *)prim)->phys_shape);	/* free shape first!! */
		if(((FBL_PRIM *)prim)->type != FBL_LINE) cpBodyFree(((FBL_PRIM *)prim)->phys_body);
	}

	return 0;

}

/*
 * Return the correct item at the id.
 * Check sprite-version for details.
 */
DLLIST *get_prim_item_at_id(int id)
{

	DLLIST *item;

	if (id < NUM_DIRECT_REF_PRIMS)
	{

		return direct_prim_ref[id]; // this is fast

	}

	item = DLGetFirst(fbl_prim_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;

        item = item->Next;

	}

	return NULL;

}


/*
 *	Give each prim new x, y and angle based on physics engine.
 */
int translate_phys_prim(int tag, void *prim, void *dummy)
{

	cpVect pos;
	cpFloat angle;

	FBL_PRIM* p = (FBL_PRIM*)prim;	// p is now the current primitive to be used

	if(p->physics_on && p->active && p->type != FBL_LINE)
	{

		/* first get the physics body's position and angle */

		pos = cpBodyGetPosition(p->phys_body);
		angle = cpBodyGetAngle(p->phys_body);

		/* then set it on the prim */

		p->dest_rect.x = (int)pos.x;
		p->dest_rect.y = (int)pos.y;

		/* convert from radians to degrees */

		p->angle = angle * 180.0 / M_PI;

		if(p->type == FBL_TRI || p->type == FBL_RECT) translate_shape(p);

	}
	if (p->type == FBL_RAY)
	{

		// check the return value of this (cpShape *) to get what you collided with
		if (engine_ray_collided(&p->dest_rect, p->radius, &p->segInfo))
		{

			cpVect point = p->segInfo.point;	// the point of colission

			// check out Query.c in the chipmunk demo for more information
			//ChipmunkDemoPrintString("Segment Query: Dist(%f) Normal(%5.2f, %5.2f)", segInfo.alpha * cpvdist(start, end), n.x, n.y);

			//if (fbl_get_raw_frames_count() % 10 == 1)
				//printf("X = %f         Y = %f\n", point.x, point.y);

		}

	}

	return 0;

}


/*
 * Walk through the prim-list and give them new x, y and angle based on physics engine.
 */
void engine_translate_all_phys_prims()
{

	DLWalk(fbl_prim_list, translate_phys_prim, NULL);

}


/*
 * Render a prim complete with camera offsets and angle.
 */
int render_prim(int tag, void *prim, void *dummy)
{

	cpVect start, end;	// for raycast

	/* camera adjustment */

	SDL_Rect temp_rect;

	FBL_PRIM* p = (FBL_PRIM*)prim;	// p is now the current primitive to be used
    
    if (p->active)
    {

        if(p->fix_to_screen)
        {
            temp_rect.x = p->dest_rect.x;
            temp_rect.y = p->dest_rect.y;
        }
        else
        {
            temp_rect.x = p->dest_rect.x - fbl_camera.x;
            temp_rect.y = p->dest_rect.y - fbl_camera.y;
        }

        temp_rect.w = p->dest_rect.w;
        temp_rect.h = p->dest_rect.h;

		if (temp_rect.x > -temp_rect.w && temp_rect.x < (int)fbl_engine.w &&
			temp_rect.y > -temp_rect.h && temp_rect.y < (int)fbl_engine.h)

		{

			SDL_SetRenderDrawColor(fbl_engine.renderer, p->color.r, p->color.g, p->color.b, p->color.a);
			SDL_SetRenderDrawBlendMode(fbl_engine.renderer, SDL_BLENDMODE_BLEND);

			switch (p->type)
			{

				case FBL_POINT:
					SDL_RenderDrawPoint(fbl_engine.renderer, temp_rect.x, temp_rect.y);
					if (p->thick)
					{
						SDL_RenderDrawPoint(fbl_engine.renderer, temp_rect.x + 1, temp_rect.y);
						SDL_RenderDrawPoint(fbl_engine.renderer, temp_rect.x, temp_rect.y + 1);
						SDL_RenderDrawPoint(fbl_engine.renderer, temp_rect.x + 1, temp_rect.y + 1);
					}
				break;
				case FBL_LINE:
					SDL_RenderDrawLine(fbl_engine.renderer, temp_rect.x, temp_rect.y, temp_rect.w, temp_rect.h);
					if (p->thick)
						SDL_RenderDrawLine(fbl_engine.renderer, temp_rect.x + 1, temp_rect.y + 1, temp_rect.w + 1, temp_rect.h + 1);
				break;
				case FBL_TRI:
				case FBL_RECT:
					if (p->fill)
					{
						temp_rect.x = temp_rect.x - temp_rect.w;
						temp_rect.y = temp_rect.y - temp_rect.h;
						temp_rect.w *= 2;
						temp_rect.h *= 2;
						SDL_RenderFillRect(fbl_engine.renderer, &temp_rect);
					}
					else draw_shape(p);
				break;
				case FBL_CIRCLE:
					if (p->fill)
					{
						draw_circle_fill(temp_rect.x, temp_rect.y, p->radius);
					}
					else
					{
						draw_circle_(temp_rect.x, temp_rect.y, p->radius);
						if (p->thick)
							draw_circle_(temp_rect.x, temp_rect.y, p->radius - 1);
					}
				break;
				case FBL_RAY:

					start.x = (cpFloat)temp_rect.x;
					start.y = (cpFloat)temp_rect.y;
					end.x = (cpFloat)temp_rect.w;
					end.y = (cpFloat)temp_rect.h;

					end = cpvlerp(start, end, p->segInfo.alpha);

					temp_rect.w = (int)end.x;
					temp_rect.h = (int)end.y;

					SDL_RenderDrawLine(fbl_engine.renderer, temp_rect.x, temp_rect.y, temp_rect.w, temp_rect.h);
					if (p->thick)
						SDL_RenderDrawLine(fbl_engine.renderer, temp_rect.x + 1, temp_rect.y + 1, temp_rect.w + 1, temp_rect.h + 1);
				break;
				case 8:
					// draw a normal sized rect from the top left corner :)
					SDL_RenderFillRect(fbl_engine.renderer, &temp_rect);

				break;

			}

		} // culling
        
	}

	return 0;

}

/*
 * Walk through the prim-list and render all active prims.
 */
void engine_render_all_prims()
{

	DLWalk(fbl_prim_list, render_prim, NULL);

}

/*
 * draw the actual primitives!
 *
 * todo: find a way to draw filled polygons fast! And filled circle.
 *
 * as for now filled circle is unusably slow, filled rect is very fast,
 * normal circle is pretty fast (due to using an 8th of the drawcalls) and the rest in between.
 *
 */

void draw_circle(int x0, int y0, int radius)
{
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 + x, y0 + y);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 + y, y0 + x);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 - y, y0 + x);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 - x, y0 + y);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 - x, y0 - y);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 - y, y0 - x);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 + y, y0 - x);
		SDL_RenderDrawPoint(fbl_engine.renderer, x0 + x, y0 - y);

		y += 1;
		if (err <= 0)
		{
			err += 2 * y + 1;
		}
		else {
			x -= 1;
			err += 2 * (y - x) + 1;
		}
	}
}


void draw_circle_(int x0, int y0, int radius)
{

	SDL_Point point[8];

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		point[0].x = x0 + x; point[0].y = y0 + y;
		point[1].x = x0 + y; point[1].y = y0 + x;
		point[2].x = x0 - y; point[2].y = y0 + x;
		point[3].x = x0 - x; point[3].y = y0 + y;
		point[4].x = x0 - x; point[4].y = y0 - y;
		point[5].x = x0 - y; point[5].y = y0 - x;
		point[6].x = x0 + y; point[6].y = y0 - x;
		point[7].x = x0 + x; point[7].y = y0 - y;

		/* draw 8 pixels in a batch */
		SDL_RenderDrawPoints(fbl_engine.renderer, point, 8);

		y += 1;
		if (err <= 0)
		{
			err += 2 * y + 1;
		}
		else {
			x -= 1;
			err += 2 * (y - x) + 1;
		}
	}
}

/*
 *  This is insanely slow! Only to be used for a couple of small circles or whatever. Will do better. Don't use :}
 */
void draw_circle_fill(int x, int y, int radius)
{

	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx*dx + dy*dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(fbl_engine.renderer, x + dx, y + dy);	// this is the bad part, thousands of draw calls for one circle..
			}
		}
	}

}


void translate_shape(FBL_PRIM *obj)
{

	int i;

	if (obj->active) {

		for (i = 0; i<obj->shape.num_points; i++) {

			translate_point(&obj->scr.point[i], &obj->shape.point[i], obj->angle);
			obj->scr.point[i].x += obj->dest_rect.x;
			obj->scr.point[i].y += obj->dest_rect.y;

		}

	}
}

void translate_point(A_POINT *dest, A_POINT *src, double angle)
{

	double radians;

	radians = angle * 0.01745;

	dest->x = src->x * cos(radians) - src->y * sin(radians);
	dest->y = src->x * sin(radians) + src->y * cos(radians);

}


/* 
 *  Reasonably fast way of drawing 2D polygons.
 */
void draw_shape(FBL_PRIM *obj)
{

	int i;


	if(obj->thick)
	{

		/* cool 3d effect :) */

		int dist = 2;	/* how far away the shadow is */

		/* set slightly darker shadow */

		SDL_SetRenderDrawColor(fbl_engine.renderer, (Uint8)(obj->color.r * 0.6), (Uint8)(obj->color.g * 0.6), (Uint8)(obj->color.b * 0.6), (Uint8)(obj->color.a * 0.5));

		/* draw offset shadow */

		for(i = 1; i<obj->shape.num_points; i++)
			SDL_RenderDrawLine(fbl_engine.renderer, (int)obj->scr.point[i - 1].x + dist, (int)obj->scr.point[i - 1].y - dist, (int)obj->scr.point[i].x + dist, (int)obj->scr.point[i].y - dist);
			SDL_RenderDrawLine(fbl_engine.renderer, (int)obj->scr.point[i - 1].x + dist, (int)obj->scr.point[i - 1].y - dist, (int)obj->scr.point[0].x + dist, (int)obj->scr.point[0].y - dist);


		/* set drawing color back */

		SDL_SetRenderDrawColor(fbl_engine.renderer, obj->color.r, obj->color.g, obj->color.b, obj->color.a);

	}


	/* draw a 2D polygon */

	for(i = 1; i<obj->shape.num_points; i++)
		SDL_RenderDrawLine(fbl_engine.renderer, (int)obj->scr.point[i - 1].x, (int)obj->scr.point[i - 1].y, (int)obj->scr.point[i].x, (int)obj->scr.point[i].y);
		SDL_RenderDrawLine(fbl_engine.renderer, (int)obj->scr.point[i - 1].x, (int)obj->scr.point[i - 1].y, (int)obj->scr.point[0].x, (int)obj->scr.point[0].y);


}

/* end of primitives.c */
