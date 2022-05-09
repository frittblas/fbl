/*
 *
 *	FBL ENGINE
 *
 *	sprite.c
 *
 *	Contains sprite related functions including animation and collision.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* This is how many sprites that use direct reference (the first 1000 or whatever sprites gets stored in an array)
   for super fast access (very useful for setting ang getting values every frame (fbl_set_sprite*())) */

#define NUM_DIRECT_REF_SPRITES 2000


 /* functions only used by sprite.c */

int cmp_layer(void *a, void *b);
int cmp_blendmode(void *a, void *b);
int update_sprite_refs(int tag, void* item, void* dummy);
int destroy_sprite_phys(int tag, void *sprite, void *dummy);
DLLIST *get_sprite_item_at_id(int id);
int  translate_phys_sprite(int tag, void *sprite, void *dummy);
int  animate_sprite(int tag, void *sprite, void *dummy);
int  render_sprite(int tag, void *sprite, void *dummy);


extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

SDL_Texture *fbl_texture = NULL;
DLLIST *fbl_sprite_list = NULL;

DLLIST* direct_sprite_ref[NUM_DIRECT_REF_SPRITES] = {NULL};

unsigned int current_sprite = 0;

int sprite_alignment = FBL_SPRITE_ALIGN_CENTER;

/*
 * load a texture to memory to be used later by sprites 
 * and stuff.
 * NOTE: Make a dll-list with multiple atlases? Maybe ver 2.0.
 */
int fbl_load_texture(const char *img_file)
{

	char* new_path = engine_get_platform_asset_path(img_file);

	SDL_Surface *surface = IMG_Load(new_path);

	if (!surface)
	{
		fprintf(FBL_ERROR_OUT, "IMG_Load: %s\n", IMG_GetError());
		return -1;
	}

	fbl_texture = SDL_CreateTextureFromSurface(fbl_engine.renderer, surface);

	if (!fbl_texture)
	{
		fprintf(FBL_ERROR_OUT, "Texture could not be created: %s\n", SDL_GetError());
		return -1;
	}


	// testing purposes. turns out theyre SUUPER CHEAP can use every frame np (on direct3d atl)

	//SDL_SetTextureBlendMode(fbl_texture, SDL_BLENDMODE_NONE);


	SDL_FreeSurface(surface);

	return 0;

}

void fbl_destroy_texture()
{

	if (!fbl_texture) {
		SDL_DestroyTexture(fbl_texture);
		fbl_texture = NULL;
	}

}


/*
 * Create sprite by adding a source - SDL_Rect which is
 * a location on the atlas-texture, and a dest - SDL_Rect
 * which is a position on the screen.
 */
int fbl_create_sprite(int x, int y, int w, int h, int r)
{

	FBL_SPRITE *fbl_sprite = NULL;

	if (!fbl_texture)
	{
		fprintf(FBL_ERROR_OUT, "Load spritesheet before create sprite!\n");
		return -1;
	}

	fbl_sprite = malloc(sizeof(FBL_SPRITE));

	if (!fbl_sprite)
	{
		fprintf(FBL_ERROR_OUT, "No memory for sprite!\n");
		return -1;
	}


	fbl_sprite->source_rect.x = x;
	fbl_sprite->source_rect.y = y;
	fbl_sprite->source_rect.w = w;
	fbl_sprite->source_rect.h = h;

	fbl_sprite->dest_rect.x = 0;
	fbl_sprite->dest_rect.y = 0;
	fbl_sprite->dest_rect.w = w;
	fbl_sprite->dest_rect.h = h;

	fbl_sprite->radius = r;

	fbl_sprite->angle = 0.0;
	fbl_sprite->scale = 1.0;

	fbl_sprite->blendmode = FBL_BLENDMODE_BLEND;	/* blend, so transparent pngs work */
	fbl_sprite->color.a = 255;	/* start as fully opaque */
	fbl_sprite->color.r = 255;
	fbl_sprite->color.g = 255;
	fbl_sprite->color.b = 255;

	fbl_sprite->flip = SDL_FLIP_NONE;

	fbl_sprite->layer = 0;

	fbl_sprite->fix_to_screen = false;

	fbl_sprite->active = true;

	fbl_sprite->animated = false;
	fbl_sprite->animation_frames = 0;
	fbl_sprite->current_frame = 0;
	fbl_sprite->animation_speed = 0;
	fbl_sprite->anim_loop = false;

	fbl_sprite->physics_on = false;

	//fbl_sprite->shape = FBL_RECT; /* default physics shape */
    
	/* if it's the first sprite created start a new list, otherwise append */

	if (current_sprite == 0)
	{

		fbl_sprite_list = DLCreate(current_sprite, fbl_sprite, sizeof(FBL_SPRITE));

	}
	else
	{

		DLAppend(&fbl_sprite_list, current_sprite, fbl_sprite, sizeof(FBL_SPRITE));

	}


	/* store the last list item in the direct access array */
	
	if(current_sprite < NUM_DIRECT_REF_SPRITES)
		direct_sprite_ref[current_sprite] = DLGetLast(fbl_sprite_list);


	free(fbl_sprite);

	current_sprite++;

	return current_sprite - 1;

}

/*
 * Delete sprite by only removing the item on the list
 */
void fbl_delete_sprite(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		/* security so that the first element can't be deleted, will crash! */

		if (item != DLGetFirst(fbl_sprite_list))	//  before: if(id > 0)
		{

			/* free physics body and shape first */

			if (sprite->physics_on)
			{

				destroy_sprite_phys(0, sprite, NULL);

			}

			/* set the direct ref pointer to null */

			if (id < NUM_DIRECT_REF_SPRITES)
				direct_sprite_ref[id] = NULL;

			DLDelete(item);
		}
		else
			fbl_set_sprite_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
			//fprintf(FBL_ERROR_OUT, "Will not delete the first sprite in list! (id: %d) Use destroy_all..\n", id);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete sprite %d, that does not exist!\n", id);
#endif

}


/*
 * Set and get blend-related stuff. Check available blendmodes in fbl.h
 * An alpha value of 255 is fully opaque. With FBL_BLENDMODE_MOD a sprite
 * looks normal with all values (r,g,b) set to 255.
 */
void fbl_set_sprite_blendmode(int id, int mode)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		sprite->blendmode = mode;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set blendmode for sprite %d, that does not exist!\n", id);
#endif

}

int fbl_get_sprite_blendmode(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->blendmode;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get blendmode for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_sprite_alpha(int id, unsigned char alpha)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		sprite->color.a = alpha;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set alpha for sprite %d, that does not exist!\n", id);
#endif

}

int fbl_get_sprite_alpha(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->color.a;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get alpha for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_sprite_color(int id, unsigned char r, unsigned char g, unsigned char b)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->color.r = r;
		sprite->color.g = g;
		sprite->color.b = b;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set color for sprite %d, that does not exist!\n", id);
#endif

}

void fbl_get_sprite_color(int id, unsigned char *r, unsigned char *g, unsigned char *b)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		*r = sprite->color.r;
		*g = sprite->color.g;
		*b = sprite->color.b;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set color for sprite %d, that does not exist!\n", id);
#endif


}

/*
 * Set which point all sprites are drawn from
 */
void fbl_set_sprite_align(int mode)
{

	sprite_alignment = mode;

}


/*
 * Set which layer the sprite should reside in. Call fbl_sort_sprites after you call this.
 */
void fbl_set_sprite_layer(int id, int layer)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);
		sprite->layer = layer;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set layer for sprite %d, that does not exist!\n", id);
#endif

}

/*
 * Get the layer on which the sprite resides.
 */
int fbl_get_sprite_layer(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->layer;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get layer for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}


/*
 * Self explanatory stuff. Just get and set parameters from the sprite-struct.
 */
void fbl_set_sprite_xy(int id, int x, int y)
{
	
	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if(item != NULL)
	{
	
		sprite = ((FBL_SPRITE *)item->Object);
	
		sprite->dest_rect.x = x;
		sprite->dest_rect.y = y;

		if(sprite->physics_on)
		{
			cpVect pos;
			pos.x = (float)x;
			pos.y = (float)y;
			cpBodySetPosition(sprite->phys_body, pos);
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set xy for sprite %d, that does not exist!\n", id);
#endif

}

int fbl_get_sprite_x(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->dest_rect.x;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get x for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}

int fbl_get_sprite_y(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->dest_rect.y;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get y for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_sprite_angle(int id, double angle)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->angle = angle;

		if (sprite->physics_on)
		{
			cpBodySetAngle(sprite->phys_body, (cpFloat)angle);
		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set angle for sprite %d, that does not exist!\n", id);
#endif

}

double fbl_get_sprite_angle(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->angle;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get angle for sprite %d, that does not exist!\n", id);
#endif

	return 0.0;

}

void fbl_set_sprite_scale(int id, float scale)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->scale = scale;

		/* reset the dest_rect to original */

		sprite->dest_rect.w = sprite->source_rect.w;
		sprite->dest_rect.h = sprite->source_rect.h;

		/* now set the dest rect after the scale */

		sprite->dest_rect.w = (int)(sprite->dest_rect.w * scale);
		sprite->dest_rect.h = (int)(sprite->dest_rect.h * scale);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set scale for sprite %d, that does not exist!\n", id);
#endif

}

float fbl_get_sprite_scale(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->scale;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get scale for sprite %d, that does not exist!\n", id);
#endif

	return 1.0;

}

void fbl_set_sprite_flip(int id, int flipmode)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		switch (flipmode)
		{

			case 0 :
				sprite->flip = SDL_FLIP_NONE;
			break;
			case 1 :
				sprite->flip = SDL_FLIP_HORIZONTAL;
			break;
			case 2 :
				sprite->flip = SDL_FLIP_VERTICAL;
			break;
			case 3 :
				sprite->flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
			break;

		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to flip sprite %d, that does not exist!\n", id);
#endif

}

int fbl_get_sprite_flip(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->flip;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get flipmode for sprite %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_sprite_animation(int id, bool on, int x, int y , int w , int h, int num_frames, int speed, bool loop)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->animated = on;
		sprite->animation_frames = num_frames;
		sprite->current_frame = 0;
		sprite->animation_speed = speed;
		sprite->anim_loop = loop;

		sprite->source_rect.x = x;
		sprite->source_rect.y = y;
		sprite->source_rect.w = w;
		sprite->source_rect.h = h;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set animation for sprite %d, that does not exist!\n", id);
#endif


}

/*
 * If sprite is fixed to screen it ignores camera offset
 */
void fbl_fix_sprite_to_screen(int id, bool fix)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->fix_to_screen = fix;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set fix to screen for sprite %d, that does not exist!\n", id);
#endif

}

/*
 * Basic bounding box collision detection between 2 sprites.
 * Does not work well with angled sprites but works with scale.
 * This is just here for an easy way to do simple collision checking.
 * It's slow, use chipmunk functions instead maybe..
 */
bool fbl_get_sprite_collision(int id_1, int id_2)
{

	FBL_SPRITE *sprite_1;
	FBL_SPRITE *sprite_2;
	DLLIST *item_1 = get_sprite_item_at_id(id_1);
	DLLIST *item_2 = get_sprite_item_at_id(id_2);

	if(item_1 != NULL && item_2 != NULL)
	{

		sprite_1 = ((FBL_SPRITE *)item_1->Object);
		sprite_2 = ((FBL_SPRITE *)item_2->Object);

		if(sprite_1->active && sprite_2->active)
		{

			if ((sprite_1->dest_rect.x > sprite_2->dest_rect.x + sprite_1->dest_rect.w) ||
				(sprite_2->dest_rect.x > sprite_1->dest_rect.x + sprite_1->dest_rect.w) ||
				(sprite_1->dest_rect.y > sprite_2->dest_rect.y + sprite_1->dest_rect.h) ||
				(sprite_2->dest_rect.y > sprite_1->dest_rect.y + sprite_1->dest_rect.h))
				return false;
			else return true;

		}

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get collision for sprites %d and %d, of one or both does not exist!\n", id_1, id_2);
#endif

	return false;

}

/*
 * Activate physics on a sprite. Give it box or circle shape.
 */
void fbl_set_sprite_phys(int id, bool on, int type, int body_type, bool can_rotate)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		if(on)
		{

			/* set body and shape */

			if(!sprite->physics_on)
			{


				switch (type)
				{

					case FBL_RECT:
						sprite->phys_body = engine_phys_add_body(FBL_RECT, 1.0, sprite->dest_rect.w, sprite->dest_rect.h, 0, body_type, can_rotate);
						sprite->phys_shape = engine_phys_add_shape(sprite->phys_body, FBL_RECT, 0, 0, sprite->dest_rect.w, sprite->dest_rect.h, 0);
                        //sprite->shape = FBL_RECT;
					break;
					case FBL_CIRCLE:
						sprite->phys_body = engine_phys_add_body(FBL_CIRCLE, 1.0, sprite->radius, sprite->radius, sprite->radius, body_type, can_rotate);
						sprite->phys_shape = engine_phys_add_shape(sprite->phys_body, FBL_CIRCLE, 0, 0, 0, 0, sprite->radius);
                        //sprite->shape = FBL_CIRCLE;
					break;

				}
			
			
				/* update position */

				cpVect pos;
				pos.x = (float)sprite->dest_rect.x;
				pos.y = (float)sprite->dest_rect.y;
				cpBodySetPosition(sprite->phys_body, pos);

			
			}

		}
		else
		{

			/* free body and shape */

			if (sprite->physics_on)
			{
				destroy_sprite_phys(0, sprite, NULL);
			}

		}

		sprite->physics_on = on;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set physics for sprite %d, that does not exist!\n", id);
#endif

}

/*
 * If sprite is inactive it's not rendered and not checked for collisions.
 */
void fbl_set_sprite_active(int id, bool active)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{

		sprite = ((FBL_SPRITE *)item->Object);

		sprite->active = active;

		/* turn on and off physics, Note: physics gets turned off by deactivating sprite. */

        if(sprite->physics_on && active == false){
			destroy_sprite_phys(0, sprite, NULL);
            sprite->physics_on = false;
        }

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set active for sprite %d, that does not exist!\n", id);
#endif

}

bool fbl_get_sprite_active(int id)
{

	FBL_SPRITE *sprite = NULL;
	DLLIST *item = get_sprite_item_at_id(id);

	if (item != NULL)
	{
		sprite = ((FBL_SPRITE *)item->Object);
		return sprite->active;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get active for sprite %d, that does not exist!\n", id);
#endif

	return false;

}

/*
 * Return total number of sprites created.
 */
int fbl_get_num_sprites()
{

	return DLCount(fbl_sprite_list);

}

/*
 * Sort the spritelist based on layer or blendmode
 */
void fbl_sort_sprites(int mode)
{

	DLLIST *first = NULL;
	DLLIST *last = NULL;

	first = DLGetFirst(fbl_sprite_list);
	last = DLGetLast(fbl_sprite_list);

	if (mode == FBL_SORT_BY_LAYER)
		DLQuickSort(&fbl_sprite_list, &first, &last, cmp_layer);
	else if (mode == FBL_SORT_BY_BLEND)
		DLQuickSort(&fbl_sprite_list, &first, &last, cmp_blendmode);


	/* make sure the direct ref array is correct after sort! */

	DLWalkParent(fbl_sprite_list, update_sprite_refs, NULL);

}

int cmp_layer(void *a, void *b)
{

	/* change "<" to ">" for reverse sorting */

	if (((FBL_SPRITE *)a)->layer < ((FBL_SPRITE *)b)->layer)
		return 1;

	return 0;

}

int cmp_blendmode(void *a, void *b)
{

	if (((FBL_SPRITE *)a)->blendmode < ((FBL_SPRITE *)b)->blendmode)
		return 1;

	return 0;

}

int update_sprite_refs(int tag, void* item, void* dummy)
{
	// static int i = 0;

	if (((DLLIST*)item)->Tag < NUM_DIRECT_REF_SPRITES)
		direct_sprite_ref[((DLLIST*)item)->Tag] = (DLLIST*)item;

	// printf("i = %d, Tag = %d\n", i, ((DLLIST*)item)->Tag);

	// i++;

	return 0;

}

void fbl_destroy_all_sprites()
{

	int i;

	DLWalk(fbl_sprite_list, destroy_sprite_phys, NULL);

	DLDestroy(&fbl_sprite_list);

	/* null all direct ref pointers */

	for (i = 0; i < NUM_DIRECT_REF_SPRITES; i++)
		direct_sprite_ref[i] = NULL;


	current_sprite = 0;

}

int destroy_sprite_phys(int tag, void *sprite, void *dummy)
{

	if(((FBL_SPRITE *)sprite)->physics_on)
	{
		engine_phys_remove_shape(((FBL_SPRITE *)sprite)->phys_shape);
		engine_phys_remove_body(((FBL_SPRITE *)sprite)->phys_body);
		cpShapeFree(((FBL_SPRITE *)sprite)->phys_shape);	/* free shape first! */
		cpBodyFree(((FBL_SPRITE *)sprite)->phys_body);
	}

	return 0;

}

/*
 * Return the correct item at the id.
 *
 * This is really slow, fix some direct reference to the
 * item instead. Testing this now.. OK direct ref is insanely faster!
 *
 */
DLLIST *get_sprite_item_at_id(int id)
{

	DLLIST *item;
	
	if (id < NUM_DIRECT_REF_SPRITES)
	{

		return direct_sprite_ref[id]; // this is fast

	}
	
	item = DLGetFirst(fbl_sprite_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;	// maybe return item->Object? yes in the future :)

		item = item->Next;

	}

	return NULL;

}


/*
 *	Give each sprite new x, y and angle based on physics engine.
 */
int translate_phys_sprite(int tag, void *sprite, void *dummy)
{

	cpVect pos;
	cpFloat angle;

	FBL_SPRITE *spr = (FBL_SPRITE *)sprite;	// spr is now the current sprite to be used

	if(spr->physics_on && spr->active)
	{

		/* first get the physics body's position and angle */

		pos = cpBodyGetPosition(spr->phys_body);
		angle = cpBodyGetAngle(spr->phys_body);

		/* then set it on the sprite */

		spr->dest_rect.x = (int)pos.x;
		spr->dest_rect.y = (int)pos.y;

		/* convert from radians to degrees */

		spr->angle = angle * 180.0 / M_PI;

	}

	return 0;

}

/*
 * Walk through the sprite-list and give them new x, y and angle based on physics engine.
 */
void engine_translate_all_phys_sprites()
{

	DLWalk(fbl_sprite_list, translate_phys_sprite, NULL);

}


/*	
 *	Animate a sprite by changing the source rect coordinates.
 */
int animate_sprite(int tag, void *sprite, void *dummy)
{

	FBL_SPRITE *spr = (FBL_SPRITE *)sprite;	// spr is now the current sprite to be used

	if(spr->animated)
	{

		/* an animation speed of 7 will spend 7 frames on each image */

		if (fbl_engine.current_frame % spr->animation_speed == 0)
		{

			/* move the source rect to the right to change image */

			spr->source_rect.x += spr->source_rect.w;


			/* if on last frame reset xy and frame counter (if looped) */

			spr->current_frame++;
			if (spr->current_frame == spr->animation_frames)
			{
				if (spr->anim_loop) {
					spr->current_frame = 0;
					spr->source_rect.x -= spr->source_rect.w * spr->animation_frames;
				}
				else {
					spr->source_rect.x -= spr->source_rect.w;
					spr->animated = false;
				}
			}


		}


	}

	return 0;

}

/*
 * Walk through the sprite-list and animate all sprites with animated == true.
 */
void engine_animate_all_sprites()
{

	DLWalk(fbl_sprite_list, animate_sprite, NULL);

}

/*
 * Render a sprite complete with camera offsets, angle and flip parameters.
 */
int render_sprite(int tag, void *sprite, void *dummy)
{

	/* camera adjustment */

	SDL_Rect temp_rect;


	FBL_SPRITE *spr = (FBL_SPRITE *)sprite;	// spr is now the current sprite to be used


	if (spr->active)
	{

		if (spr->fix_to_screen)
		{
			temp_rect.x = spr->dest_rect.x;
			temp_rect.y = spr->dest_rect.y;
		}
		else
		{
			temp_rect.x = spr->dest_rect.x - fbl_camera.x;
			temp_rect.y = spr->dest_rect.y - fbl_camera.y;
		}


		/* adjust so sprite gets drawn from the correct point */

		switch (sprite_alignment)
		{
			case FBL_SPRITE_ALIGN_UP_LEFT:
				// do nothing :)
			break;
			case FBL_SPRITE_ALIGN_CENTER:
				temp_rect.x = temp_rect.x - spr->dest_rect.w / 2;
				temp_rect.y = temp_rect.y - spr->dest_rect.h / 2;
			break;
			case FBL_SPRITE_ALIGN_DOWN_LEFT:
				temp_rect.y = temp_rect.y - spr->dest_rect.h;
			break;
		}


		temp_rect.w = spr->dest_rect.w;
		temp_rect.h = spr->dest_rect.h;


		/* Cull sprites (don't draw sprites outside the screen), this helps performance alot for big tilemaps etc (fix_to_screen-sprites are always shown (if active))
		   This also takes in a count the different sprite-alignmets */

		if (temp_rect.x > -temp_rect.w && temp_rect.x < (int)fbl_engine.w &&
			temp_rect.y > -temp_rect.h && temp_rect.y < (int)fbl_engine.h)

		{


			/* set blend modes and colors and alpha (benchmark this! update: switching blendmodes is kinda slow the rest very fast */

				SDL_SetTextureBlendMode(fbl_texture, spr->blendmode);
				SDL_SetTextureAlphaMod(fbl_texture, spr->color.a);
				SDL_SetTextureColorMod(fbl_texture, spr->color.r, spr->color.g, spr->color.b);


				SDL_RenderCopyEx(fbl_engine.renderer, fbl_texture, &spr->source_rect,
					&temp_rect, spr->angle, NULL, spr->flip);


		}
	}


	return 0;

}

/*
 * Walk through the sprite-list and render all active sprites.
 */
void engine_render_all_sprites()
{

	DLWalk(fbl_sprite_list, render_sprite, NULL);

}

/* end of sprite.c */
