--
--
--	FBL ENGINE
--
--	fbl.lua
--
--	Contains constants and function definitions for fbl
--
--	Hans Strömquist 2021
--
--

-- fbl bool

fbl_true	= 1
fbl_false	= 0

-- change this to where you store your emscripten assets (if you're using emscripten :) only nessecary if used in main.lua)

PATH_TO_EMSCRIPTEN_ASSETS = "fbl/project_emscripten/output/"

-- modes for fbl_set_window_mode() 0, 1 and 4097 respectively (FBL_WINDOW_FULLSCREEN | 0x00001000) == 4097

FBL_WINDOWED = 0
FBL_WINDOW_FULLSCREEN = 1
FBL_WINDOW_FULLSCREEN_DESKTOP = 4097


-- FBL_BLENDMODES

	FBL_BLENDMODE_NONE  = 0
	FBL_BLENDMODE_BLEND = 1
	FBL_BLENDMODE_ADD   = 2
	FBL_BLENDMODE_MOD   = 3


-- FBL_KEYS


	FBLK_UP		= 0
	FBLK_DOWN	= 1
	FBLK_LEFT	= 2
	FBLK_RIGHT	= 3

	FBLK_SPACE		= 4
	FBLK_RETURN		= 5
	FBLK_ESCAPE		= 6
	FBLK_BACKSPACE	= 7

	FBLK_LCTRL		= 8
	FBLK_LSHIFT		= 9
	FBLK_LALT		= 10
	FBLK_TAB		= 11

	FBLK_RCTRL		= 12
	FBLK_RSHIFT		= 13
	FBLK_RALT		= 14

	FBLK_INSERT		= 15
	FBLK_DELETE		= 16
	FBLK_HOME		= 17
	FBLK_END		= 18
	FBLK_PGUP		= 19
	FBLK_PGDOWN		= 20

	FBLK_A	= 21
	FBLK_B	= 22
	FBLK_C	= 23
	FBLK_D	= 24
	FBLK_E	= 25
	FBLK_F	= 26
	FBLK_G	= 27
	FBLK_H	= 28
	FBLK_I	= 29
	FBLK_J	= 30
	FBLK_K	= 31
	FBLK_L	= 32
	FBLK_M	= 33
	FBLK_N	= 34
	FBLK_O	= 35
	FBLK_P	= 36
	FBLK_Q	= 37
	FBLK_R	= 38
	FBLK_S	= 39
	FBLK_T	= 40
	FBLK_U	= 41
	FBLK_V	= 42
	FBLK_W	= 43
	FBLK_X	= 44
	FBLK_Y	= 45
	FBLK_Z	= 46

	FBLK_0	= 47
	FBLK_1	= 48
	FBLK_2	= 49
	FBLK_3	= 50
	FBLK_4	= 51
	FBLK_5	= 52
	FBLK_6	= 53
	FBLK_7	= 54
	FBLK_8	= 55
	FBLK_9	= 56

	FBLK_F1	= 57
	FBLK_F2	= 58
	FBLK_F3 = 59
	FBLK_F4 = 60
	FBLK_F5 = 61
	FBLK_F6 = 62
	FBLK_F7 = 63
	FBLK_F8	= 64
	FBLK_F9	= 65
	FBLK_F10	= 66
	FBLK_F11	= 67
	FBLK_F12	= 68


-- FBL_MOUSE_BUTTONS

	FBLMB_LEFT	= 0
	FBLMB_RIGHT	= 1


-- FBL_PRIM_TYPES

	FBL_POINT = 0
	FBL_LINE = 1
	FBL_TRI = 2
	FBL_RECT = 3
	FBL_PENTA = 4  --/* not impl. yet */
	FBL_HEXA = 5   --/* not impl. yet */
	FBL_CIRCLE = 6

	FBL_RAY = 7 --/* raycasting */

	FBL_NORMAL_RECT = 8 --/* rectfill drawn from top left without X2 size */
	FBL_NO_PRIM = 9	 --/* no prim can be used by the particle emitter so it uses sprite instead of prim */
	FBL_RECT_PHYS = 10	 --/* only to be used when creating physics-sprites (slightly smaller hitbox, 2px each direction) */


-- FBL_TEXT_ALIGN

	FBL_ALIGN_LEFT		= 0
	FBL_ALIGN_CENTER	= 1
	FBL_ALIGN_RIGHT		= 2


--  sprite alignment, what point sprite gets draw from

-- FBL_SPRITE_ALIGN

	FBL_SPRITE_ALIGN_UP_LEFT	= 0
	FBL_SPRITE_ALIGN_CENTER		= 1
	FBL_SPRITE_ALIGN_DOWN_LEFT	= 2


-- FBL_PHYS_BODY_TYPES
    
    FBL_PHYS_DYNAMIC	= 0
    FBL_PHYS_KINEMATIC	= 1
    -- FBL_PHYS_STATIC	= 2 /* not impl. yet */
    -- FBL_PHYS_SENSOR	= 3 /* not impl. yet */


-- FBL_UI_ELEM_TYPES

	FBL_UI_BUTTON_CLICK		 = 0
	FBL_UI_BUTTON_HOLD		 = 1
	FBL_UI_BUTTON_INTERVAL	 = 2
	FBL_UI_CHECKBOX			 = 3
	FBL_UI_CHECKBOX_INTERVAL = 4
	-- FBL_UI_SLIDER,  /* not impl. yet */
	-- FBL_UI_EDITBOX  /* not impl. yet */
	FBL_UI_DRAGGABLE		 = 7


-- astar pathfinding

FBL_PATHF_NOT_STARTED	= 0
FBL_PATHF_FOUND			= 1
FBL_PATHF_NON_EXISTENT	= 2
FBL_PATHF_WALKABLE		= 0
FBL_PATHF_UNWALKABLE	= 1
FBL_PATHF_USE_DIAG		= 1
FBL_PATHF_NO_DIAG		= 0
FBL_PATHF_OUT_OF_MEM	= 3


-- sprite sorting

FBL_SORT_BY_LAYER	= 0
FBL_SORT_BY_BLEND	= 1

--[[

All function declarations in fbl:

/* these three functions has to be in the Lua game-project */

void fbl_start(void);
void fbl_game_loop(void);
void fbl_end(void);


/* fbl.c */

int  fbl_engine_init(int w, int h, int fps);
int  fbl_set_window_mode(unsigned int mode);
void fbl_set_assets_folder_name(const char *name);		(normally you don't need to use this from Lua)
void fbl_log(const char *msg, ...);
void fbl_set_clear_color(int r, int g, int b, int a);
void fbl_set_screen_wh(int w, int h);
int  fbl_get_screen_w(void);
int  fbl_get_screen_h(void);
int  fbl_get_device_res(int* w, int* h);
int  fbl_set_render_scale(float scale_x, float scale_y);
int  fbl_set_viewport(int x, int y, int w, int h);
void fbl_get_viewport(int *x, int *y, int *w, int *h);
int  fbl_set_render_logical_size(int x, int y);
void fbl_set_fps(int fps);
int  fbl_get_fps(void);
unsigned int fbl_get_raw_frames_count(void);
void fbl_set_system_delay_ms(int delay);
int  fbl_get_system_delay_ms(void);
void fbl_set_fps_locked(bool locked);
bool fbl_get_fps_locked(void);
unsigned int fbl_timer_get_ticks(void);
void fbl_set_camera_xy(int x, int y);
int  fbl_get_camera_x(void);
int  fbl_get_camera_y(void);
void fbl_set_window_title(const char *title);
const char *fbl_get_version(void);
const char *fbl_get_platform(void);
const char *fbl_get_renderer(void);
void fbl_engine_quit(void);


/* sprite.c */

int    fbl_load_texture(const char *img_file);
int    fbl_get_texture_w(void);
int    fbl_get_texture_h(void);
void   fbl_destroy_texture(void);
int    fbl_create_sprite(int x, int y, int w, int h, int r);
void   fbl_delete_sprite(int id);
void   fbl_set_sprite_image(int id, int x, int y, int w, int h, int r);
void   fbl_set_sprite_align(int mode);
void   fbl_set_lighting_tint(bool on_off, uint8_t r, uint8_t g, uint8_t b);
void   fbl_set_sprite_is_light(int id, bool light);
void   fbl_set_sprite_blendmode(int id, int mode);
int    fbl_get_sprite_blendmode(int id);
void   fbl_set_sprite_alpha(int id, uint8_t alpha);
int    fbl_get_sprite_alpha(int id);
void   fbl_set_sprite_color(int id, uint8_t r, uint8_t g, uint8_t b);
void   fbl_get_sprite_color(int id, uint8_t *r, uint8_t *g, uint8_t *b);
void   fbl_set_sprite_layer(int id, int layer);
int    fbl_get_sprite_layer(int id);
void   fbl_set_sprite_xy(int id, int x, int y);
int    fbl_get_sprite_x(int id);
int    fbl_get_sprite_y(int id);
void   fbl_set_sprite_angle(int id, double angle);
double fbl_get_sprite_angle(int id);
void   fbl_set_sprite_scale(int id, float scale);
float  fbl_get_sprite_scale(int id);
void   fbl_set_sprite_flip(int id, int flipmode);
int    fbl_get_sprite_flip(int id);
void   fbl_set_sprite_animation(int id, bool on, int x, int y, int w, int h, int num_frames, int speed, bool loop);
void   fbl_fix_sprite_to_screen(int id, bool fix);
bool   fbl_get_sprite_collision(int id_1, int id_2);
void   fbl_set_sprite_phys(int id, bool on, int type, int body_type, bool can_rotate);
void   fbl_set_sprite_active(int id, bool active);
bool   fbl_get_sprite_active(int id);
int    fbl_get_num_sprites(void);
void   fbl_sort_sprites(int mode);
void   fbl_destroy_all_sprites(void);


/* primitives.c */

int    fbl_create_prim(uint8_t type, int x, int y, int w, int h, int r, bool thick, bool fill);
void   fbl_delete_prim(int id);
void   fbl_set_prim_color(int id, int r, int g, int b, int a);
void   fbl_set_prim_size(int id, int w, int h, int r);
void   fbl_set_prim_xy(int id, int x, int y);
int    fbl_get_prim_x(int id);
int    fbl_get_prim_y(int id);
void   fbl_set_prim_angle(int id, double angle);
double fbl_get_prim_angle(int id);
void   fbl_fix_prim_to_screen(int id, bool fix);
bool   fbl_get_prim_collision(int id_1, int id_2);
void   fbl_get_ray_hit_sprite(int id, int *hit_id, int *x, int *y);
void   fbl_set_prim_phys(int id, bool on, int body_type, bool can_rotate);
void   fbl_set_prim_active(int id, bool active);
bool   fbl_get_prim_active(int id);
int    fbl_get_num_prims(void);
void   fbl_destroy_all_prims(void);


/* text.c */

int    fbl_load_ttf_font(const char *font_file, int size);
void   fbl_destroy_ttf_font(void);
int    fbl_create_text(int r, int g, int b, int a, char *format, ...);
int    fbl_update_text(int id, int r, int g, int b, int a, char *format, ...);
void   fbl_delete_text(int id);
void   fbl_set_text_xy(int id, int x, int y);
int    fbl_get_text_x(int id);
int    fbl_get_text_y(int id);
void   fbl_set_text_scale(int id, float scale);
float  fbl_get_text_scale(int id);
void   fbl_set_text_align(int id, uint8_t align);
void   fbl_fix_text_to_screen(int id, bool fix);
void   fbl_set_text_active(int id, bool active);
bool   fbl_get_text_active(int id);
int    fbl_get_num_text_objects(void);
void   fbl_destroy_all_text_objects(void);


/* ui.c */

int    fbl_load_ui_texture(const char *img_file);
void   fbl_destroy_ui_texture(void);
int    fbl_create_ui_elem(uint8_t type, int x, int y, int w, int h, int(*func)(int, int));
void   fbl_delete_ui_elem(int id);
void   fbl_set_ui_elem_xy(int id, int x, int y);
int    fbl_get_ui_elem_x(int id);
int    fbl_get_ui_elem_y(int id);
void   fbl_set_ui_elem_color(int id, uint8_t r, uint8_t g, uint8_t b);
int    fbl_get_ui_elem_val(int id);
void   fbl_set_ui_elem_val(int id, int value);
void   fbl_set_ui_elem_access(int id, int frames);
void   fbl_set_ui_elem_access_left(int id, int frames);
void   fbl_set_ui_elem_active(int id, bool active);
bool   fbl_get_ui_elem_active(int id);
int    fbl_get_num_ui_elems(void);
void   fbl_destroy_all_ui_elems(void);


/* sound.c */

int  fbl_load_sound(const char *sample_file);
void fbl_delete_sound(int id);
void fbl_play_sound(int id, int channel, int loops);
void fbl_set_sound_volume(int id, int volume); // max volume is 128
void fbl_pause_sound(int id);
void fbl_resume_sound(int id);
void fbl_destroy_all_sounds(void);

int  fbl_load_music(const char *song_file);
void fbl_destroy_music(void);
void fbl_play_music(int volume, int loops); // max volume is 128
void fbl_pause_music(void);
void fbl_resume_music(void);
void fbl_stop_music(void);


/* input.c */

bool fbl_get_key_down(int key);
bool fbl_get_key_up(int key);
bool fbl_get_mouse_click(int button);
bool fbl_get_mouse_release(int button);
int  fbl_get_mouse_x(void);
int  fbl_get_mouse_y(void);
int fbl_get_mouse_logical_x(void);
int fbl_get_mouse_logical_y(void);


/* lua.c */

int  fbl_lua_init(const char* file);
void fbl_lua_shutdown(void);
void fbl_lua_do_file(const char *file);


/* pathfind.c */

int  fbl_pathf_init(void);
void fbl_pathf_shutdown(void);
int  fbl_pathf_get_map_w(void);
int  fbl_pathf_get_map_h(void);
int  fbl_pathf_get_tile_size(void);
int  fbl_pathf_get_num_chars(void);
void fbl_pathf_set_walkability(int x, int y, int value);
char fbl_pathf_get_walkability(int x, int y);
int  fbl_pathf_find_path(int path_id, int starting_x, int starting_y, int target_x, int target_y, int diagonals);
void fbl_pathf_set_path_status(int path_id, int value);
int  fbl_pathf_get_path_status(int path_id);
int  fbl_pathf_get_path_length(int path_id);
int  fbl_pathf_get_path_location(int path_id);
int  fbl_pathf_get_x_path(int path_id);
int  fbl_pathf_get_y_path(int path_id);
void fbl_pathf_read_path(int path_id, int current_x, int current_y, int pixels_per_frame);
int  fbl_pathf_read_path_x(int path_id, int path_location);
int  fbl_pathf_read_path_y(int path_id, int path_location);


/* physics.c */

int  fbl_phys_init(void);
void fbl_phys_shutdown(void);
void fbl_phys_set_gravity(int x, int y);

/* particles */

int  fbl_create_emitter(int num_particles);
void fbl_delete_emitter(int id);
void fbl_set_emitter_active(int id, bool active);
void fbl_set_emitter_xy(int id, int x, int y);
void fbl_set_emitter_vel_xy(int id, float vel_x, float vel_y, bool start);
void fbl_set_emitter_color(int id, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool start);
void fbl_set_emitter_particle_shape(int id, int shape, int x, int y, int w, int h);
void fbl_set_emitter_params(int id, int type, int spawn_w, int spawn_h, int life, int rate, int density, float scale_start, float scale_end);
void fbl_fix_emitter_to_screen(int id, bool fix);
int  fbl_get_num_active_particles(int emitter_id);
int  fbl_get_num_emitters(void);
void fbl_destroy_all_emitters(void);


/* thread pool */

int  fbl_create_threadpool(void);
void fbl_destroy_threadpool(void);

--]]
