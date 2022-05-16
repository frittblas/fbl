/*
 *
 *	FBL ENGINE
 *
 *	physics.c
 *
 *	Contains physics related functions including setting
 *	up sprites and primitives with chipmunk physics.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */


#include "engine.h"

/* only used by physics.c */


bool fbl_physics_active = false;


cpVect   fbl_phys_gravity;
cpSpace *fbl_phys_space;
cpBody  *fbl_static_body;

/*
 *  init Chipmunk.
 */
int fbl_phys_init()
{

	/* set gravity */

	fbl_phys_gravity = cpv(0, 100);

	/* Create an empty space. */

	fbl_phys_space = cpSpaceNew();
	cpSpaceSetIterations(fbl_phys_space, 5);	// defaults to 10. Lower = better performance and worse simulation.
	cpSpaceSetGravity(fbl_phys_space, fbl_phys_gravity);

	
	fbl_static_body = cpSpaceGetStaticBody(fbl_phys_space);


	fbl_physics_active = true;


	return 0;

}

/*
 *  shut down Chipmunk.
 */
void fbl_phys_shutdown()
{

	if (fbl_physics_active)
	{

		cpSpaceFree(fbl_phys_space);

		fbl_physics_active = false;

	}


}


/*
 *  Step through physics.
 */
void engine_phys_step(int fps)
{

	if (fbl_physics_active)
	{
		cpSpaceStep(fbl_phys_space, 1.0 / (cpFloat)fps);
	}

}

/*
 *  Add a physics body to the space.
 *  
 */
cpBody *engine_phys_add_body(int type, float m, int w, int h, int r, int body_type, bool can_rotate)
{

	cpBody* body = NULL;


	switch (type)
	{
		case FBL_LINE :
			//body = cpSpaceGetStaticBody(fbl_phys_space);	// unnecessary I think..
		break;
		case FBL_RECT :
			if (body_type == FBL_PHYS_DYNAMIC)
				body = cpSpaceAddBody(fbl_phys_space, cpBodyNew(m, can_rotate ? cpMomentForBox(m, (float)w, (float)h) : INFINITY));
			else if (body_type == FBL_PHYS_KINEMATIC)
				body = cpSpaceAddBody(fbl_phys_space, cpBodyNewKinematic());
			else if (body_type == FBL_PHYS_STATIC) {

				//body = cpSpaceAddBody(fbl_phys_space, cpBodyNewKinematic());
				//cpBodySetType(body, CP_BODY_TYPE_STATIC);
				//printf("body-type(cm): %d, body-type(fbl: %d)\n", cpBodyGetType(body), body_type);
				// find a way to add static bodies :)
				// 

			}
		break;
		case FBL_CIRCLE :
			body = cpSpaceAddBody(fbl_phys_space, cpBodyNew(m, can_rotate ? cpMomentForCircle(m, (float)r, (float)r, cpvzero) : INFINITY));
		break;
	}

	return body;

}

cpShape *engine_phys_add_shape(cpBody *body, int type, int x, int y, int w, int h, int r)
{

	cpShape *shape = NULL;

	switch (type)
	{

		case FBL_LINE :
			shape = cpSegmentShapeNew(cpSpaceGetStaticBody(fbl_phys_space), cpv(x, y), cpv(w, h), 0);
			cpShapeSetFriction(shape, 1);
			cpSpaceAddShape(fbl_phys_space, shape);
		break;
		case FBL_RECT :
			shape = cpSpaceAddShape(fbl_phys_space, cpBoxShapeNew(body, (float)w, (float)h, (float)r));
		break;
		case FBL_CIRCLE :
			shape = cpSpaceAddShape(fbl_phys_space, cpCircleShapeNew(body, (float)r, cpvzero));
		break;

	}

	return shape;

}


void engine_phys_remove_shape(cpShape *shape)
{

	cpSpaceRemoveShape(fbl_phys_space, shape);

}

void engine_phys_remove_body(cpBody *body)
{

	cpSpaceRemoveBody(fbl_phys_space, body);

}

cpShape *engine_ray_collided(SDL_Rect *dest_rect, int radius, cpSegmentQueryInfo *segInfo)
{

	cpVect start, end;

	start.x = (cpFloat)dest_rect->x;
	start.y = (cpFloat)dest_rect->y;

	end.x = (cpFloat)dest_rect->w;
	end.y = (cpFloat)dest_rect->h;

	return cpSpaceSegmentQueryFirst(fbl_phys_space, start, end, (cpFloat)radius, CP_SHAPE_FILTER_ALL, segInfo);

}

/* end of physics.c */
