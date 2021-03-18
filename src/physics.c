/*
 *
 *	FBL ENGINE
 *
 *	physics.c
 *
 *	Contains physics related functions including setting
 *	up sprites and primitives with chipmunk physics.
 *
 *	Hans Strömquist 2017 - 2020
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

	cpBody *body = NULL;


	switch (type)
	{
		case FBL_LINE :
			body = cpSpaceGetStaticBody(fbl_phys_space);
		break;
		case FBL_RECT :
            		body = cpSpaceAddBody(fbl_phys_space, cpBodyNew(m, can_rotate ? cpMomentForBox(m, (float)w, (float)h) : INFINITY));
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



//cpShape *cpSpaceAddShape(cpSpace *space, cpShape *shape)
//cpBody *cpSpaceAddBody(cpSpace *space, cpBody *body)

void engine_phys_remove_shape(cpShape *shape)
{

	cpSpaceRemoveShape(fbl_phys_space, shape);

}

void engine_phys_remove_body(cpBody *body)
{

	cpSpaceRemoveBody(fbl_phys_space, body);

}

/* end of physics.c */
