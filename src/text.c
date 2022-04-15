/*
 *
 *	FBL ENGINE
 *
 *	text.c
 *
 *	Contains text related functions including loading and
 *	displaying TTF fonts.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"

/* for string formatting */
#include <stdarg.h>

/* functions only used by text.c */

int destroy_text_object_image(int tag, void *text_obj, void *dummy);
DLLIST *get_text_item_at_id(int id);
int render_text_object(int tag, void *text_obj, void *dummy);


extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

TTF_Font *fbl_font = NULL;
DLLIST *fbl_text_object_list = NULL;

unsigned int current_text_object = 0;

/*
 * load a ttf font to memory.
 */
int fbl_load_ttf_font(const char *font_file, int size)
{

	/* close font in case it was open */

	if (fbl_font != NULL)
		TTF_CloseFont(fbl_font);

	char* new_path = engine_get_platform_asset_path(font_file);

	fbl_font = TTF_OpenFont(new_path, size);

	if (!fbl_font)
	{
		fprintf(FBL_ERROR_OUT, "TTF_Open: %s\n", TTF_GetError());
		return -1;
	}

	return 0;

}

/*
 * Close font file.
 */
void fbl_destroy_ttf_font()
{

	if (fbl_font != NULL) {
		TTF_CloseFont(fbl_font);
		fbl_font = NULL;
	}

}

/*
 * Create text object.
 */
int fbl_create_text(int r, int g, int b, int a, char *format, ...)
{

	/* put the formatted text in text_buf */

	va_list args;
	char text_buf[256];
	va_start(args, format);
#ifdef _MSC_VER
	vsprintf_s(text_buf, 256,format, args);
#else
	vsprintf(text_buf, format, args);
#endif
	va_end(args);


	FBL_TEXT_OBJECT *fbl_text_object = NULL;

	if (!fbl_font)
	{
		fprintf(FBL_ERROR_OUT, "Load font before create text!\n");
		return -1;
	}

	fbl_text_object = malloc(sizeof(FBL_TEXT_OBJECT));

	if (!fbl_text_object)
	{
		fprintf(FBL_ERROR_OUT, "No memory for text object!\n");
		return -1;
	}


	/* set x, y to top left corner */

	fbl_text_object->dest_rect.x = 0;
	fbl_text_object->dest_rect.y = 0;

	fbl_text_object->scale = 1.0;

	/* render the string to a texture and keep the texture in list */

	SDL_Color fg = { r, g, b, a };
	SDL_Surface *text_surface = TTF_RenderText_Blended(fbl_font, text_buf, fg);

	if(!text_surface)
	{
		fprintf(FBL_ERROR_OUT, "TTF error! %s\n", TTF_GetError());
		return -1;
	}
	fbl_text_object->text_image = SDL_CreateTextureFromSurface(fbl_engine.renderer, text_surface);
	if (!text_surface)
	{
		fprintf(FBL_ERROR_OUT, "SDL error! %s\n", SDL_GetError());
		return -1;
	}
	SDL_QueryTexture(fbl_text_object->text_image, NULL, NULL,
					 &fbl_text_object->dest_rect.w, &fbl_text_object->dest_rect.h);
	SDL_FreeSurface(text_surface);


	/* text objects are fixed by default */

	fbl_text_object->fix_to_screen = true;

	fbl_text_object->align = FBL_ALIGN_LEFT;

	fbl_text_object->active = true;

	/* if it's the first text object created start a new list, otherwise append */

	if (current_text_object == 0)
	{

		fbl_text_object_list = DLCreate(current_text_object , fbl_text_object, sizeof(FBL_TEXT_OBJECT));

	}
	else
	{

		DLAppend(&fbl_text_object_list, current_text_object, fbl_text_object, sizeof(FBL_TEXT_OBJECT));

	}


	free(fbl_text_object);

	current_text_object++;

	return current_text_object - 1;

}

/*
 * Update existing text object with new color and text + format.
 */
int fbl_update_text(int id, int r, int g, int b, int a, char *format, ...)
{


	/* get string and args */

	va_list args;
	char text_buf[256];
	va_start(args, format);
#ifdef _MSC_VER
	vsprintf_s(text_buf, 256, format, args);
#else
	vsprintf(text_buf, format, args);
#endif
	va_end(args);


	/* get the specific text object */

	FBL_TEXT_OBJECT *fbl_text_object = NULL;
	DLLIST *item = NULL;

	item = get_text_item_at_id(id);

	if (item != NULL)
	{

		fbl_text_object = ((FBL_TEXT_OBJECT *)item->Object);


		/* render the string to a texture and keep the texture in list */

		SDL_Color fg = { r, g, b, a };
		SDL_Surface *text_surface = TTF_RenderText_Blended(fbl_font, text_buf, fg);

		if (!text_surface)
		{
			fprintf(FBL_ERROR_OUT, "TTF error! %s\n", TTF_GetError());
			return -1;
		}
		SDL_DestroyTexture(fbl_text_object->text_image);
		fbl_text_object->text_image = SDL_CreateTextureFromSurface(fbl_engine.renderer, text_surface);
		if (!text_surface)
		{
			fprintf(FBL_ERROR_OUT, "SDL error! %s\n", SDL_GetError());
			return -1;
		}
		SDL_QueryTexture(fbl_text_object->text_image, NULL, NULL,
			&fbl_text_object->dest_rect.w, &fbl_text_object->dest_rect.h);
		SDL_FreeSurface(text_surface);

		/* set scale */

		fbl_text_object->dest_rect.w = (int)(fbl_text_object->dest_rect.w * fbl_text_object->scale);
		fbl_text_object->dest_rect.h = (int)(fbl_text_object->dest_rect.h * fbl_text_object->scale);

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to update text object %d, that does not exist!\n", id);
#endif

	/* return o for success */

	return 0;

}

/*
 * Delete text object by destroying the texture and freeing the dllist item.
 */
void fbl_delete_text(int id)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{
		text_obj = ((FBL_TEXT_OBJECT *)item->Object);

		/* security so that the first element can't be deleted, will crash! */

		if (id > 0) {
			SDL_DestroyTexture(text_obj->text_image);
			DLDelete(item);
		}
		else
			fbl_set_text_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
			//fprintf(FBL_ERROR_OUT, "Will not delete the first text object in list! Use destroy_all..\n");

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete text %d, object that does not exist!\n", id);
#endif

}


/*
 * Self explanatory stuff. Get and set parameters from the text object struct.
 */
void fbl_set_text_xy(int id, int x, int y)
{
	
	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		text_obj->dest_rect.x = x;
		text_obj->dest_rect.y = y;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set xy for text object %d, that does not exist!\n", id);
#endif

}

int fbl_get_text_x(int id)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		return text_obj->dest_rect.x;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get x for text object %d, that does not exist!\n", id);
#endif

	return 0;

}

int fbl_get_text_y(int id)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		return text_obj->dest_rect.y;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get y for text object %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_text_scale(int id, float scale)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		text_obj->scale = scale;

		/* reset the dest_rect to original */

		SDL_QueryTexture(text_obj->text_image, NULL, NULL,
			&text_obj->dest_rect.w, &text_obj->dest_rect.h);

		/* now set the dest rect after the scale */

		text_obj->dest_rect.w = (int)(text_obj->dest_rect.w * scale);
		text_obj->dest_rect.h = (int)(text_obj->dest_rect.h * scale);


	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set scale to text object %d, that does not exist!\n", id);
#endif

}

float fbl_get_text_scale(int id)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		return text_obj->scale;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get scale for text object %d, that does not exist!\n", id);
#endif

	return 1.0;

}

/*
 * Set text alignment.
 */
void fbl_set_text_align(int id, unsigned char align)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		text_obj->align = align;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set align for text object %d, that does not exist!\n", id);
#endif

}

/*
 * Fixed text objects ignore the camera offset.
 */
void fbl_fix_text_to_screen(int id, bool fix)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);

		text_obj->fix_to_screen = fix;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set fix to screen for text object %d, that does not exist!\n", id);
#endif

}

/*
 * Inactive text objects dont get rendered.
 */
void fbl_set_text_active(int id, bool active)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		text_obj->active = active;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set active for text object %d, that does not exist!\n", id);
#endif

}

bool fbl_get_text_active(int id)
{

	FBL_TEXT_OBJECT *text_obj = NULL;
	DLLIST *item = get_text_item_at_id(id);

	if (item != NULL)
	{

		text_obj = ((FBL_TEXT_OBJECT *)item->Object);
		return text_obj->active;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get active for text object %d, that does not exist!\n", id);
#endif

	return false;

}

/*
 * Get total number of text objects created.
 */
int fbl_get_num_text_objects()
{

	return DLCount(fbl_text_object_list);

}

void fbl_destroy_all_text_objects()
{

	DLWalk(fbl_text_object_list, destroy_text_object_image, NULL);

	DLDestroy(&fbl_text_object_list);

	current_text_object = 0;

}

int destroy_text_object_image(int tag, void *text_obj, void *dummy)
{

	SDL_DestroyTexture(((FBL_TEXT_OBJECT *)text_obj)->text_image);

    return 0;
    
}

/*
 * Return the correct item at the id.
 * Check the sprite-version for detailed explanation.
 */
DLLIST *get_text_item_at_id(int id)
{

	DLLIST *item;

	item = DLGetFirst(fbl_text_object_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;

		item = item->Next;

	}

	return NULL;

}

/*
 * Render the text object complete with camera offset. 
 */
int render_text_object(int tag, void *text_obj, void *dummy)
{

	/* camera adjustment */

	SDL_Rect temp_rect;

	if (((FBL_TEXT_OBJECT *)text_obj)->fix_to_screen)
	{
		temp_rect.x = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.x;
		temp_rect.y = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.y;
	}
	else
	{
		temp_rect.x = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.x - fbl_camera.x;
		temp_rect.y = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.y - fbl_camera.y;
	}


	/* adjust so text gets drawn from center, also adjust if align is centered */

	switch(((FBL_TEXT_OBJECT *)text_obj)->align)
	{
		case FBL_ALIGN_LEFT:
			// do nothing
		break;
		case FBL_ALIGN_CENTER :
			temp_rect.x = temp_rect.x - ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.w / 2;
		break;
		case FBL_ALIGN_RIGHT:
			temp_rect.x = temp_rect.x - ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.w;
		break;
	}

	temp_rect.y = temp_rect.y - ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.h / 2;


	temp_rect.w = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.w;
	temp_rect.h = ((FBL_TEXT_OBJECT *)text_obj)->dest_rect.h;


	if (((FBL_TEXT_OBJECT *)text_obj)->active)
		SDL_RenderCopy(fbl_engine.renderer, ((FBL_TEXT_OBJECT *)text_obj)->text_image, NULL, &temp_rect);

	return 0;

}

/*
 * Walk through the sprite-list and call render on all active sprites.
 */
void engine_render_all_text_objects()
{

	DLWalk(fbl_text_object_list, render_text_object, NULL);

}

/* end of text.c */
