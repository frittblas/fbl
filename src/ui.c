/*
 *
 *	FBL ENGINE
 *
 *	ui.c
 *
 *	Contains user interface related functions like buttons and stuff.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* functions only used by ui.c */

DLLIST *get_ui_item_at_id(int id);
int	render_ui_elem(int tag, void *ui_elem, void *dummy);
int	process_ui_elem(int tag, void *ui_elem, void *dummy);


extern FBL_ENGINE fbl_engine;
extern FBL_CAMERA fbl_camera;

SDL_Texture *fbl_ui_texture = NULL;
DLLIST *fbl_ui_list = NULL;

unsigned int current_ui_elem = 0;

/*
 * load a texture to memory to be used later by ui elements
 * and stuff.
 */
int fbl_load_ui_texture(const char *img_file)
{

    char* new_path = engine_get_platform_asset_path(img_file);

	SDL_Surface *surface = IMG_Load(new_path);

	if (!surface)
	{
		fprintf(FBL_ERROR_OUT, "IMG_Load: %s\n", IMG_GetError());
		return -1;
	}

	fbl_ui_texture = SDL_CreateTextureFromSurface(fbl_engine.renderer, surface);

	if (!fbl_ui_texture)
	{
		fprintf(FBL_ERROR_OUT, "UI texture could not be created: %s\n", SDL_GetError());
		return -1;
	}

	SDL_FreeSurface(surface);

	return 0;

}

void fbl_destroy_ui_texture()
{

    if (!fbl_ui_texture) {
        SDL_DestroyTexture(fbl_ui_texture);
        fbl_ui_texture = NULL;
    }

}


/*
 * Create ui element by adding a type, a SDL_Rect which is
 * a location on the atlas-texture, and a dest - SDL_Rect
 * which is a position on the screen. Also takes a function pointer
 * for the funcion to be called when the element is used.
 */
int fbl_create_ui_elem(uint8_t type, int x, int y, int w, int h, int(*func)(int, int))
{

	FBL_UI_ELEM *fbl_ui_elem = NULL;

	if (!fbl_ui_texture)
	{
		fprintf(FBL_ERROR_OUT, "Load ui image sheet before create ui element!\n");
		return -1;
	}

	fbl_ui_elem = malloc(sizeof(FBL_UI_ELEM));

	if (!fbl_ui_elem)
	{
		fprintf(FBL_ERROR_OUT, "No memory for ui element!\n");
		return -1;
	}


    fbl_ui_elem->type = type;
    
	fbl_ui_elem->source_rect.x = x;
	fbl_ui_elem->source_rect.y = y;
	fbl_ui_elem->source_rect.w = w;
	fbl_ui_elem->source_rect.h = h;

	fbl_ui_elem->dest_rect.x = 0;
	fbl_ui_elem->dest_rect.y = 0;
	fbl_ui_elem->dest_rect.w = w;
	fbl_ui_elem->dest_rect.h = h;

    fbl_ui_elem->color.a = 255;	/* start as fully opaque */
    fbl_ui_elem->color.r = 255;
    fbl_ui_elem->color.g = 255;
    fbl_ui_elem->color.b = 255;

    fbl_ui_elem->orig_x = x;
    fbl_ui_elem->value = 0;
	fbl_ui_elem->active = true;
    fbl_ui_elem->pressed = false;
    fbl_ui_elem->access = 0;
    fbl_ui_elem->access_delay = 30;     /* default to half aa second */

	/* set function */

	fbl_ui_elem->func = func;


	/* if it's the first ui element created start a new list, otherwise append */

	if (current_ui_elem == 0)
	{

		fbl_ui_list = DLCreate(current_ui_elem, fbl_ui_elem, sizeof(FBL_UI_ELEM));

	}
	else
	{

		DLAppend(&fbl_ui_list, current_ui_elem, fbl_ui_elem, sizeof(FBL_UI_ELEM));

	}


	free(fbl_ui_elem);

	current_ui_elem++;

	return current_ui_elem - 1;

}

/*
 * Delete a ui element by only removing the item on the list
 */
void fbl_delete_ui_elem(int id)
{

	DLLIST *item = get_ui_item_at_id(id);

	if (item != NULL)
	{

		/* security so that the first element can't be deleted, will crash! */

		if(item != DLGetFirst(fbl_ui_list))	//  !!!!! is this perfect?  before: if(id > 0)
		{
			DLDelete(item);
		}
		else
			fbl_set_ui_elem_active(id, false);  // a kinda hacky way to avoid crash when deleting head node...
			//fprintf(FBL_ERROR_OUT, "Will not delete the first ui element in list! Use destroy_all..\n");

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to delete ui element %d, that does not exist!\n", id);
#endif

}


/*
 * Self explanatory stuff. Just get and set parameters from the ui elem-struct.
 */
void fbl_set_ui_elem_xy(int id, int x, int y)
{
	
	FBL_UI_ELEM *ui_elem = NULL;
	DLLIST *item = get_ui_item_at_id(id);

	if(item != NULL)
	{
	
		ui_elem = ((FBL_UI_ELEM *)item->Object);
	
		ui_elem->dest_rect.x = x;
		ui_elem->dest_rect.y = y;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set xy for ui element %d, that does not exist!\n", id);
#endif

}

int fbl_get_ui_elem_x(int id)
{

	FBL_UI_ELEM *ui_elem = NULL;
	DLLIST *item = get_ui_item_at_id(id);

	if (item != NULL)
	{
		ui_elem = ((FBL_UI_ELEM *)item->Object);
		return ui_elem->dest_rect.x;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get x for ui element %d, that does not exist!\n", id);
#endif

	return 0;

}

int fbl_get_ui_elem_y(int id)
{

	FBL_UI_ELEM *ui_elem = NULL;
	DLLIST *item = get_ui_item_at_id(id);

	if (item != NULL)
	{
		ui_elem = ((FBL_UI_ELEM *)item->Object);
		return ui_elem->dest_rect.y;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get y for ui element %d, that does not exist!\n", id);
#endif

	return 0;

}

void fbl_set_ui_elem_color(int id, uint8_t r, uint8_t g, uint8_t b)
{

    FBL_UI_ELEM* ui_elem = NULL;
    DLLIST* item = get_ui_item_at_id(id);

    if (item != NULL)
    {

        ui_elem = ((FBL_UI_ELEM*)item->Object);

        ui_elem->color.r = r;
        ui_elem->color.g = g;
        ui_elem->color.b = b;

    }
#ifdef FBL_DEBUG
    else fprintf(FBL_ERROR_OUT, "Tried to set color for ui element %d, that does not exist!\n", id);
#endif

}

/*
 * Get the value param from the ui element. Gets set by ui elements only.
 * This should primarily be used when checking buttons from lua.
 */
int fbl_get_ui_elem_val(int id)
{

    FBL_UI_ELEM *ui_elem = NULL;
    DLLIST *item = get_ui_item_at_id(id);
    
    if (item != NULL)
    {
        ui_elem = ((FBL_UI_ELEM *)item->Object);
        return ui_elem->value;
    }
#ifdef FBL_DEBUG
    else fprintf(FBL_ERROR_OUT, "Tried to get value for ui element %d, that does not exist!\n", id);
#endif
    
    return 0;

}

/*
 * Set the value from code. Simulating a click kinda.
 * Set values 1 or 0.
 */
void fbl_set_ui_elem_val(int id, int value)
{

    FBL_UI_ELEM* ui_elem = NULL;
    DLLIST* item = get_ui_item_at_id(id);

    if (item != NULL)
    {
        ui_elem = ((FBL_UI_ELEM*)item->Object);
        
        if (value == 0) {
            ui_elem->source_rect.x = ui_elem->orig_x;
            ui_elem->value = value;
        }
        else if (value == 1) {
            ui_elem->source_rect.x = ui_elem->orig_x + ui_elem->source_rect.w * 2;
            ui_elem->value = value;
        }

    }
#ifdef FBL_DEBUG
    else fprintf(FBL_ERROR_OUT, "Tried to set value for ui element %d, that does not exist!\n", id);
#endif

}

/*
 *  Set the delay for _INTERVAL ui buttons
 */
void fbl_set_ui_elem_access(int id, int frames)
{

    FBL_UI_ELEM* ui_elem = NULL;
    DLLIST* item = get_ui_item_at_id(id);

    if (item != NULL)
    {

        ui_elem = ((FBL_UI_ELEM*)item->Object);

        ui_elem->access_delay = frames;

    }
#ifdef FBL_DEBUG
    else fprintf(FBL_ERROR_OUT, "Tried to set access for ui element %d, that does not exist!\n", id);
#endif

}

/*
 *  Set the access time left for _INTERVAL ui buttons
 */
void fbl_set_ui_elem_access_left(int id, int frames)
{

    FBL_UI_ELEM* ui_elem = NULL;
    DLLIST* item = get_ui_item_at_id(id);

    if (item != NULL)
    {

        ui_elem = ((FBL_UI_ELEM*)item->Object);

        ui_elem->access = frames;

    }
#ifdef FBL_DEBUG
    else fprintf(FBL_ERROR_OUT, "Tried to set access left for ui element %d, that does not exist!\n", id);
#endif

}

/*
 * If ui element is inactive it's not rendered and not checked for interactions.
 */
void fbl_set_ui_elem_active(int id, bool active)
{

	FBL_UI_ELEM *ui_elem = NULL;
	DLLIST *item = get_ui_item_at_id(id);

	if (item != NULL)
	{

		ui_elem = ((FBL_UI_ELEM *)item->Object);

		ui_elem->active = active;

	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to set active for ui element %d, that does not exist!\n", id);
#endif

}

bool fbl_get_ui_elem_active(int id)
{

	FBL_UI_ELEM *ui_elem = NULL;
	DLLIST *item = get_ui_item_at_id(id);

	if (item != NULL)
	{
		ui_elem = ((FBL_UI_ELEM *)item->Object);
		return ui_elem->active;
	}
#ifdef FBL_DEBUG
	else fprintf(FBL_ERROR_OUT, "Tried to get active for ui element %d, that does not exist!\n", id);
#endif

	return false;

}



/*
 * Return total number of ui elements created.
 */
int fbl_get_num_ui_elems()
{

	return DLCount(fbl_ui_list);

}


void fbl_destroy_all_ui_elems()
{

	DLDestroy(&fbl_ui_list);

	current_ui_elem = 0;

}


/*
 * Return the correct item at the id.
 * Check sprite-version for details.
 *
 * NOTE: if this gets LAST and item->prev the program acts really weird!!!
 * (meaning searches from the end of the list to the start)
 * really weird! clicks doesnt seem to register etc check out!
 * No reason to traverse the list this way but still weird.
 *
 */
DLLIST *get_ui_item_at_id(int id)
{

	DLLIST *item;

	item = DLGetFirst(fbl_ui_list);


	while (item != NULL)
	{

		if (item->Tag == id)
			return item;

		item = item->Next;

	}

	return NULL;

}


/*
 * Render a ui element.
 */
int render_ui_elem(int tag, void *ui_elem, void *dummy)
{

	/* for camera adjustment and drawing from center */

	SDL_Rect temp_rect;

    FBL_UI_ELEM* ui = (FBL_UI_ELEM*)ui_elem;	// ui is now the current ui-element

	if(ui->active)
    {
        
        /* ui elements are always fixed to screen */
    
        temp_rect.x = ui->dest_rect.x;
        temp_rect.y = ui->dest_rect.y;


        /* adjust so ui element gets drawn from the center */

        temp_rect.x = temp_rect.x - ui->dest_rect.w / 2;
        temp_rect.y = temp_rect.y - ui->dest_rect.h / 2;


        temp_rect.w = ui->dest_rect.w;
        temp_rect.h = ui->dest_rect.h;


        /* set color */

        SDL_SetTextureColorMod(fbl_ui_texture, ui->color.r, ui->color.g, ui->color.b);

        /* render */
        
        SDL_RenderCopy(fbl_engine.renderer, fbl_ui_texture, &ui->source_rect, &temp_rect);

	}

	return 0;

}

/*
 * Walk through the ui-list and render all active elements.
 */
void engine_render_all_ui_elems()
{

	DLWalk(fbl_ui_list, render_ui_elem, NULL);

}


/*
 * Process what should happen to every ui element every frame.
 * Check mouse over, then check mouse click, then switch images
 * by moving the source rect then call function pointer.
 */
int process_ui_elem(int tag, void *ui_elem, void *dummy)
{

    
    SDL_Point point;
    SDL_Rect  temp_rect;

    FBL_UI_ELEM* ui = (FBL_UI_ELEM*)ui_elem;	// ui is now the current ui-element
    
    if(ui->active)
    {
    
        /* get mouse position */
    
        //SDL_GetMouseState(&point.x, &point.y);

		point.x = fbl_get_mouse_logical_x();    // NOTE: this seems to work fine but it used to, too :) (using getMouseState())
		point.y = fbl_get_mouse_logical_y();
    
    
        /* adjust for ui element gets drawn from the center */
    
        temp_rect.x = ui->dest_rect.x;
        temp_rect.y = ui->dest_rect.y;
        temp_rect.x = temp_rect.x - ui->dest_rect.w / 2;
        temp_rect.y = temp_rect.y - ui->dest_rect.h / 2;
        temp_rect.w = ui->dest_rect.w;
        temp_rect.h = ui->dest_rect.h;
    
    
        switch(ui->type)
        {
        
            /* hold is used for buttons that needs to be held down (like a throttle) */
                
            case FBL_UI_BUTTON_HOLD :
                 
                
                ui->value = 0;
                
                /* check "mouse over" */
    
                if(SDL_PointInRect(&point, &temp_rect))
                {
    
                    ui->source_rect.x = ui->orig_x + ui->source_rect.w;
        
                    if(fbl_get_mouse_click(FBLMB_LEFT))
                    {
        
                        /* call the function as long as button is held! */
                        
                        if(ui->func != NULL)
                            ui->func(1, 2);

                        ui->source_rect.x = ui->orig_x + ui->source_rect.w * 2;
                        
                        ui->value = 1;
            
                    }
                    else ui->source_rect.x = ui->orig_x + ui->source_rect.w;
                    
                    
                }
                else ui->source_rect.x = ui->orig_x;
                
            break;

            /* hold interval turns value on, off, on off with a small delay (good for touch) */

            case FBL_UI_BUTTON_INTERVAL:


                ui->value = 0;

                /* check "mouse over" */

                if (SDL_PointInRect(&point, &temp_rect))
                {

                    ui->source_rect.x = ui->orig_x + ui->source_rect.w;

                    if (fbl_get_mouse_click(FBLMB_LEFT) && ui->access == 0)
                    {

                        /* call the function once every nth frame */

                        if (ui->func != NULL)
                            ui->func(1, 2);

                        ui->source_rect.x = ui->orig_x + ui->source_rect.w * 2;

                        ui->value = 1;

                        ui->access = ui->access_delay;

                    }
                    else ui->source_rect.x = ui->orig_x + ui->source_rect.w;


                }
                else ui->source_rect.x = ui->orig_x;

                ui->access--;
                if (ui->access < 0) ui->access = 0;

                break;
                
            /* click is used for buttons that needs to be one clicked at a time (on the mouse release) */
                
            case FBL_UI_BUTTON_CLICK :
                

                ui->value = 0;
                
                /* check "mouse over" */
                
                if(SDL_PointInRect(&point, &temp_rect))
                {
                    
                    ui->source_rect.x = ui->orig_x + ui->source_rect.w;
                    
                    
                    if(fbl_get_mouse_click(FBLMB_LEFT))
                    {
                        
                        ui->source_rect.x = ui->orig_x + ui->source_rect.w * 2;
                        
                        ui->pressed = true;
                    
                    }
                    else ui->source_rect.x = ui->orig_x + ui->source_rect.w;

                    if(fbl_get_mouse_release(FBLMB_LEFT) && ui->pressed)
                    {
                        
                        /* call the function once! */
                        
                        if(ui->func != NULL)
                            ui->func(1, 2);
                        
                        ui->pressed = false;
                        
                        ((FBL_UI_ELEM*)ui_elem)->value = 1;
                        
                    }

                    
                    
                }
                else ui->source_rect.x = ui->orig_x;
                
            break;
                
                
            /* checkbox is a regular checkbox (like button_click but switches between check and uncheck images) */
                
            case FBL_UI_CHECKBOX :
                
                /* check "mouse over" */
                
                if(SDL_PointInRect(&point, &temp_rect))
                {
                    
                    if(ui->value == 0)
                        ui->source_rect.x = ui->orig_x + ui->source_rect.w;
                    if(ui->value == 1)
                        ui->source_rect.x = ui->orig_x + ui->source_rect.w * 3;
                    
                    
                    if(fbl_get_mouse_click(FBLMB_LEFT))
                    {
                        
                        ui->pressed = true;
                        
                    }
                    
                    if(fbl_get_mouse_release(FBLMB_LEFT) && ui->pressed)
                    {
                        
                        ui->pressed = false;
                        
                        
                        /* toggle */
                        
                        ui->value = ui->value ? 0 : 1;
                     
                        
                        /* call the function once! */
                        
                        if(ui->func != NULL)
                            ui->func(ui->pressed, ui->value);
                        
                        
                    }
                    
                    
                    
                }
                else if(ui->value == 0) ui->source_rect.x = ui->orig_x;
                else if(ui->value == 1) ui->source_rect.x = ui->orig_x + ui->source_rect.w * 2;
                
            break;

            /* checkbox interval turns on/off with n frames delay */

            case FBL_UI_CHECKBOX_INTERVAL:

                /* check "mouse over" */

                if (SDL_PointInRect(&point, &temp_rect))
                {

                    if (ui->value == 0)
                        ui->source_rect.x = ui->orig_x + ui->source_rect.w;
                    if (ui->value == 1)
                        ui->source_rect.x = ui->orig_x + ui->source_rect.w * 3;


                    if (fbl_get_mouse_click(FBLMB_LEFT) && ui->access == 0)
                    {

                        /* toggle */

                        ui->value = ui->value ? 0 : 1;


                        /* call the function once with 30 frames delay */

                        if (ui->func != NULL)
                            ui->func(ui->pressed, ui->value);

                        ui->access = ui->access_delay;

                    }



                }
                else if (ui->value == 0) ui->source_rect.x = ui->orig_x;
                else if (ui->value == 1) ui->source_rect.x = ui->orig_x + ui->source_rect.w * 2;

                ui->access--;
                if (ui->access < 0) ui->access = 0;

                break;
                
                
        }
    

    }
        
    
    return 0;
    

}

/*
 * Walk through the ui-list and process all active elements.
 */
void engine_process_all_ui_elems()
{
    
    DLWalk(fbl_ui_list, process_ui_elem, NULL);
    
}

/* end of ui.c */
