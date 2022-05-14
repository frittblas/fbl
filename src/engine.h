/*
 *
 *	FBL ENGINE
 *
 *	engine.h
 *	
 *	Include this if you want access to the renderer and stuff.
 *
 *	
 *	Hans Strömquist 2017 - 2022
 *
 */

#ifndef ENGINE_H__
#define ENGINE_H__

#ifdef __EMSCRIPTEN__
//#include "../dependencies/emscripten/chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include "../dependencies/common/chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#else
#include "../dependencies/common/chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#endif

#include "a_star/a_star.h"
#include "dl_list/dllist.h"

#include "fbl.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

/* set if include Lua in compile */

#define FBL_INCLUDE_LUA

/* set where error messages will go */

#define FBL_ERROR_OUT stdout


/* fbl version */

#define FBL_VERSION "0.9.8"

/* engine struct */


typedef struct
{

	Uint32 fps;
	Uint32 current_frame;
	Uint32 raw_frames_count;
	Uint32 intermediate_frames;
	Uint32 intermediate_ticks;
	SDL_Window   *window;
	SDL_Renderer *renderer;
	Uint32 w;
	Uint32 h;

	Uint32 start_time;
	bool should_start_logic_loop;
	Uint32 whole_frames_to_do;
	Uint32 system_delay;
	bool fps_locked;

}FBL_ENGINE;


/* camera struct */

typedef struct
{

	int x;
	int y;

}FBL_CAMERA;


/* sprite struct */

typedef struct
{

	SDL_Rect source_rect;
	SDL_Rect dest_rect;

	int radius;

	double angle;
	float scale;

	bool is_light;
	Uint8 blendmode;
	SDL_Color color;

	SDL_RendererFlip flip;

	/* for sorting sprites in layers */

	Uint8 layer;

	bool fix_to_screen;
	bool active;

	/* for animation */

	bool animated;
	Uint8 animation_frames;
	Uint8 current_frame;
	Uint8 animation_speed;
	bool anim_loop;

	/* for physics */

	bool physics_on;

    //unsigned char shape; /* FBL_RECT or FBL_CIRCLE */
    
	cpBody  *phys_body;
	cpShape *phys_shape;


}FBL_SPRITE;


/* Used to set "time of day" or various effects */

typedef struct
{

	bool on;
	Uint8 r, g, b;

}FBL_LIGHTING_TINT;


/* point and shape structs for the poly primitives */

typedef struct
{

	double x, y;

}A_POINT;

typedef struct
{

	char	num_points;
	A_POINT point[6];		/* hexagon is max */

}A_SHAPE;

/* primitives struct */

typedef struct
{

	/* type could be: point, line, tri, rect, penta, hexa, circle and ray */

	unsigned char type;

	SDL_Rect dest_rect;

	int radius;

	A_POINT	loc;
	A_SHAPE	shape;
	A_SHAPE	scr;

	double angle;
	float scale;

	SDL_Color color;

	bool thick;
	bool fill;
	bool fix_to_screen;
	bool active;

	/* for physics */

	bool physics_on;

	cpBody  *phys_body;
	cpShape *phys_shape;

	/* for raycast*/

	cpSegmentQueryInfo segInfo;


}FBL_PRIM;


/* text struct */

typedef struct
{

	SDL_Rect dest_rect;

	float scale;
	SDL_Texture *text_image;
	Uint8 align;

	bool fix_to_screen;
	bool active;


}FBL_TEXT_OBJECT;


/* ui element struct */

typedef struct
{

	/* could be button, checkbox, slider etc */

	unsigned char type;

	SDL_Rect source_rect;
	SDL_Rect dest_rect;
	
	int orig_x; /* original x position */
	int value;  /* just a value : D */
    bool pressed; /* check to see left click before release (mouse) */
	bool active;

	/* the function to be called when a ui element is used */

	int(*func)(int, int);

}FBL_UI_ELEM;


/* sound struct */

typedef struct {

	Mix_Chunk *sample;	/* The sample to play */
	int channel;		/* Channel used by the sample */

}FBL_SOUND;


/* one particle */

typedef struct {
	
	float x;
	float y;
	float vel_x;
	float vel_y;

	float scale;

	SDL_Color color;
	Uint8 alpha;

	int life;

}PARTICLE;


/* particle emitter struct */

typedef struct
{

	Uint8 type;

	PARTICLE *particle;
	SDL_Rect spawn_area;

	float scale_start;
	float scale_end;
	float vel_x_start, vel_y_start;	/* starting direction and speed */
	float vel_x_end, vel_y_end;
	SDL_Color color_start;
	SDL_Color color_end;

	int num_particles;
	int life_total;

	Uint8 emit_rate;	/* how many frames per particle? 1 = emit every frame, 7 = emits every 7th frame */
	Uint8 density;		/* how many particles emits every time */
	float rand_amount;	/* how much of random movement */
	bool fix_to_screen;
	bool active;

}FBL_PARTICLE_EMITTER;


/* threadpool struct */

typedef struct
{

	//void (*taskFunction)(int, int);
	void (*task_func)();
	//int arg1, arg2;

}FBL_TP_TASK;


/* from fbl.c */

void  engine_start(void);
char *engine_get_platform_asset_path(const char *file);
void  engine_free_all_mem(void);


/* from sprite.c */

void engine_translate_all_phys_sprites(void);
void engine_animate_all_sprites(void);
void engine_render_all_sprites(void);


/* from primitives.c */

void engine_translate_all_phys_prims(void);
void engine_render_all_prims(void);


/* from text.c */

void engine_render_all_text_objects(void);


/* from ui.c */

void engine_render_all_ui_elems(void);
void engine_process_all_ui_elems(void);


/* from input.c */

void engine_process_keyboard(SDL_Event *event);
void engine_process_mouse(SDL_Event *event);


/* from lua.c */

void engine_lua_start(void);
void engine_lua_loop(void);
void engine_lua_end(void);


/* from physics.c */

void     engine_phys_step(int fps);
cpBody  *engine_phys_add_body(int type, float m, int w, int h, int r, int body_type, bool rotate);
cpShape *engine_phys_add_shape(cpBody *body, int type, int x, int y, int w, int h, int r);
void	 engine_phys_remove_shape(cpShape *shape);
void	 engine_phys_remove_body(cpBody *body);
cpShape* engine_ray_collided(SDL_Rect *dest_rect, int radius, cpSegmentQueryInfo *segInfo);


/* from particles.c */

void engine_update_all_particles(void);
void engine_render_all_particles(void);


#endif

/* end of engine.h */
