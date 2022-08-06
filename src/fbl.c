/*
 *
 * FBL ENGINE
 *
 * fbl.c
 *
 * Contains core functions such as startup code, game_loops
 * fps-stuff, timers and camera.
 *
 * Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"


/* functions only used by fbl.c */

void emscripten_loop_handler();
void normal_loop_handler();
bool should_continue_logic_loops();

#ifdef __EMSCRIPTEN__
int touch_callback(int event_type, const EmscriptenTouchEvent *touchEvent, void *userData);
#endif

FBL_ENGINE fbl_engine;
FBL_CAMERA fbl_camera;

SDL_Color fbl_clear_color;

bool fbl_quit = false;

char name_of_assets_folder[64] = "assets/";

extern int last_touch_x;
extern int last_touch_y;


int main(int argc, char **argv)
{

	fbl_start();
	engine_start();
	fbl_end();
	engine_lua_end();
	engine_free_all_mem();

	return 0;

}


/*
 * Setup a SDL Renderer and window, set fps
 * and stuff.
 */
int fbl_engine_init(int w, int h, int fps)
{

	fbl_engine.fps = fps;
	fbl_engine.current_frame = 0;
	fbl_engine.raw_frames_count = 0;
	fbl_engine.intermediate_frames = 0;
	fbl_engine.intermediate_ticks = 0;

	fbl_engine.start_time = 0;
	fbl_engine.should_start_logic_loop = true;
	fbl_engine.whole_frames_to_do = 0;


	/* start with an arbitrary normal number */

	fbl_engine.system_delay = 5;
	fbl_engine.fps_locked = true;

	fbl_camera.x = 0;
	fbl_camera.y = 0;


	// check init errors on these

	SDL_Init(SDL_INIT_EVERYTHING); // init everything?
	IMG_Init(IMG_INIT_PNG); // png rocks my socks
	TTF_Init();

	/* Initialize SDL_mixer */

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		fprintf(FBL_ERROR_OUT, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		// return 1;
	}

	//fbl_set_render_logical_size(480, 270);

	SDL_CreateWindowAndRenderer(w, h, 0, &fbl_engine.window, &fbl_engine.renderer);


	//SDL_SetRenderDrawColor(fbl_engine.renderer, 168, 230, 255, 255);

	fbl_engine.w = w;
	fbl_engine.h = h;

	if (fbl_engine.window == NULL) {
		fprintf(FBL_ERROR_OUT, "Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

    
    /* set this for now so alpha is available to primitives (as default) */

    SDL_SetRenderDrawBlendMode(fbl_engine.renderer, SDL_BLENDMODE_BLEND);


	/* register callbacks for touch devices that use browser */

#ifdef __EMSCRIPTEN__
	emscripten_set_touchstart_callback("#canvas", 0, false, touch_callback);
	emscripten_set_touchend_callback("#canvas", 0, false, touch_callback);
	emscripten_set_touchmove_callback("#canvas", 0, false, touch_callback);
	emscripten_set_touchcancel_callback("#canvas", 0, false, touch_callback);
#endif


	return 0;

}

int fbl_set_window_mode(unsigned int mode)
{

	return SDL_SetWindowFullscreen(fbl_engine.window, mode);

}

/*
 * Start engine. Call main game loop.
 */
void engine_start()
{

	fbl_engine.start_time = SDL_GetTicks();


#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(emscripten_loop_handler, -1, 1);
#else
	normal_loop_handler();
#endif

}

/*
 * Return the full filename with correct path for the current platform.
 * This is handy bc it will allow you to call the file-loading fbl-functions
 * i.e fbl_load_texture() with only the filename as argument.
 */
char *engine_get_platform_asset_path(const char *file)
{

	static char new_path[256];

#ifdef __EMSCRIPTEN__
	strcpy(new_path, PATH_TO_EMSCRIPTEN_ASSETS);
	strcat(new_path, name_of_assets_folder);
	strcat(new_path, file);
#elif __ANDROID__
	strcpy(new_path, file);
#elif _MSC_VER
	strcpy_s(new_path, 64, name_of_assets_folder);
	strcat_s(new_path, 256, file);
#else
	strcpy(new_path, name_of_assets_folder);
	strcat(new_path, file);
#endif


	return new_path;

}


/*
 * Check if logic loops should continue
 * based on fps.
 */
bool should_continue_logic_loops()
{
	if (fbl_engine.should_start_logic_loop) {
		unsigned int logic_loop_start_time = SDL_GetTicks();
		double elapsed_frames = (double)(logic_loop_start_time - fbl_engine.start_time) / 1000.0f * fbl_engine.fps;

		fbl_engine.whole_frames_to_do = (unsigned int)elapsed_frames - fbl_engine.current_frame;
	}

	if (!fbl_engine.whole_frames_to_do) {
		fbl_engine.should_start_logic_loop = true;
		return false;
	}

	fbl_engine.whole_frames_to_do--;
	fbl_engine.current_frame++;
	fbl_engine.intermediate_frames++;
	fbl_engine.should_start_logic_loop = false;

	return true;
}

/*
 * This gets called by emscripten in a nice loop same update speed as the browser updates.
 * Check documentation for emscripten_set_main_loop()
 */
void emscripten_loop_handler()
{

	//bool paused = false;

	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			fbl_quit = true;
		break;
	}

	engine_process_keyboard(&event);
	engine_process_mouse(&event);


	/* clear to selected color */

		SDL_SetRenderDrawColor(fbl_engine.renderer, fbl_clear_color.r, fbl_clear_color.g, fbl_clear_color.b, fbl_clear_color.a);
		SDL_RenderClear(fbl_engine.renderer);

		engine_render_all_sprites();
		engine_render_all_prims();
		engine_render_all_particles();
		engine_render_all_text_objects();
		engine_render_all_ui_elems();

	while (should_continue_logic_loops()) {

		engine_translate_all_phys_sprites();
		engine_translate_all_phys_prims();
		engine_animate_all_sprites();
		engine_update_all_particles();
		engine_process_all_ui_elems();
		fbl_game_loop();
		engine_lua_loop();
		engine_phys_step(fbl_engine.fps);

	}


	SDL_RenderPresent(fbl_engine.renderer);

	fbl_engine.raw_frames_count++;

}

/*
 * Standard game loop for all other platforms.
 */
void normal_loop_handler()
{

	//bool render = true;
	//bool paused = false;

	SDL_Event event;

	while (!fbl_quit)
	{

		SDL_PollEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
				fbl_quit = true;
			break;

		}


		engine_process_keyboard(&event);
		engine_process_mouse(&event);


		/* clear to selected color */

		SDL_SetRenderDrawColor(fbl_engine.renderer, fbl_clear_color.r, fbl_clear_color.g, fbl_clear_color.b, fbl_clear_color.a);
		SDL_RenderClear(fbl_engine.renderer);

		engine_render_all_sprites();
		engine_render_all_prims();
		engine_render_all_particles();
		engine_render_all_text_objects();
		engine_render_all_ui_elems();


		if (fbl_engine.fps_locked)
		{
			while (should_continue_logic_loops())
			{
				fbl_submit_task_tp(engine_translate_all_phys_sprites);
				fbl_submit_task_tp(engine_translate_all_phys_prims);

				fbl_submit_task_tp(engine_animate_all_sprites); // dependent on fb_engine.current frame
				fbl_submit_task_tp(engine_update_all_particles);
				fbl_submit_task_tp(engine_process_all_ui_elems);

				fbl_game_loop();
				engine_lua_loop();
				engine_phys_step(fbl_engine.fps);
				//render = true;
			}
		}
		else
		{
			fbl_submit_task_tp(engine_translate_all_phys_sprites);
			fbl_submit_task_tp(engine_translate_all_phys_prims);

			fbl_submit_task_tp(engine_animate_all_sprites);
			fbl_submit_task_tp(engine_update_all_particles);
			fbl_submit_task_tp(engine_process_all_ui_elems);

			fbl_game_loop();
			engine_lua_loop();
			engine_phys_step(fbl_engine.fps);
			//render = true;
		}

		/*
		static float x = 2.0;
		static float y = 2.0;
		SDL_RenderSetScale(fbl_engine.renderer, x, y); // this function is very cheap (can use every frame)
		x -= 0.001;
		y -= 0.001;
		if (x < 1.0) x = 1.0;
		if (y < 1.0) y = 1.0;


		Also test SDL_RenderSetLogicalSize()

		*/

		//if(render)
			SDL_RenderPresent(fbl_engine.renderer);

		//render = false;


		/* this could be on when framerate is unlocked to show real fps */

		//if(!fbl_engine.fps_locked) fbl_engine.current_frame++; // if this is commented out animation won't work in unlocked framerate

		if(fbl_engine.system_delay > 0)
			SDL_Delay(fbl_engine.system_delay);

		fbl_engine.raw_frames_count++;

		/* set a point every something frames and check fps from that point 

		if (fbl_engine.current_frame % 200 == 0)
		{
			printf("engine_fps : %d\n", fbl_engine.current_frame);
			printf("raw_fps : %d\n", fbl_engine.raw_frames_count);


			fbl_engine.intermediate_frames = 0;
			fbl_engine.intermediate_ticks = SDL_GetTicks();

			printf("delay: %d\n", fbl_engine.system_delay);

		} */


	}

}


/*
 * Set the name of the folder from which you want to load assets (default is "assets").
 */
void fbl_set_assets_folder_name(const char *name)
{

#ifdef _MSC_VER
	strcpy_s(name_of_assets_folder, 64, name);
#else
	strcpy(name_of_assets_folder, name);
#endif

}

/*
 * Set the clear color for backbuffer.
 */
void fbl_set_clear_color(int r, int g, int b, int a)
{

	fbl_clear_color.r = r;
	fbl_clear_color.g = g;
	fbl_clear_color.b = b;
	fbl_clear_color.a = a;

}

/*
 * Self explanatory stuff
 */

void fbl_set_screen_wh(int w, int h)
{

	SDL_SetWindowSize(fbl_engine.window, w, h);

	fbl_engine.w = w;
	fbl_engine.h = h;

}

int fbl_get_screen_w()
{

	return fbl_engine.w;

}

int fbl_get_screen_h()
{

	return fbl_engine.h;

}

/* For zooming and stuff */

int fbl_set_render_scale(float scale_x, float scale_y)
{

	return SDL_RenderSetScale(fbl_engine.renderer, scale_x, scale_y);

}

/* Set the drawing area */

int fbl_set_viewport(int x, int y, int w, int h)
{

	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return SDL_RenderSetViewport(fbl_engine.renderer, &rect);

}

/* Set a device independent resolution for rendering */

int fbl_set_render_logical_size(int x, int y)
{

	return SDL_RenderSetLogicalSize(fbl_engine.renderer, x, y);

}


void fbl_set_fps(int fps)
{

	fbl_engine.fps = fps;

}

int fbl_get_fps()
{

	/*
	
		this function calculates the average framerate
		using total frames rendered divided by total
		seconds in use. It gets really wierd if you
		for example add alot of sprites along the way.
		what you want is just check the last second of everything.
		do it!
	
	*/

	int screen_fps = 0;
	unsigned int current_time = SDL_GetTicks();

	current_time = current_time - fbl_engine.start_time; // - intermed

	/* avoid division by 0! */

	if (current_time > 0)
	 screen_fps = ((fbl_engine.current_frame * 1000) / current_time); // current_frame instead of  intermed

	return screen_fps;

}



/*
 * return total amount of rendered frames since program start.
 */

unsigned int fbl_get_raw_frames_count()
{

	return fbl_engine.raw_frames_count;

}


/*
 * Give up this many ms to the OS every frame.
 */

void fbl_set_system_delay_ms(int delay)
{


	/* limit to some resonable values */

	if (delay > 100) delay = 100;
	if (delay < 1) delay = 0;

	fbl_engine.system_delay = delay;

}

int fbl_get_system_delay_ms()
{
	/*

	// dynamically change the sdl_delay but do this later maybe..

	int fps = fbl_get_fps();
	int target_fps = fbl_engine.fps - 3;

	if (fps < target_fps) fbl_engine.system_delay--;
	else fbl_engine.system_delay++;

	if (fbl_engine.system_delay > 15) fbl_engine.system_delay = 15;
	if (fbl_engine.system_delay < 2) fbl_engine.system_delay = 0;

	*/

	return fbl_engine.system_delay;

}

void fbl_set_fps_locked(bool locked)
{

	fbl_engine.fps_locked = locked;

}

bool fbl_get_fps_locked()
{

	return fbl_engine.fps_locked;

}

/*
 * Get number of ms since SDL was initialized
 */
unsigned int fbl_timer_get_ticks()
{

	return SDL_GetTicks();

}


/*
 * Camera functions.
 */
void fbl_set_camera_xy(int x, int y)
{

	fbl_camera.x = x;
	fbl_camera.y = y;

}

int fbl_get_camera_x(void)
{

	return fbl_camera.x;

}

int fbl_get_camera_y(void)
{

	return fbl_camera.y;

}


/*
* Set window title.
*/
void fbl_set_window_title(const char *title)
{

	SDL_SetWindowTitle(fbl_engine.window, title);

}


/*
 * Get FBL version
 */
const char *fbl_get_version()
{

	return FBL_VERSION;

}


/*
 * Get FBL platform
 */
const char *fbl_get_platform()
{

	return SDL_GetPlatform();

}

/*
 * Get FBL renderer
 */
const char *fbl_get_renderer()
{


	SDL_RendererInfo info = {0};


	if (SDL_GetRendererInfo(fbl_engine.renderer, &info) > -1)
	{

		//printf("\nw: %d, h %d\n", info.max_texture_width, info.max_texture_height);

		return info.name;
	}

	return "unknown renderer";

}

/*
 * Free all memory and shut everything down.
 */
void engine_free_all_mem()
{

	fbl_destroy_texture();
	fbl_destroy_all_sprites();
	fbl_destroy_all_prims();
	fbl_destroy_all_text_objects();
	fbl_destroy_ttf_font();
	fbl_destroy_ui_texture();
	fbl_destroy_all_ui_elems();
	fbl_destroy_all_emitters();
	fbl_destroy_all_sounds();
	fbl_destroy_music();
	fbl_pathf_shutdown();
	fbl_lua_shutdown();
	fbl_phys_shutdown();
	SDL_DestroyRenderer(fbl_engine.renderer);
	SDL_DestroyWindow(fbl_engine.window);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

}


/*
 * Break the game loop and quit program.
 */
void fbl_engine_quit()
{

	fbl_quit = true;

}

/* touch callback function for touch devices that run in browser, thanks to App Game Kit for this (source on Github) */

#ifdef __EMSCRIPTEN__
int touch_callback(int event_type, const EmscriptenTouchEvent *touchEvent, void *userData)
{

	int i;
	int X, Y;
	int isFullscreen;
	float scale;
	SDL_Event sdlevent;

	switch (event_type)
	{
	case EMSCRIPTEN_EVENT_TOUCHSTART:
	{
		for (i = 0; i < touchEvent->numTouches; i++)
		{
			if (touchEvent->touches[i].isChanged)
			{
				isFullscreen = EM_ASM_INT_V({ return window.isFullScreen ? 1 : 0 });
				X = touchEvent->touches[i].canvasX;
				Y = touchEvent->touches[i].canvasY;
				if (isFullscreen)
				{
					scale = EM_ASM_DOUBLE_V({ return window.devicePixelRatio });
					X = (int)X*scale;
					Y = (int)Y*scale;
				}

				sdlevent.type = SDL_MOUSEBUTTONDOWN;
				sdlevent.button.button = SDL_BUTTON_LEFT;
				last_touch_x = X;
				last_touch_y = Y;
				SDL_PushEvent(&sdlevent);
			}
		}
		break;
	}
	case EMSCRIPTEN_EVENT_TOUCHEND:
	{
		for (i = 0; i < touchEvent->numTouches; i++)
		{
			if (touchEvent->touches[i].isChanged)
			{
				isFullscreen = EM_ASM_INT_V({ return window.isFullScreen ? 1 : 0 });
				X = touchEvent->touches[i].canvasX;
				Y = touchEvent->touches[i].canvasY;
				if (isFullscreen)
				{
					scale = EM_ASM_DOUBLE_V({ return window.devicePixelRatio });
					X = (int)X*scale;
					Y = (int)Y*scale;
				}

				sdlevent.type = SDL_MOUSEBUTTONUP;
				sdlevent.button.button = SDL_BUTTON_LEFT;
				last_touch_x = X;
				last_touch_y = Y;
				SDL_PushEvent(&sdlevent);
			}
		}
		break;
	}
	case EMSCRIPTEN_EVENT_TOUCHMOVE:
	{
		for (i = 0; i < touchEvent->numTouches; i++)
		{
			if (touchEvent->touches[i].isChanged)
			{
				isFullscreen = EM_ASM_INT_V({ return window.isFullScreen ? 1 : 0 });
				X = touchEvent->touches[i].canvasX;
				Y = touchEvent->touches[i].canvasY;
				if (isFullscreen)
				{
					scale = EM_ASM_DOUBLE_V({ return window.devicePixelRatio });
					X = (int)X*scale;
					Y = (int)Y*scale;
				}

				sdlevent.type = SDL_MOUSEMOTION;
				sdlevent.button.button = SDL_BUTTON_LEFT;
				last_touch_x = X;
				last_touch_y = Y;
				SDL_PushEvent(&sdlevent);
			}
		}
		break;
	}
	case EMSCRIPTEN_EVENT_TOUCHCANCEL:
	{
		for (int i = 0; i < touchEvent->numTouches; i++)
		{
			if (touchEvent->touches[i].isChanged)
			{
				isFullscreen = EM_ASM_INT_V({ return window.isFullScreen ? 1 : 0 });
				X = touchEvent->touches[i].canvasX;
				Y = touchEvent->touches[i].canvasY;
				if (isFullscreen)
				{
					scale = EM_ASM_DOUBLE_V({ return window.devicePixelRatio });
					X = (int)X*scale;
					Y = (int)Y*scale;
				}

				sdlevent.type = SDL_MOUSEBUTTONUP;
				sdlevent.button.button = SDL_BUTTON_LEFT;
				last_touch_x = X;
				last_touch_y = Y;
				SDL_PushEvent(&sdlevent);
			}
		}
		break;
	}
	default: return 0;
	}

	return 1;
}
#endif

/* end of fbl.c */
