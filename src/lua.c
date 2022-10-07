/*
 *
 *	FBL ENGINE
 *
 *	lua.c
 *
 *	Contains lua related functions including functionality
 *  	to call all  fbl_* functions from lua.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"

#ifdef FBL_INCLUDE_LUA

#include "../dependencies/common/lua-5.4.1/include/lua.h"
#include "../dependencies/common/lua-5.4.1/include/lualib.h"
#include "../dependencies/common/lua-5.4.1/include/lauxlib.h"

/* only used by lua.c */

void  register_fbl_functions_to_lua();
char *read_file_to_buf(const char* filename);


/* fbl.c */

int lua_engine_init(lua_State *lua_env);
int lua_set_window_mode(lua_State *lua_env);
int lua_set_assets_folder_name(lua_State* lua_env);
int lua_fbl_log(lua_State* lua_env);
int lua_set_clear_color(lua_State *lua_env);
int lua_set_screen_wh(lua_State* lua_env);
int lua_get_screen_w(lua_State *lua_env);
int lua_get_screen_h(lua_State *lua_env);
int lua_set_render_scale(lua_State* lua_env);
int lua_set_viewport(lua_State* lua_env);
int lua_set_render_logical_size(lua_State* lua_env);
int lua_set_fps(lua_State *lua_env);
int lua_get_fps(lua_State *lua_env);
int lua_get_raw_frames_count(lua_State *lua_env);
int lua_set_system_delay_ms(lua_State *lua_env);
int lua_get_system_delay_ms(lua_State *lua_env);
int lua_set_fps_locked(lua_State *lua_env);
int lua_get_fps_locked(lua_State *lua_env);
int lua_timer_get_ticks(lua_State *lua_env);
int lua_set_camera_xy(lua_State *lua_env);
int lua_get_camera_x(lua_State *lua_env);
int lua_get_camera_y(lua_State *lua_env);
int lua_set_window_title(lua_State* lua_env);
int lua_get_version(lua_State *lua_env);
int lua_get_platform(lua_State *lua_env);
int lua_get_renderer(lua_State *lua_env);
int lua_engine_quit(lua_State *lua_env);


/* sprite.c */

int lua_load_texture(lua_State *lua_env);
int lua_get_texture_w(lua_State* lua_env);
int lua_get_texture_h(lua_State* lua_env);
int lua_destroy_texture(lua_State *lua_env);
int lua_create_sprite(lua_State *lua_env);
int lua_delete_sprite(lua_State *lua_env);
int lua_set_sprite_image(lua_State* lua_env);
int lua_set_sprite_align(lua_State* lua_env);
int lua_set_lighting_tint(lua_State* lua_env);
int lua_set_sprite_is_light(lua_State* lua_env);
int lua_set_sprite_blendmode(lua_State *lua_env);
int lua_get_sprite_blendmode(lua_State *lua_env);
int lua_set_sprite_alpha(lua_State *lua_env);
int lua_get_sprite_alpha(lua_State *lua_env);
int lua_set_sprite_color(lua_State *lua_env);
int lua_get_sprite_color(lua_State *lua_env);

int lua_set_sprite_layer(lua_State *lua_env);
int lua_get_sprite_layer(lua_State *lua_env);
int	lua_set_sprite_xy(lua_State *lua_env);
int lua_get_sprite_x(lua_State *lua_env);
int lua_get_sprite_y(lua_State *lua_env);
int lua_set_sprite_angle(lua_State *lua_env);
int lua_get_sprite_angle(lua_State *lua_env);
int lua_set_sprite_scale(lua_State *lua_env);
int lua_get_sprite_scale(lua_State *lua_env);
int lua_set_sprite_flip(lua_State *lua_env);
int lua_get_sprite_flip(lua_State *lua_env);
int lua_set_sprite_animation(lua_State *lua_env);
int lua_fix_sprite_to_screen(lua_State *lua_env);
int lua_get_sprite_collision(lua_State *lua_env);
int lua_set_sprite_phys(lua_State *lua_env);
int lua_set_sprite_active(lua_State *lua_env);
int lua_get_sprite_active(lua_State *lua_env);
int lua_get_num_sprites(lua_State *lua_env);
int lua_sort_sprites(lua_State *lua_env);
int lua_destroy_all_sprites(lua_State *lua_env);

/* primitives.c */

int lua_create_prim(lua_State *lua_env);
int lua_delete_prim(lua_State *lua_env);
int lua_set_prim_color(lua_State *lua_env);
int lua_set_prim_size(lua_State* lua_env);
int lua_set_prim_xy(lua_State *lua_env);
int lua_get_prim_x(lua_State *lua_env);
int lua_get_prim_y(lua_State *lua_env);
int lua_set_prim_angle(lua_State *lua_env);
int lua_get_prim_angle(lua_State *lua_env);
int lua_fix_prim_to_screen(lua_State *lua_env);
int lua_get_prim_collision(lua_State *lua_env);
int lua_set_prim_phys(lua_State *lua_env);
int lua_set_prim_active(lua_State *lua_env);
int lua_get_prim_active(lua_State *lua_env);
int lua_get_num_prims(lua_State *lua_env);
int lua_destroy_all_prims(lua_State *lua_env);

/* text.c */

int lua_load_ttf_font(lua_State *lua_env);
int lua_destroy_ttf_font(lua_State *lua_env);
int lua_create_text(lua_State *lua_env);
int lua_update_text(lua_State *lua_env);
int lua_delete_text(lua_State *lua_env);
int lua_set_text_xy(lua_State *lua_env);
int lua_get_text_x(lua_State *lua_env);
int lua_get_text_y(lua_State *lua_env);
int lua_set_text_scale(lua_State *lua_env);
int lua_get_text_scale(lua_State *lua_env);
int lua_set_text_align(lua_State *lua_env);
int lua_fix_text_to_screen(lua_State *lua_env);
int lua_set_text_active(lua_State *lua_env);
int lua_get_text_active(lua_State *lua_env);
int lua_get_num_text_objects(lua_State *lua_env);
int lua_destroy_all_text_objects(lua_State *lua_env);

/* ui.c */

int lua_load_ui_texture(lua_State *lua_env);
int lua_destroy_ui_texture(lua_State *lua_env);
int lua_create_ui_elem(lua_State *lua_env);
int lua_delete_ui_elem(lua_State *lua_env);
int lua_set_ui_elem_xy(lua_State *lua_env);
int lua_get_ui_elem_x(lua_State *lua_env);
int lua_get_ui_elem_y(lua_State *lua_env);
int lua_get_ui_elem_val(lua_State *lua_env);
int lua_set_ui_elem_val(lua_State* lua_env);
int lua_set_ui_elem_active(lua_State *lua_env);
int lua_get_ui_elem_active(lua_State *lua_env);
int lua_get_num_ui_elems(lua_State *lua_env);
int lua_destroy_all_ui_elems(lua_State *lua_env);

/* sound.c */

int lua_load_sound(lua_State *lua_env);
int lua_delete_sound(lua_State *lua_env);
int lua_play_sound(lua_State *lua_env);
int lua_pause_sound(lua_State *lua_env);
int lua_resume_sound(lua_State *lua_env);
int lua_destroy_all_sounds(lua_State *lua_env);

int  lua_load_music(lua_State *lua_env);
int  lua_destroy_music(lua_State *lua_env);
int  lua_play_music(lua_State *lua_env);
int  lua_pause_music(lua_State *lua_env);
int  lua_resume_music(lua_State *lua_env);
int  lua_stop_music(lua_State *lua_env);

/* input.c */

int lua_get_key_down(lua_State *lua_env);
int lua_get_key_up(lua_State *lua_env);
int lua_get_mouse_click(lua_State *lua_env);
int lua_get_mouse_release(lua_State *lua_env);
int lua_get_mouse_x(lua_State *lua_env);
int lua_get_mouse_y(lua_State *lua_env);
int lua_get_mouse_logical_x(lua_State* lua_env);
int lua_get_mouse_logical_y(lua_State* lua_env);

/* pathfind */

int lua_pathf_init(lua_State *lua_env);
int lua_pathf_shutdown(lua_State *lua_env);
int lua_pathf_get_map_w(lua_State *lua_env);
int lua_pathf_get_map_h(lua_State *lua_env);
int lua_pathf_get_tile_size(lua_State *lua_env);
int lua_pathf_get_num_chars(lua_State *lua_env);
int lua_pathf_set_walkability(lua_State *lua_env);
int lua_pathf_get_walkability(lua_State *lua_env);
int lua_pathf_find_path(lua_State *lua_env);
int lua_pathf_set_path_status(lua_State *lua_env);
int lua_pathf_get_path_status(lua_State *lua_env);
int lua_pathf_get_path_length(lua_State *lua_env);
int lua_pathf_get_path_location(lua_State *lua_env);
int lua_pathf_get_x_path(lua_State *lua_env);
int lua_pathf_get_y_path(lua_State *lua_env);
int lua_pathf_read_path(lua_State *lua_env);
int lua_pathf_read_path_x(lua_State *lua_env);
int lua_pathf_read_path_y(lua_State *lua_env);

/* physics.c */

int lua_phys_init(lua_State *lua_env);
int lua_phys_shutdown(lua_State *lua_env);


/* particles */

int lua_create_emitter(lua_State* lua_env);
int lua_delete_emitter(lua_State* lua_env);
int lua_set_emitter_active(lua_State* lua_env);
int lua_set_emitter_xy(lua_State* lua_env);
int lua_set_emitter_vel_xy(lua_State* lua_env);
int lua_set_emitter_color(lua_State* lua_env);
int lua_fix_emitter_to_screen(lua_State* lua_env);
int lua_get_num_active_particles(lua_State* lua_env);
int lua_get_num_emitters(lua_State* lua_env);
int lua_destroy_all_emitters(lua_State* lua_env);


/* threadpool */

int lua_create_threadpool(lua_State* lua_env);
int lua_destroy_threadpool(lua_State* lua_env);


bool fbl_lua_active = false;

lua_State* fbl_lua_env;

#endif

/*
 * Lua init.
 */
int fbl_lua_init(const char *file, void (*reg_funcs)())
{

#ifdef FBL_INCLUDE_LUA

	fbl_lua_env = luaL_newstate();
	luaL_openlibs(fbl_lua_env);

	char *new_path = engine_get_platform_asset_path(file);

#ifdef __ANDROID__
	char* buf = read_file_to_buf(new_path);
	luaL_loadstring(fbl_lua_env, buf);
	free(buf);
#else
	if (luaL_loadfile(fbl_lua_env, new_path)) {
		fprintf(FBL_ERROR_OUT, "%s", lua_tostring(fbl_lua_env, -1));	// print error msg, including syntax errors, very helpful!
	}
#endif


	//free(new_path);

	/* run file once (does not need to contain anything outside the functions)*/


	lua_call(fbl_lua_env, 0, 0);

	register_fbl_functions_to_lua();	// register all of the fbl functions
	if(reg_funcs != NULL) reg_funcs();	// register additional functions (from your game)

	fbl_lua_active = true;

	engine_lua_start(); /* Call the start function in main.lua */

#endif

	return 0;

}

/*
 * Lua shutdown.
 */
void fbl_lua_shutdown()
{

#ifdef FBL_INCLUDE_LUA

	if (fbl_lua_active)
	{
		lua_close(fbl_lua_env);
		fbl_lua_active = false;
	}

#endif

}


/*
 *  Load a Lua file and run it. Look above, fbl_lua_init() on how to get error msg.
 */
void fbl_lua_do_file(const char *file)
{
    
#ifdef FBL_INCLUDE_LUA
	luaL_loadfile(fbl_lua_env, file);
	lua_call(fbl_lua_env, 0, 0);
/*
	if (luaL_loadfile(fbl_lua_env, file) || lua_pcall(fbl_lua_env, 0, 0, 0))
		fprintf(FBL_ERROR_OUT, "Cannot run lua file: %s\n", lua_tostring(fbl_lua_env, -1));
*/
#endif
    
}


/*
 *  This is used only by the Android port atm. And loadMap_fbl.
 *	Load a text-file (Lua-file) to a buffer and return it.
 *	This can later be run by luaL_loadstring() and gets around the fact
 *	that Lua uses FILE* to read. This enables you to read and run Lua-files
 *	on Android.
 * 
 */
char *read_file_to_buf(const char* filename)
{

	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char *res = (char*)malloc(res_size + 1);
	if (res == NULL) return NULL;

	Sint64 nb_read_total = 0, nb_read = 1;
	
	char *buf = res;
	
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	
	SDL_RWclose(rw);
	
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	
	return res;

}


/*
 *  Register the following functions so they can be called from Lua.
 */
void register_fbl_functions_to_lua()
{

#ifdef FBL_INCLUDE_LUA

	/* fbl.c */

	lua_register(fbl_lua_env, "fbl_engine_init", lua_engine_init);
	lua_register(fbl_lua_env, "fbl_set_window_mode", lua_set_window_mode);
	lua_register(fbl_lua_env, "fbl_set_assets_folder_name", lua_set_assets_folder_name);
	lua_register(fbl_lua_env, "fbl_log", lua_fbl_log);
	lua_register(fbl_lua_env, "fbl_set_clear_color", lua_set_clear_color);
	lua_register(fbl_lua_env, "fbl_set_screen_wh", lua_set_screen_wh);
	lua_register(fbl_lua_env, "fbl_get_screen_w", lua_get_screen_w);
	lua_register(fbl_lua_env, "fbl_get_screen_h", lua_get_screen_h);
	lua_register(fbl_lua_env, "fbl_set_render_scale", lua_set_render_scale);
	lua_register(fbl_lua_env, "fbl_set_viewport", lua_set_viewport);
	lua_register(fbl_lua_env, "fbl_set_render_logical_size", lua_set_render_logical_size);
	lua_register(fbl_lua_env, "fbl_set_fps", lua_set_fps);
	lua_register(fbl_lua_env, "fbl_get_fps", lua_get_fps);
	lua_register(fbl_lua_env, "fbl_get_raw_frames_count", lua_get_raw_frames_count);
	lua_register(fbl_lua_env, "fbl_set_system_delay_ms", lua_set_system_delay_ms);
	lua_register(fbl_lua_env, "fbl_get_system_delay_ms", lua_get_system_delay_ms);
	lua_register(fbl_lua_env, "fbl_set_fps_locked", lua_set_fps_locked);
	lua_register(fbl_lua_env, "fbl_get_fps_locked", lua_get_fps_locked);
	lua_register(fbl_lua_env, "fbl_timer_get_ticks", lua_timer_get_ticks);
	lua_register(fbl_lua_env, "fbl_set_camera_xy", lua_set_camera_xy);
	lua_register(fbl_lua_env, "fbl_get_camera_x", lua_get_camera_x);
	lua_register(fbl_lua_env, "fbl_get_camera_y", lua_get_camera_y);
	lua_register(fbl_lua_env, "fbl_set_window_title", lua_set_window_title);
	lua_register(fbl_lua_env, "fbl_get_version", lua_get_version);
	lua_register(fbl_lua_env, "fbl_get_platform", lua_get_platform);
	lua_register(fbl_lua_env, "fbl_get_renderer", lua_get_renderer);
	lua_register(fbl_lua_env, "fbl_engine_quit", lua_engine_quit);


	/* sprite.c */

	lua_register(fbl_lua_env, "fbl_load_texture", lua_load_texture);
	lua_register(fbl_lua_env, "fbl_get_texture_w", lua_get_texture_w);
	lua_register(fbl_lua_env, "fbl_get_texture_h", lua_get_texture_h);
	lua_register(fbl_lua_env, "fbl_destroy_texture", lua_destroy_texture);
	lua_register(fbl_lua_env, "fbl_create_sprite", lua_create_sprite);
	lua_register(fbl_lua_env, "fbl_delete_sprite", lua_delete_sprite);
	lua_register(fbl_lua_env, "fbl_set_sprite_image", lua_set_sprite_image);
	lua_register(fbl_lua_env, "fbl_set_sprite_align", lua_set_sprite_align);
	lua_register(fbl_lua_env, "fbl_set_lighting_tint", lua_set_lighting_tint);
	lua_register(fbl_lua_env, "fbl_set_sprite_is_light", lua_set_sprite_is_light);
	lua_register(fbl_lua_env, "fbl_set_sprite_blendmode", lua_set_sprite_blendmode);
	lua_register(fbl_lua_env, "fbl_get_sprite_blendmode", lua_get_sprite_blendmode);
	lua_register(fbl_lua_env, "fbl_set_sprite_alpha", lua_set_sprite_alpha);
	lua_register(fbl_lua_env, "fbl_get_sprite_alpha", lua_get_sprite_alpha);
	lua_register(fbl_lua_env, "fbl_set_sprite_color", lua_set_sprite_color);
	lua_register(fbl_lua_env, "fbl_get_sprite_color", lua_get_sprite_color);
	lua_register(fbl_lua_env, "fbl_set_sprite_layer", lua_set_sprite_layer);
	lua_register(fbl_lua_env, "fbl_get_sprite_layer", lua_get_sprite_layer);
	lua_register(fbl_lua_env, "fbl_set_sprite_xy", lua_set_sprite_xy);
	lua_register(fbl_lua_env, "fbl_get_sprite_x", lua_get_sprite_x);
	lua_register(fbl_lua_env, "fbl_get_sprite_y", lua_get_sprite_y);
	lua_register(fbl_lua_env, "fbl_set_sprite_angle", lua_set_sprite_angle);
	lua_register(fbl_lua_env, "fbl_get_sprite_angle", lua_get_sprite_angle);
	lua_register(fbl_lua_env, "fbl_set_sprite_scale", lua_set_sprite_scale);
	lua_register(fbl_lua_env, "fbl_get_sprite_scale", lua_get_sprite_scale);
	lua_register(fbl_lua_env, "fbl_set_sprite_flip", lua_set_sprite_flip);
	lua_register(fbl_lua_env, "fbl_get_sprite_flip", lua_get_sprite_flip);
	lua_register(fbl_lua_env, "fbl_set_sprite_animation", lua_set_sprite_animation);
	lua_register(fbl_lua_env, "fbl_fix_sprite_to_screen", lua_fix_sprite_to_screen);
	lua_register(fbl_lua_env, "fbl_get_sprite_collision", lua_get_sprite_collision);
	lua_register(fbl_lua_env, "fbl_set_sprite_phys", lua_set_sprite_phys);
	lua_register(fbl_lua_env, "fbl_set_sprite_active", lua_set_sprite_active);
	lua_register(fbl_lua_env, "fbl_get_sprite_active", lua_get_sprite_active);
	lua_register(fbl_lua_env, "fbl_get_num_sprites", lua_get_num_sprites);
	lua_register(fbl_lua_env, "fbl_sort_sprites", lua_sort_sprites);
	lua_register(fbl_lua_env, "fbl_destroy_all_sprites", lua_destroy_all_sprites);


	/* primitives.c */

	lua_register(fbl_lua_env, "fbl_create_prim", lua_create_prim);
	lua_register(fbl_lua_env, "fbl_delete_prim", lua_delete_prim);
	lua_register(fbl_lua_env, "fbl_set_prim_color", lua_set_prim_color);
	lua_register(fbl_lua_env, "fbl_set_prim_size", lua_set_prim_size);
	lua_register(fbl_lua_env, "fbl_set_prim_xy", lua_set_prim_xy);
	lua_register(fbl_lua_env, "fbl_get_prim_x", lua_get_prim_x);
	lua_register(fbl_lua_env, "fbl_get_prim_y", lua_get_prim_y);
	lua_register(fbl_lua_env, "fbl_set_prim_angle", lua_set_prim_angle);
	lua_register(fbl_lua_env, "fbl_get_prim_angle", lua_get_prim_angle);
	lua_register(fbl_lua_env, "fbl_fix_prim_to_screen", lua_fix_prim_to_screen);
	lua_register(fbl_lua_env, "fbl_get_prim_collision", lua_get_prim_collision);
	lua_register(fbl_lua_env, "fbl_set_prim_phys", lua_set_prim_phys);
	lua_register(fbl_lua_env, "fbl_set_prim_active", lua_set_prim_active);
	lua_register(fbl_lua_env, "fbl_get_prim_active", lua_get_prim_active);
	lua_register(fbl_lua_env, "fbl_get_num_prims", lua_get_num_prims);
	lua_register(fbl_lua_env, "fbl_destroy_all_prims", lua_destroy_all_prims);


	/* text.c */

	lua_register(fbl_lua_env, "fbl_load_ttf_font", lua_load_ttf_font);
	lua_register(fbl_lua_env, "fbl_destroy_ttf_font", lua_destroy_ttf_font);
	lua_register(fbl_lua_env, "fbl_create_text", lua_create_text);
	lua_register(fbl_lua_env, "fbl_update_text", lua_update_text );
	lua_register(fbl_lua_env, "fbl_delete_text", lua_delete_text);
	lua_register(fbl_lua_env, "fbl_set_text_xy", lua_set_text_xy);
	lua_register(fbl_lua_env, "fbl_get_text_x", lua_get_text_x);
	lua_register(fbl_lua_env, "fbl_get_text_y", lua_get_text_y);
	lua_register(fbl_lua_env, "fbl_set_text_scale", lua_set_text_scale);
	lua_register(fbl_lua_env, "fbl_get_text_scale", lua_get_text_scale);
	lua_register(fbl_lua_env, "fbl_set_text_align", lua_set_text_align);
	lua_register(fbl_lua_env, "fbl_fix_text_to_screen", lua_fix_text_to_screen);
	lua_register(fbl_lua_env, "fbl_set_text_active", lua_set_text_active);
	lua_register(fbl_lua_env, "fbl_get_text_active", lua_get_text_active);
	lua_register(fbl_lua_env, "fbl_get_num_text_objects", lua_get_num_text_objects);
	lua_register(fbl_lua_env, "fbl_destroy_all_text_objects", lua_destroy_all_text_objects);


	/* ui.c */

	lua_register(fbl_lua_env, "fbl_load_ui_texture", lua_load_ui_texture);
	lua_register(fbl_lua_env, "fbl_destroy_ui_texture", lua_destroy_ui_texture);
	lua_register(fbl_lua_env, "fbl_create_ui_elem", lua_create_ui_elem);
	lua_register(fbl_lua_env, "fbl_delete_ui_elem", lua_delete_ui_elem);
	lua_register(fbl_lua_env, "fbl_set_ui_elem_xy", lua_set_ui_elem_xy);
	lua_register(fbl_lua_env, "fbl_get_ui_elem_x", lua_get_ui_elem_x);
	lua_register(fbl_lua_env, "fbl_get_ui_elem_y", lua_get_ui_elem_y);
	lua_register(fbl_lua_env, "fbl_get_ui_elem_val", lua_get_ui_elem_val);
	lua_register(fbl_lua_env, "fbl_set_ui_elem_val", lua_set_ui_elem_val);
	lua_register(fbl_lua_env, "fbl_set_ui_elem_active", lua_set_ui_elem_active);
	lua_register(fbl_lua_env, "fbl_get_ui_elem_active", lua_get_ui_elem_active);
	lua_register(fbl_lua_env, "fbl_get_num_ui_elems", lua_get_num_ui_elems);
	lua_register(fbl_lua_env, "fbl_destroy_all_ui_elems", lua_destroy_all_ui_elems);


	/* sound.c */

	lua_register(fbl_lua_env, "fbl_load_sound", lua_load_sound);
	lua_register(fbl_lua_env, "fbl_delete_sound", lua_delete_sound);
	lua_register(fbl_lua_env, "fbl_play_sound", lua_play_sound);
	lua_register(fbl_lua_env, "fbl_pause_sound", lua_pause_sound);
	lua_register(fbl_lua_env, "fbl_resume_sound", lua_resume_sound);
	lua_register(fbl_lua_env, "fbl_destroy_all_sounds", lua_destroy_all_sounds);

	lua_register(fbl_lua_env, "fbl_load_music", lua_load_music);
	lua_register(fbl_lua_env, "fbl_destroy_music", lua_destroy_music);
	lua_register(fbl_lua_env, "fbl_play_music", lua_play_music);
	lua_register(fbl_lua_env, "fbl_pause_music", lua_pause_music);
	lua_register(fbl_lua_env, "fbl_resume_music", lua_resume_music);
	lua_register(fbl_lua_env, "fbl_stop_music", lua_stop_music);


	/* input.c */

	lua_register(fbl_lua_env, "fbl_get_key_down", lua_get_key_down);
	lua_register(fbl_lua_env, "fbl_get_key_up", lua_get_key_up);
	lua_register(fbl_lua_env, "fbl_get_mouse_click", lua_get_mouse_click);
	lua_register(fbl_lua_env, "fbl_get_mouse_release", lua_get_mouse_release);
	lua_register(fbl_lua_env, "fbl_get_mouse_x", lua_get_mouse_x);
	lua_register(fbl_lua_env, "fbl_get_mouse_y", lua_get_mouse_y);
	lua_register(fbl_lua_env, "fbl_get_mouse_logical_x", lua_get_mouse_logical_x);
	lua_register(fbl_lua_env, "fbl_get_mouse_logical_y", lua_get_mouse_logical_y);


	/* pathfind */

	lua_register(fbl_lua_env, "fbl_pathf_init", lua_pathf_init);
	lua_register(fbl_lua_env, "fbl_pathf_shutdown", lua_pathf_shutdown);
	lua_register(fbl_lua_env, "fbl_pathf_get_map_w", lua_pathf_get_map_w);
	lua_register(fbl_lua_env, "fbl_pathf_get_map_h", lua_pathf_get_map_h);
	lua_register(fbl_lua_env, "fbl_pathf_get_tile_size", lua_pathf_get_tile_size);
	lua_register(fbl_lua_env, "fbl_pathf_get_num_chars", lua_pathf_get_num_chars);
	lua_register(fbl_lua_env, "fbl_pathf_set_walkability", lua_pathf_set_walkability);
	lua_register(fbl_lua_env, "fbl_pathf_get_walkability", lua_pathf_get_walkability);
	lua_register(fbl_lua_env, "fbl_pathf_find_path", lua_pathf_find_path);
	lua_register(fbl_lua_env, "fbl_pathf_set_path_status", lua_pathf_set_path_status);
	lua_register(fbl_lua_env, "fbl_pathf_get_path_status", lua_pathf_get_path_status);
	lua_register(fbl_lua_env, "fbl_pathf_get_path_length", lua_pathf_get_path_length);
	lua_register(fbl_lua_env, "fbl_pathf_get_path_location", lua_pathf_get_path_location);
	lua_register(fbl_lua_env, "fbl_pathf_get_x_path", lua_pathf_get_x_path);
	lua_register(fbl_lua_env, "fbl_pathf_get_y_path", lua_pathf_get_y_path);
	lua_register(fbl_lua_env, "fbl_pathf_read_path", lua_pathf_read_path);
	lua_register(fbl_lua_env, "fbl_pathf_read_path_x", lua_pathf_read_path_x);
	lua_register(fbl_lua_env, "fbl_pathf_read_path_y", lua_pathf_read_path_y);


	/* physics.c */

	lua_register(fbl_lua_env, "fbl_phys_init", lua_phys_init);
	lua_register(fbl_lua_env, "fbl_phys_shutdown", lua_phys_shutdown);


	/* particles */

	lua_register(fbl_lua_env, "fbl_create_emitter", lua_create_emitter);
	lua_register(fbl_lua_env, "fbl_delete_emitter", lua_delete_emitter);
	lua_register(fbl_lua_env, "fbl_set_emitter_active", lua_set_emitter_active);
	lua_register(fbl_lua_env, "fbl_set_emitter_xy", lua_set_emitter_xy);
	lua_register(fbl_lua_env, "fbl_set_emitter_vel_xy", lua_set_emitter_vel_xy);
	lua_register(fbl_lua_env, "fbl_set_emitter_color", lua_set_emitter_color);
	lua_register(fbl_lua_env, "fbl_fix_emitter_to_screen", lua_fix_emitter_to_screen);
	lua_register(fbl_lua_env, "fbl_get_num_active_particles", lua_get_num_active_particles);
	lua_register(fbl_lua_env, "fbl_get_num_emitters", lua_get_num_emitters);
	lua_register(fbl_lua_env, "fbl_destroy_all_emitters", lua_destroy_all_emitters);


	/* threadpool */

	lua_register(fbl_lua_env, "fbl_create_threadpool", lua_create_threadpool);
	lua_register(fbl_lua_env, "fbl_destroy_threadpool", lua_destroy_threadpool);


#endif

}

/*
 * The only three functions called from C to Lua.
 */
void engine_lua_start()
{

#ifdef FBL_INCLUDE_LUA

	if (fbl_lua_active)
	{
		lua_getglobal(fbl_lua_env, "fbl_lua_start");
		lua_call(fbl_lua_env, 0, 0);
	}

#endif

}

void engine_lua_loop()
{

#ifdef FBL_INCLUDE_LUA

	if (fbl_lua_active)
	{
		lua_getglobal(fbl_lua_env, "fbl_lua_loop");
		lua_call(fbl_lua_env, 0, 0);
	}

#endif

}

void engine_lua_end()
{

#ifdef FBL_INCLUDE_LUA

	if (fbl_lua_active)
	{
		lua_getglobal(fbl_lua_env, "fbl_lua_end");
		lua_call(fbl_lua_env, 0, 0);
	}

#endif

}

#ifdef FBL_INCLUDE_LUA

/* All registered fbl functions "callable" from Lua. Here we go! */

/* fbl.c */

int lua_engine_init(lua_State *lua_env)
{

	int w = (int)lua_tonumber(lua_env, 1);
	int h = (int)lua_tonumber(lua_env, 2);
	int fps = (int)lua_tonumber(lua_env, 3);

	lua_pushnumber(lua_env, (lua_Number)fbl_engine_init(w, h, fps));

	return 1;

}

int lua_set_window_mode(lua_State *lua_env)
{

	int mode = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_set_window_mode(mode));

	return 1;

}

int lua_set_assets_folder_name(lua_State* lua_env)
{

	char* name = (char*)lua_tostring(lua_env, 1);

	fbl_set_assets_folder_name(name);

	return 1;

}

int lua_fbl_log(lua_State* lua_env)
{

	char* format = (char*)lua_tostring(lua_env, 1);

	//int variable_args = (int)lua_tonumber(lua_env, 2); // NOTE: figure out

	fbl_log(format);
	
	return 1;

}

int lua_set_clear_color(lua_State *lua_env)
{

	int r = (int)lua_tonumber(lua_env, 1);
	int g = (int)lua_tonumber(lua_env, 2);
	int b = (int)lua_tonumber(lua_env, 3);
	int a = (int)lua_tonumber(lua_env, 4);

	fbl_set_clear_color(r, g, b, a);

	return 1;

}

int lua_set_screen_wh(lua_State* lua_env)
{

	int w = (int)lua_tonumber(lua_env, 1);
	int h = (int)lua_tonumber(lua_env, 2);

	fbl_set_screen_wh(w, h);

	return 1;

}

int lua_get_screen_w(lua_State *lua_env)
{ 

	lua_pushnumber(lua_env, (lua_Number)fbl_get_screen_w());

	return 1;

}

int lua_get_screen_h(lua_State *lua_env)
{
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_screen_h());

	return 1;

}

int lua_set_render_scale(lua_State* lua_env)
{

	float x = (float)lua_tonumber(lua_env, 1);
	float y = (float)lua_tonumber(lua_env, 2);

	fbl_set_render_scale(x, y);
	
	return 1;

}

int lua_set_viewport(lua_State* lua_env)
{

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);
	int w = (int)lua_tonumber(lua_env, 3);
	int h = (int)lua_tonumber(lua_env, 4);

	fbl_set_viewport(x, y, w, h);

	return 1;

}

int lua_set_render_logical_size(lua_State* lua_env)
{

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);

	fbl_set_render_logical_size(x, y);
	
	return 1;

}

int lua_set_fps(lua_State *lua_env)
{ 

	int fps = (int)lua_tonumber(lua_env, 1);

	fbl_set_fps(fps);

	return 1; 

}

int lua_get_fps(lua_State *lua_env)
{
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_fps());

	return 1; 

}

int lua_get_raw_frames_count(lua_State *lua_env)
{ 
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_raw_frames_count());

	return 1; 

}

int lua_set_system_delay_ms(lua_State *lua_env)
{ 
	
	int delay = (int)lua_tonumber(lua_env, 1);

	fbl_set_system_delay_ms(delay);

	return 1; 

}

int lua_get_system_delay_ms(lua_State *lua_env)
{ 
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_system_delay_ms());

	return 1;

}

int lua_set_fps_locked(lua_State *lua_env)
{ 

	int locked = (int)lua_tonumber(lua_env, 1);

	fbl_set_fps_locked(locked);

	return 1;

}

int lua_get_fps_locked(lua_State *lua_env)
{
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_fps_locked());

	return 1; 

}

int lua_timer_get_ticks(lua_State *lua_env)
{
	
	lua_pushnumber(lua_env, (lua_Number)fbl_timer_get_ticks());

	return 1; 

}

int lua_set_camera_xy(lua_State *lua_env)
{ 

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);

	fbl_set_camera_xy(x, y);

	return 1; 

}

int lua_get_camera_x(lua_State *lua_env)
{ 

	lua_pushnumber(lua_env, (lua_Number)fbl_get_camera_x());

	return 1; 

}

int lua_get_camera_y(lua_State *lua_env)
{ 
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_camera_y());

	return 1; 

}

int lua_set_window_title(lua_State* lua_env)
{

	char* title = (char*)lua_tostring(lua_env, 1);

	fbl_set_window_title(title);

	return 1;

}

int lua_get_version(lua_State *lua_env)
{ 

	lua_pushstring(lua_env, fbl_get_version());

	return 1; 

}

int lua_get_platform(lua_State *lua_env)
{ 
	
	lua_pushstring(lua_env, fbl_get_platform());

	return 1; 

}

int lua_get_renderer(lua_State *lua_env)
{ 

	lua_pushstring(lua_env, fbl_get_renderer());

	return 1; 

}

int lua_engine_quit(lua_State *lua_env)
{
	
	fbl_engine_quit();
	
	return 1; 

}


/* sprite.c */

int lua_load_texture(lua_State *lua_env)
{

	char *img_file = (char *)lua_tostring(lua_env, 1);

	fbl_load_texture(img_file);

	return 1;

}

int lua_get_texture_w(lua_State* lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_texture_w());

	return 1;

}

int lua_get_texture_h(lua_State* lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_texture_h());

	return 1;

}

int lua_destroy_texture(lua_State *lua_env)
{ 

	fbl_destroy_texture();

	return 1;

}

int lua_create_sprite(lua_State *lua_env)
{

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);
	int w = (int)lua_tonumber(lua_env, 3);
	int h = (int)lua_tonumber(lua_env, 4);
	int r = (int)lua_tonumber(lua_env, 5);

	lua_pushnumber(lua_env, (lua_Number)fbl_create_sprite(x, y, w, h, r));

	return 1;

}

int lua_delete_sprite(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_sprite(id);

	return 1;

}

int lua_set_sprite_image(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);
	int w = (int)lua_tonumber(lua_env, 4);
	int h = (int)lua_tonumber(lua_env, 5);
	int r = (int)lua_tonumber(lua_env, 6);

	fbl_set_sprite_image(id, x, y, w, h, r);

	return 1;

}

int lua_set_sprite_align(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int mode = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_align(mode);

	return 1;

}


int lua_set_lighting_tint(lua_State* lua_env)
{

	int on_off = (int)lua_tonumber(lua_env, 1);
	int r = (int)lua_tonumber(lua_env, 2);
	int g = (int)lua_tonumber(lua_env, 3);
	int b = (int)lua_tonumber(lua_env, 4);

	fbl_set_lighting_tint(on_off, r, g, b);

	return 1;

}

int lua_set_sprite_is_light(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int light = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_is_light(id, light);

	return 1;

}

int lua_set_sprite_blendmode(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int mode = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_blendmode(id, mode);

	return 1;

}

int lua_get_sprite_blendmode(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_blendmode(id));

	return 1;

}

int lua_set_sprite_alpha(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int alpha = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_alpha(id, alpha);

	return 1;

}

int lua_get_sprite_alpha(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_alpha(id));

	return 1;

}

int lua_set_sprite_color(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int r = (int)lua_tonumber(lua_env, 2);
	int g = (int)lua_tonumber(lua_env, 3);
	int b = (int)lua_tonumber(lua_env, 4);

	fbl_set_sprite_color(id, r, g, b);

	return 1;

}

int lua_get_sprite_color(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	unsigned char r, g, b;

	fbl_get_sprite_color(id, &r, &g, &b);

	lua_pushnumber(lua_env, (lua_Number)r);
	lua_pushnumber(lua_env, (lua_Number)g);
	lua_pushnumber(lua_env, (lua_Number)b);

	return 3;
}

int lua_set_sprite_layer(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int layer = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_layer(id, layer);

	return 1;

}

int lua_get_sprite_layer(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_layer(id));

	return 1;

}

int lua_set_sprite_xy(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);

	fbl_set_sprite_xy(id, x, y);

	return 1;

}

int lua_get_sprite_x(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_x(id));

	return 1;

}

int lua_get_sprite_y(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_y(id));

	return 1;

}

int lua_set_sprite_angle(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	double angle = (double)lua_tonumber(lua_env, 2);

	fbl_set_sprite_angle(id, angle);

	return 1;

}

int lua_get_sprite_angle(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_angle(id));

	return 1;

}

int lua_set_sprite_scale(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	float scale = (float)lua_tonumber(lua_env, 2);

	fbl_set_sprite_scale(id, scale);

	return 1;

}

int lua_get_sprite_scale(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_scale(id));

	return 1;

}

int lua_set_sprite_flip(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int flipmode = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_flip(id, flipmode);

	return 1;

}

int lua_get_sprite_flip(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_flip(id));

	return 1;

}

int lua_set_sprite_animation(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int on = (int)lua_tonumber(lua_env, 2);
	int x = (int)lua_tonumber(lua_env, 3);
	int y = (int)lua_tonumber(lua_env, 4);
	int w = (int)lua_tonumber(lua_env, 5);
	int h = (int)lua_tonumber(lua_env, 6);
	int num_frames = (int)lua_tonumber(lua_env, 7);
	int speed = (int)lua_tonumber(lua_env, 8);
	int loop = (int)lua_tonumber(lua_env, 9);

	fbl_set_sprite_animation(id, on, x, y, w, h, num_frames, speed, loop);

	return 1;

}

int lua_fix_sprite_to_screen(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int fix = (int)lua_tonumber(lua_env, 2);

	fbl_fix_sprite_to_screen(id, fix);

	return 1;

}

int lua_get_sprite_collision(lua_State *lua_env)
{

	int id_1 = (int)lua_tonumber(lua_env, 1);
	int id_2 = (int)lua_tonumber(lua_env, 2);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_collision(id_1, id_2));

	return 1;

}

int lua_set_sprite_phys(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int on = (int)lua_tonumber(lua_env, 2);
	int type = (int)lua_tonumber(lua_env, 3);
	int body_type = (int)lua_tonumber(lua_env, 4);
	int can_rotate = (int)lua_tonumber(lua_env, 5);

	fbl_set_sprite_phys(id, on, type, body_type, can_rotate);

	return 1;

}

int lua_set_sprite_active(lua_State *lua_env)
{ 

	int id = (int)lua_tonumber(lua_env, 1);
	int active = (int)lua_tonumber(lua_env, 2);

	fbl_set_sprite_active(id, active);

	return 1; 

}

int lua_get_sprite_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_sprite_active(id));

	return 1;

}

int lua_get_num_sprites(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_sprites());

	return 1;

}

int lua_sort_sprites(lua_State *lua_env)
{

	int mode = (int)lua_tonumber(lua_env, 1);

	fbl_sort_sprites(mode);

	return 1;

}

int lua_destroy_all_sprites(lua_State *lua_env)
{
	
	fbl_destroy_all_sprites();

	return 1;

}

/* primitives.c */

int lua_create_prim(lua_State *lua_env)
{ 

	int type = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);
	int w = (int)lua_tonumber(lua_env, 4);
	int h = (int)lua_tonumber(lua_env, 5);
	int r = (int)lua_tonumber(lua_env, 6);
	int thick = (int)lua_tonumber(lua_env, 7);
	int fill = (int)lua_tonumber(lua_env, 8);

	lua_pushnumber(lua_env, (lua_Number)fbl_create_prim(type, x, y, w, h, r, thick, fill));

	return 1;

}
int lua_delete_prim(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_prim(id);

	return 1;

}

int lua_set_prim_color(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int r = (int)lua_tonumber(lua_env, 2);
	int g = (int)lua_tonumber(lua_env, 3);
	int b = (int)lua_tonumber(lua_env, 4);
	int a = (int)lua_tonumber(lua_env, 5);

	fbl_set_prim_color(id, r, g, b, a);

	return 1;

}

int lua_set_prim_size(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int w = (int)lua_tonumber(lua_env, 2);
	int h = (int)lua_tonumber(lua_env, 3);
	int r = (int)lua_tonumber(lua_env, 4);

	fbl_set_prim_size(id, w, h, r);

	return 1;

}

int lua_set_prim_xy(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);

	fbl_set_prim_xy(id, x, y);
	
	return 1;

}

int lua_get_prim_x(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_prim_x(id));

	return 1;

}

int lua_get_prim_y(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_prim_y(id));

	return 1;

}

int lua_set_prim_angle(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	double angle = (double)lua_tonumber(lua_env, 2);

	fbl_set_prim_angle(id, angle);

	return 1;
}

int lua_get_prim_angle(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_prim_angle(id));

	return 1;

}

int lua_fix_prim_to_screen(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int fix = (int)lua_tonumber(lua_env, 2);

	fbl_fix_prim_to_screen(id, fix);

	return 1;

}

int lua_get_prim_collision(lua_State *lua_env)
{
	
	int id_1 = (int)lua_tonumber(lua_env, 1);
	int id_2 = (int)lua_tonumber(lua_env, 2);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_prim_collision(id_1, id_2));

	return 1;

}

int lua_set_prim_phys(lua_State *lua_env)
{ 

	int id = (int)lua_tonumber(lua_env, 1);
	int on = (int)lua_tonumber(lua_env, 2);
	int body_type = (int)lua_tonumber(lua_env, 3);
	int can_rotate = (int)lua_tonumber(lua_env, 4);

	fbl_set_prim_phys(id, on, body_type, can_rotate);

	return 1;

}

int lua_set_prim_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int active = (int)lua_tonumber(lua_env, 2);

	fbl_set_prim_active(id, active);

	return 1;

}

int lua_get_prim_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_prim_active(id));

	return 1;

}

int lua_get_num_prims(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_prims());

	return 1;
}

int lua_destroy_all_prims(lua_State *lua_env)
{

	fbl_destroy_all_prims();

	return 1;

}

/* text.c */

int lua_load_ttf_font(lua_State *lua_env)
{
	
	char *font_file = (char *)lua_tostring(lua_env, 1);
	int size = (int)lua_tonumber(lua_env, 2);

	fbl_load_ttf_font(font_file, size);

	return 1;

}

int lua_destroy_ttf_font(lua_State *lua_env)
{

	fbl_destroy_ttf_font();

	return 1;

}

int lua_create_text(lua_State *lua_env)
{

	int r = (int)lua_tonumber(lua_env, 1);
	int g = (int)lua_tonumber(lua_env, 2);
	int b = (int)lua_tonumber(lua_env, 3);
	int a = (int)lua_tonumber(lua_env, 4);
	char *format = (char *)lua_tostring(lua_env, 5);
	
	//int variable_args = (int)lua_tonumber(lua_env, 6); // NOTE: figure out

	lua_pushnumber(lua_env, (lua_Number)fbl_create_text(r, g, b, a, format));

	return 1;

}

int lua_update_text(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int r = (int)lua_tonumber(lua_env, 2);
	int g = (int)lua_tonumber(lua_env, 3);
	int b = (int)lua_tonumber(lua_env, 4);
	int a = (int)lua_tonumber(lua_env, 5);
	char *format = (char *)lua_tostring(lua_env, 6);

	fbl_update_text(id, r, g, b, a, format);

	return 1;

}

int lua_delete_text(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_text(id);

	return 1;

}

int lua_set_text_xy(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);

	fbl_set_text_xy(id, x, y);
	
	return 1;

}

int lua_get_text_x(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_text_x(id));
	
	return 1;

}

int lua_get_text_y(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_text_y(id));

	return 1;

}

int lua_set_text_scale(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	float scale = (float)lua_tonumber(lua_env, 2);

	fbl_set_text_scale(id, scale);

	return 1;

}

int lua_get_text_scale(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_text_scale(id));

	return 1;

}

int lua_set_text_align(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int align = (int)lua_tonumber(lua_env, 2);

	fbl_set_text_align(id, align);

	return 1;

}

int lua_fix_text_to_screen(lua_State *lua_env)
{
	
	int id = (int)lua_tonumber(lua_env, 1);
	int fix = (int)lua_tonumber(lua_env, 2);

	fbl_fix_text_to_screen(id, fix);
	
	return 1;

}

int lua_set_text_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int active = (int)lua_tonumber(lua_env, 2);

	fbl_set_text_active(id, active);

	return 1;

}

int lua_get_text_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_text_active(id));

	return 1;

}

int lua_get_num_text_objects(lua_State *lua_env)
{
	
	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_text_objects());

	return 1;

}

int lua_destroy_all_text_objects(lua_State *lua_env)
{

	fbl_destroy_all_text_objects();

	return 1;

}

/* ui.c */

int lua_load_ui_texture(lua_State *lua_env)
{

	char *ui_file = (char *)lua_tostring(lua_env, 1);

	fbl_load_ui_texture(ui_file);

	return 1;

}

int lua_destroy_ui_texture(lua_State *lua_env)
{
	
	fbl_destroy_ui_texture();
	
	return 1;

}

int lua_create_ui_elem(lua_State *lua_env)
{

	int type = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);
	int w = (int)lua_tonumber(lua_env, 4);
	int h = (int)lua_tonumber(lua_env, 5);

	lua_pushnumber(lua_env, (lua_Number)fbl_create_ui_elem(type, x, y, w, h, NULL));

	return 1;

}

int lua_delete_ui_elem(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_ui_elem(id);

	return 1;

}

int lua_set_ui_elem_xy(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);

	fbl_set_ui_elem_xy(id, x, y);

	return 1;

}

int lua_get_ui_elem_x(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_ui_elem_x(id));

	return 1;

}

int lua_get_ui_elem_y(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_ui_elem_y(id));

	return 1;

}

int lua_get_ui_elem_val(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_ui_elem_val(id));

	return 1;

}

int lua_set_ui_elem_val(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int value = (int)lua_tonumber(lua_env, 2);

	fbl_set_ui_elem_val(id, value);

	return 1;

}

int lua_set_ui_elem_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int active = (int)lua_tonumber(lua_env, 2);

	fbl_set_ui_elem_active(id, active);

	return 1;

}

int lua_get_ui_elem_active(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_ui_elem_active(id));

	return 1;

}

int lua_get_num_ui_elems(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_ui_elems());

	return 1;

}

int lua_destroy_all_ui_elems(lua_State *lua_env)
{

	fbl_destroy_all_ui_elems();

	return 1;

}

/* sound.c */

int lua_load_sound(lua_State *lua_env)
{

	char *sample_file = (char *)lua_tostring(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_load_sound(sample_file));

	return 1;

}

int lua_delete_sound(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_sound(id);

	return 1;

}

int lua_play_sound(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_play_sound(id);

	return 1;

}

int lua_pause_sound(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_pause_sound(id);

	return 1;

}

int lua_resume_sound(lua_State *lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_pause_sound(id);

	return 1;

}

int lua_destroy_all_sounds(lua_State *lua_env)
{

	fbl_destroy_all_sounds();

	return 1;

}

int lua_load_music(lua_State *lua_env)
{

	char *music_file = (char *)lua_tostring(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_load_music(music_file));

	return 1;

}

int lua_destroy_music(lua_State *lua_env)
{

	fbl_destroy_music();

	return 1;

}

int lua_play_music(lua_State *lua_env)
{

	int loops = (int)lua_tonumber(lua_env, 1);

	fbl_play_music(loops);

	return 1;

}

int lua_pause_music(lua_State *lua_env)
{

	fbl_pause_music();

	return 1;

}

int lua_resume_music(lua_State *lua_env)
{

	fbl_resume_music();

	return 1;

}

int lua_stop_music(lua_State *lua_env)
{

	fbl_stop_music();

	return 1;

}

/* input.c */

int lua_get_key_down(lua_State *lua_env)
{

	int key = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_key_down(key));

	return 1;

}

int lua_get_key_up(lua_State *lua_env)
{

	int key = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_key_up(key));

	return 1;

}

int lua_get_mouse_click(lua_State *lua_env)
{

	int button = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_click(button));

	return 1;

}

int lua_get_mouse_release(lua_State *lua_env)
{

	int button = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_release(button));

	return 1;

}

int lua_get_mouse_x(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_x());

	return 1;

}

int lua_get_mouse_y(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_y());

	return 1;

}

int lua_get_mouse_logical_x(lua_State* lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_logical_x());

	return 1;

}

int lua_get_mouse_logical_y(lua_State* lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_get_mouse_logical_y());

	return 1;

}

/* pathfind */

int lua_pathf_init(lua_State *lua_env)
{

	fbl_pathf_init();

	return 1;

}

int lua_pathf_shutdown(lua_State *lua_env)
{

	fbl_pathf_shutdown();

	return 1;

}

int lua_pathf_get_map_w(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_map_w());

	return 1;

}

int lua_pathf_get_map_h(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_map_h());

	return 1;

}

int lua_pathf_get_tile_size(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_tile_size());

	return 1;

}

int lua_pathf_get_num_chars(lua_State *lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_num_chars());

	return 1;

}

int lua_pathf_set_walkability(lua_State *lua_env)
{

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);
	int value = (int)lua_tonumber(lua_env, 3);

	fbl_pathf_set_walkability(x, y, value);

	return 1;

}

int lua_pathf_get_walkability(lua_State *lua_env)
{

	int x = (int)lua_tonumber(lua_env, 1);
	int y = (int)lua_tonumber(lua_env, 2);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_walkability(x, y));

	return 1;

}

int lua_pathf_find_path(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);
	int starting_x = (int)lua_tonumber(lua_env, 2);
	int starting_y = (int)lua_tonumber(lua_env, 3);
	int target_x = (int)lua_tonumber(lua_env, 4);
	int target_y = (int)lua_tonumber(lua_env, 5);
	int diagonals = (int)lua_tonumber(lua_env, 6);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_find_path(path_id, starting_x, starting_y, target_x, target_y, diagonals));

	return 1;

}

int lua_pathf_set_path_status(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);
	int value = (int)lua_tonumber(lua_env, 2);

	fbl_pathf_set_path_status(path_id, value);

	return 1;

}

int lua_pathf_get_path_status(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_path_status(path_id));

	return 1;

}

int lua_pathf_get_path_length(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_path_length(path_id));

	return 1;

}

int lua_pathf_get_path_location(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_path_location(path_id));

	return 1;

}

int lua_pathf_get_x_path(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_x_path(path_id));

	return 1;

}

int lua_pathf_get_y_path(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_get_y_path(path_id));

	return 1;

}

int lua_pathf_read_path(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);
	int current_x = (int)lua_tonumber(lua_env, 2);
	int current_y = (int)lua_tonumber(lua_env, 3);
	int pixels_per_frame = (int)lua_tonumber(lua_env, 4);

	fbl_pathf_read_path(path_id, current_x, current_y, pixels_per_frame);

	return 1;

}

int lua_pathf_read_path_x(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);
	int path_location = (int)lua_tonumber(lua_env, 2);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_read_path_x(path_id, path_location));

	return 1;

}

int lua_pathf_read_path_y(lua_State *lua_env)
{

	int path_id = (int)lua_tonumber(lua_env, 1);
	int path_location = (int)lua_tonumber(lua_env, 2);

	lua_pushnumber(lua_env, (lua_Number)fbl_pathf_read_path_y(path_id, path_location));

	return 1;

}


/* physics.c */

int lua_phys_init(lua_State *lua_env)
{

	fbl_phys_init();

	return 1;

}
int lua_phys_shutdown(lua_State *lua_env)
{

	fbl_phys_shutdown();

	return 1;

}


/* particles */

int lua_create_emitter(lua_State *lua_env)
{

	//int w = (int)lua_tonumber(lua_env, 1);
	//int h = (int)lua_tonumber(lua_env, 2);

	int num = (int)lua_tonumber(lua_env, 1);

	//int life = (int)lua_tonumber(lua_env, 4);
	//int rate = (int)lua_tonumber(lua_env, 5);
	//int density = (int)lua_tonumber(lua_env, 6);
	//float scale_start = (float)lua_tonumber(lua_env, 7);
	//float scale_end = (float)lua_tonumber(lua_env, 8);

	fbl_create_emitter(num);

	return 1;

}


int lua_delete_emitter(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	fbl_delete_emitter(id);

	return 1;

}


int lua_set_emitter_active(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int active = (int)lua_tonumber(lua_env, 2);

	fbl_set_emitter_active(id, active);

	return 1;

}

int lua_set_emitter_xy(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int x = (int)lua_tonumber(lua_env, 2);
	int y = (int)lua_tonumber(lua_env, 3);

	fbl_set_emitter_xy(id, x, y);

	return 1;

}

int lua_set_emitter_vel_xy(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	float vel_x = (float)lua_tonumber(lua_env, 2);
	float vel_y = (float)lua_tonumber(lua_env, 3);
	int start = (int)lua_tonumber(lua_env, 4);

	fbl_set_emitter_vel_xy(id, vel_x, vel_y, start);

	return 1;

}

int lua_set_emitter_color(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int r = (int)lua_tonumber(lua_env, 2);
	int g = (int)lua_tonumber(lua_env, 3);
	int b = (int)lua_tonumber(lua_env, 4);
	int a = (int)lua_tonumber(lua_env, 5);
	int start = (int)lua_tonumber(lua_env, 6);

	fbl_set_emitter_color(id, r, g, b, a, start);

	return 1;

}

int lua_fix_emitter_to_screen(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);
	int fix = (int)lua_tonumber(lua_env, 2);

	fbl_fix_emitter_to_screen(id, fix);
	
	return 1;

}

int lua_get_num_active_particles(lua_State* lua_env)
{

	int id = (int)lua_tonumber(lua_env, 1);

	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_active_particles(id));

	return 1;

}

int lua_get_num_emitters(lua_State* lua_env)
{


	lua_pushnumber(lua_env, (lua_Number)fbl_get_num_emitters());

	return 1;

}

int lua_destroy_all_emitters(lua_State* lua_env)
{

	fbl_destroy_all_emitters();

	return 1;

}


/* threadpool */

int lua_create_threadpool(lua_State* lua_env)
{

	lua_pushnumber(lua_env, (lua_Number)fbl_create_threadpool());

	return 1;

}

int lua_destroy_threadpool(lua_State* lua_env)
{


	fbl_destroy_threadpool();

	return 1;

}

#endif

/* end of lua.c */
