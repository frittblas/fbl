/*
*
*	FBL ENGINE
*
*	fbl_test.c
*
*	Contains some tests for fbl. Learn how the API works.
*	Cycle through the demos with F1 and F2. Reload the Lua demo with F5 after making changes to assets/main.lua (or whatever filename you choose)
*
*	Hans Strömquist 2017 - 2022
*
*/

#include "../src/fbl.h"

#include <math.h>	// for sin and cos in one of the demos

#define NUM_DEMOS 8

#define LOGICAL_RES_X 960
#define LOGICAL_RES_Y 540

/* for timing */

unsigned int start_time;

int x = 10;
int y = 10;
int flag = 0;
double angle = 0.0;

int left_button = 0;
int right_button = 0;

void setup_demo_1(void); // fonts, sprites, camera, movement, input (redo this demo!)
void run_demo_1(void);
void setup_demo_2(void); // ui, prims, physics, sounds, music
void run_demo_2(void);
void setup_demo_3(void); // static lines w. collision and lots of dynamic bodies, mouse position.
void run_demo_3(void);
void setup_demo_4(void); // pathfinding, 2d lights
void run_demo_4(void);
void setup_demo_5(void); // lua, edit main.lua and reload with f5
void run_demo_5(void);
void setup_demo_6(void); // benchmark, blendmodes
void run_demo_6(void);
void setup_demo_7(void); // particles
void run_demo_7(void);
void setup_demo_8(void); // raycasting
void run_demo_8(void);

void cycle_demos(void);
void check_reload_lua(void);
void unload_all(void);
int  pressed_button(int x, int y);	// callback function for ui-click


/* demo-setup and demo-loop */

void(*demo_setup[NUM_DEMOS])();
void(*demo_run[NUM_DEMOS])();

int cur_demo = 3;

void fbl_start()
{

	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");


	fbl_engine_init(1280, 720, 60);
	fbl_set_render_logical_size(LOGICAL_RES_X, LOGICAL_RES_Y);
	fbl_set_clear_color(168, 230, 255, 255);
	//fbl_set_clear_color(0, 0, 0, 255);
	printf("FBL version: %s, running on %s, and %s\n", fbl_get_version(), fbl_get_platform(), fbl_get_renderer());

	demo_setup[0] = setup_demo_1;
	demo_setup[1] = setup_demo_2;
	demo_setup[2] = setup_demo_3;
	demo_setup[3] = setup_demo_4;
	demo_setup[4] = setup_demo_5;
	demo_setup[5] = setup_demo_6;
	demo_setup[6] = setup_demo_7;
	demo_setup[7] = setup_demo_8;

	demo_run[0] = run_demo_1;
	demo_run[1] = run_demo_2;
	demo_run[2] = run_demo_3;
	demo_run[3] = run_demo_4;
	demo_run[4] = run_demo_5;
	demo_run[5] = run_demo_6;
	demo_run[6] = run_demo_7;
	demo_run[7] = run_demo_8;

	/* ui */

	fbl_load_ui_texture("ui.png");

	left_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);
	right_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);

	fbl_set_ui_elem_xy(left_button, 64, fbl_get_screen_h() - 64);
	fbl_set_ui_elem_xy(right_button, fbl_get_screen_w() - 64, fbl_get_screen_h() - 64);


	(*demo_setup[cur_demo])();

	//fbl_set_fps_locked(false);
	//fbl_set_system_delay_ms(0);

	//fbl_create_threadpool();

}

void fbl_game_loop()
{

	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9)) {
		fbl_set_window_mode(0);
	}
	if (fbl_get_key_down(FBLK_F10)) {
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
		//fbl_set_render_scale(2.0, 2.0);
	}
	if (fbl_get_key_down(FBLK_F11)) {

		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);
		
		//fbl_set_viewport(0, 0, fbl_get_screen_w(), fbl_get_screen_w());
		//fbl_set_render_logical_size(fbl_get_screen_w(), fbl_get_screen_w());
		//fbl_set_render_scale(2.0, 2.0);
	}

	cycle_demos();

	(*demo_run[cur_demo])();

}

void fbl_end()
{

	//fbl_destroy_threadpool();

	printf("bye!");

}


/***************************************************************************************************/

/* Demo 1 */


void setup_demo_1()
{

	fbl_set_window_title("Demo 1: fonts, animated sprites, input, camera and collision.");


	fbl_load_texture("spritesheet.png");
	fbl_create_sprite(0, 0, 32, 64, 0);
	fbl_create_sprite(32, 0, 32, 64, 0);
	fbl_create_sprite(0, 0, 32, 64, 0);
	fbl_create_sprite(0, 0, 32, 64, 0);
	fbl_create_sprite(0, 0, 32, 64, 0);

	fbl_fix_sprite_to_screen(0, true);

	printf("num sprites = %d\n", fbl_get_num_sprites());

	fbl_load_ttf_font("edosz.ttf", 30);
	fbl_create_text(0, 255, 0, 255, "fps = %d", x);
	fbl_create_text(0, 0, 255, 255, "delay =  %d", x);
	fbl_create_text(255, 0, 0, 255, "even betaa in red %d", x);

	fbl_fix_text_to_screen(0, false);

	fbl_set_text_xy(0, 0, 100);
	fbl_set_text_xy(1, 0, 200);
	fbl_set_text_xy(2, 0, 300);

	printf("num text objects = %d\n", fbl_get_num_text_objects());


}

void create_100_sprites()
{

	int lol = 0;

	for (int i = 0; i < 100; i++) {

		lol = fbl_create_sprite(0, 0, 32, 64, 0);
		fbl_set_sprite_xy(lol, fbl_get_mouse_x() + fbl_get_camera_x(), fbl_get_mouse_y() + fbl_get_camera_y());

		//fbl_set_sprite_animation(lol, true, 4, 24, false);
		fbl_set_sprite_animation(lol, true, 0, 0, 32, 64, 4, 24, true);

		if (lol % 10 == 0)
			printf("num sprites = %d\n", fbl_get_num_sprites());

	}

}

void run_demo_1()
{

	fbl_set_sprite_xy(0, x, y);
	fbl_set_sprite_xy(1, x, y + 64);

	fbl_set_sprite_xy(3, x, y + 184);
	fbl_set_sprite_xy(4, x, y + 240);

	fbl_set_text_xy(2, x, 300);

	x++;

	if (x > 300) x = 301;

	if (x == 200) {
		//fbl_delete_sprite(4); // nothing happens as it should ??????????? what does this mean lol just move on
		fbl_set_sprite_active(1, false);
		fbl_set_sprite_flip(4, 3);
		printf("num sprites = %d\n", fbl_get_num_sprites());
		printf("num text objs = %d\n", fbl_get_num_text_objects());
		fbl_set_sprite_angle(4, 90);
		fbl_set_sprite_scale(4, 3);
	}
	if (x == 300) {

		fbl_set_sprite_active(1, true);
		fbl_set_sprite_flip(4, 2);
		fbl_set_sprite_angle(4, 0);
		fbl_set_sprite_scale(4, 1);
		printf("num sprites = %d\n", fbl_get_num_sprites());
		printf("num text objs = %d\n", fbl_get_num_text_objects());

	}


	if (x % 3 == 0) {
		fbl_update_text(0, 0, 255, 0, 255, "fps = %d", fbl_get_fps());
		fbl_update_text(1, 0, 0, 255, 255, "delay = %d", fbl_get_system_delay_ms());
	}

	static int loly = 0;

	if (fbl_get_key_down(FBLK_SPACE) && loly == 0)
	{

		loly = 1;
		fbl_submit_task_tp(create_100_sprites);

	}

	if (fbl_get_key_up(FBLK_SPACE)) loly = 0;

	if (fbl_get_key_down(FBLK_RETURN) && fbl_get_fps_locked())
	{

		fbl_set_fps_locked(false);
		printf("fps unlocked\n");

		//fbl_lua_do_file("main_.lua");

		fbl_set_sprite_layer(2, 1);
		fbl_sort_sprites(FBL_SORT_BY_LAYER);

	}

	if (fbl_get_key_down(FBLK_B) && !fbl_get_fps_locked())
	{

		fbl_set_fps_locked(true);
		printf("fps locked\n");

	}


	/* move camera */

	if (fbl_get_key_down(FBLK_UP)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - 2);
	else if (fbl_get_key_down(FBLK_DOWN)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + 2);
	else if (fbl_get_key_down(FBLK_LEFT)) fbl_set_camera_xy(fbl_get_camera_x() - 2, fbl_get_camera_y());
	else if (fbl_get_key_down(FBLK_RIGHT)) fbl_set_camera_xy(fbl_get_camera_x() + 2, fbl_get_camera_y());

	/* move a sprite with wasd */

	if (fbl_get_key_down(FBLK_W)) fbl_set_sprite_xy(2, fbl_get_sprite_x(2), fbl_get_sprite_y(2) - 2);
	if (fbl_get_key_down(FBLK_S)) fbl_set_sprite_xy(2, fbl_get_sprite_x(2), fbl_get_sprite_y(2) + 2);
	if (fbl_get_key_down(FBLK_A)) fbl_set_sprite_xy(2, fbl_get_sprite_x(2) - 2, fbl_get_sprite_y(2));
	if (fbl_get_key_down(FBLK_D)) fbl_set_sprite_xy(2, fbl_get_sprite_x(2) + 2, fbl_get_sprite_y(2));


	if (fbl_get_sprite_collision(1, 2)) printf("hit!\n");

	// check this out!!!, key_up event keeps triggering

	if(fbl_get_key_up(FBLK_RETURN)) printf("up\n");



	//fbl_set_sprite_angle(2, angle);
	//angle += 1.3;


}

/***************************************************************************************************/

/* Demo 2 */

int sound_button, music_button;
int line[3];
int circle, triangle, square;
int ping_sound;

void setup_demo_2(void)
{

	fbl_set_window_title("Demo 2: ui, primitives, sounds and stuff.");

	fbl_phys_init();

	fbl_load_texture("ui.png");
	fbl_create_sprite(0, 0, 32, 32, 16);
	fbl_create_sprite(0, 0, 32, 32, 16);

	/* generally, store the id somewhere */
	
	fbl_set_sprite_phys(0, true, FBL_CIRCLE, FBL_PHYS_DYNAMIC, true);	// circle physics shape
	fbl_set_sprite_phys(1, true, FBL_CIRCLE, FBL_PHYS_DYNAMIC, true);

	fbl_set_sprite_xy(0, 100, 0);
	fbl_set_sprite_xy(1, 300, 300);


	/* create some ui elements */

	//fbl_load_ui_texture("ui.png");
	sound_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 32, 32, 32, /*pressed_button*/NULL); // if NULL instead of function pointer, check fbl_get_ui_elem_val()
	fbl_set_ui_elem_xy(sound_button, 100, 100);
	music_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 32, 32, 32, /*pressed_button*/NULL); // if NULL instead of function pointer, check fbl_get_ui_elem_val()
	fbl_set_ui_elem_xy(music_button, 200, 100);
	

	/* write something */

	fbl_load_ttf_font("edosz.ttf", 30);
	fbl_create_text(255, 0, 255, 0, "click on some things");
	fbl_set_text_xy(0, 400, 400);
	fbl_set_text_align(0, FBL_ALIGN_RIGHT);
	
	/* create lines with static colission */

	line[0] = fbl_create_prim(FBL_LINE, 10, 300, 550, 360, 0, true, false);
	fbl_set_prim_color(line[0], 255, 0, 0, 50);
	fbl_set_prim_phys(line[0], true, FBL_PHYS_STATIC, false);

	line[1] = fbl_create_prim(FBL_LINE, 600, 10, 400, 500, 0, false, false);
	fbl_set_prim_phys(line[1], true, FBL_PHYS_STATIC, false);

	line[2] = fbl_create_prim(FBL_LINE, 10, 10, 10, 500, 0, false, false);
	fbl_set_prim_phys(line[2], true, FBL_PHYS_STATIC, false);

	/* and some primitives */

	circle = fbl_create_prim(FBL_CIRCLE, 200, 200, 0, 0, 40, false, false);
	fbl_set_prim_phys(circle, true, FBL_PHYS_DYNAMIC, true);

	triangle = fbl_create_prim(FBL_TRI, 300, 200, 24, 12, 0, true, false);

	square = fbl_create_prim(FBL_RECT, 400, 200, 32, 32, 0, true, false);


	fbl_create_prim(FBL_POINT, 400, 200, 0, 0, 40, true, false); // no need to store id for measly pixel :(


	ping_sound = fbl_load_sound("door.ogg"); 

	fbl_load_music("song_99.ogg");	// only one piece of music is loaded at a time

	x = 0;

}

void run_demo_2()
{

	static bool pressed;
	static float angle = 0.0;

	if (fbl_get_mouse_release(FBLMB_LEFT) && !pressed)
	{

		int lol = 0;

		lol = fbl_create_sprite(0, 0, 32, 32, 16);
		fbl_set_sprite_phys(lol, true, FBL_RECT, FBL_PHYS_DYNAMIC, true);
		fbl_set_sprite_animation(lol, true, 0, 0, 32, 32, 4, 24, false);
		fbl_set_sprite_xy(lol, fbl_get_mouse_x() + fbl_get_camera_x(), fbl_get_mouse_y() + fbl_get_camera_y());

		if (lol % 10 == 0)
			printf("num sprites = %d\n", fbl_get_num_sprites());

		pressed = true;

	}

	if (fbl_get_mouse_click(FBLMB_LEFT))
	{
		pressed = false;
	}

	if (fbl_get_mouse_click(FBLMB_RIGHT))
	{
		angle += 1.0;
		if (angle > 360.0) angle = 0.0;
		fbl_set_prim_angle(triangle, angle);
	}


	if (x == 1000) fbl_set_prim_phys(square, true, FBL_PHYS_DYNAMIC, true);

	/* take care of ui button clicks */

	if (fbl_get_ui_elem_val(sound_button)) {
		printf("ui button pressed (not function pointer)\n");
		fbl_play_sound(ping_sound);
	}
	if (fbl_get_ui_elem_val(music_button)) {
		printf("ui button pressed (not function pointer)\n");
		fbl_play_music(1);	// play the loaded song 1 time
	}

	x++;

}

/***************************************************************************************************/

/* Demo 3 */

void setup_demo_3()
{

	int i, lol;

	fbl_set_window_title("Demo 3: Lots of bodies and mouse position.");

	fbl_phys_init();

	fbl_load_texture("spritesheet.png");

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);

	/*
	lol = fbl_create_sprite(0, 0, 32, 64);
	fbl_set_sprite_xy(lol, 100, 100);

	lol = fbl_create_sprite(0, 0, 32, 64);
	fbl_set_sprite_xy(lol, 200, 200);
	*/


	fbl_create_prim(FBL_LINE, 10, 300, 630, 360, 0, false, false);
	fbl_set_prim_color(0, 255, 0, 0, 255);
	fbl_set_prim_phys(0, true, FBL_PHYS_STATIC, false);

	fbl_create_prim(FBL_LINE, 650, 10, 630, 360, 0, false, false);
	fbl_set_prim_color(1, 255, 0, 0, 255);
	fbl_set_prim_phys(1, true, FBL_PHYS_STATIC, false);

	fbl_create_prim(FBL_LINE, 1, 10, 1, 360, 0, false, false);
	fbl_set_prim_color(2, 255, 0, 0, 255);
	fbl_set_prim_phys(2, true, FBL_PHYS_STATIC, false);


	fbl_create_sprite(0, 0, 20, 20, 0);
	fbl_set_sprite_xy(0, fbl_get_screen_w() / 2, fbl_get_screen_h() / 2 - 64);


	/* create 80 boxes with physics */

	for (i = 1; i < 80; i++) {

		//lol = fbl_create_prim(FBL_CIRCLE, rand() % 660, rand() % 330, 1, 360, rand() % 30 + 10, false, false);
		//fbl_set_prim_color(lol, 0, 0, 255, 50);

		lol = fbl_create_sprite(0, 0, 20, 20, 0);
		fbl_set_sprite_xy(lol, rand() % 600 + 20, rand() % 300);
		if(lol > 70)
			fbl_set_sprite_phys(lol, true, FBL_RECT, FBL_PHYS_KINEMATIC, true); 
		else
			fbl_set_sprite_phys(lol, true, FBL_RECT, FBL_PHYS_DYNAMIC, true);
	}


	/*
	fbl_set_fps_locked(false);
	fbl_set_system_delay_ms(0);
	*/


	//fbl_pathf_init();


	flag = 1;

	start_time = fbl_timer_get_ticks();

}


void run_demo_3()
{


	if (fbl_get_mouse_click(FBLMB_LEFT))
	{

		fbl_set_sprite_xy(0, fbl_get_mouse_x() + fbl_get_camera_x(), fbl_get_mouse_y() + fbl_get_camera_y());
		printf("mouse clicked at = %d, %d\n", fbl_get_mouse_x() + fbl_get_camera_x(), fbl_get_mouse_y() + fbl_get_camera_y());

	}



	if (fbl_timer_get_ticks() > start_time + 10000) {
		if (flag) {
			printf("10 seconds passed.\n");
			printf("frames: %d\n", fbl_get_raw_frames_count());
			flag = 0;
			fbl_delete_sprite(0);	// tests the anti-crash solution of deleting head node (check out fbl_delete_sprite())
			fbl_delete_sprite(1);	// works like normal
		}
	}
}


/***************************************************************************************************/

/* Demo 4 */

int loc_x[4]; int loc_y[4]; int speed[4];
int id = 0;
int pathfinder = 0;
int light1, light2;

void setup_demo_4()
{

	int i, j;

	/* this demo works with these constants in a_star.h:

	#define A_STAR_MAP_WIDTH	27
	#define A_STAR_MAP_HEIGHT	16
	#define A_STAR_TILE_SIZE	32
	#define A_STAR_NUM_CHARS	1

	*/

	/* make it so sprites are drawn from top left corner */

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);

	/* init pathfinding module */

	if (fbl_pathf_init() == FBL_PATHF_OUT_OF_MEM)
	{
		/* do stuff */
	}

	fbl_set_window_title("Demo 4: Pathfinding");

	fbl_load_texture("spritesheet_.png");


	fbl_pathf_set_path_status(0, FBL_PATHF_NOT_STARTED);


	do {

		/* set the whole map as walkable */

		for (i = 0; i < fbl_pathf_get_map_w(); i++) {
			for (j = 0; j < fbl_pathf_get_map_h(); j++) {

				fbl_pathf_set_walkability(i, j, FBL_PATHF_WALKABLE);

			}
		}

		/* then set 30% of tiles as unwalkable*/

		for (i = 0; i < fbl_pathf_get_map_w(); i++) {
			for (j = 0; j < fbl_pathf_get_map_h(); j++) {

				if ((rand() % 100) < 30) fbl_pathf_set_walkability(i, j, FBL_PATHF_UNWALKABLE);

			}
		}


		fbl_pathf_set_path_status(0, fbl_pathf_find_path(0, 0, 0, 11 * fbl_pathf_get_tile_size(), 7 * fbl_pathf_get_tile_size(), FBL_PATHF_USE_DIAG));

	} while (fbl_pathf_get_path_status(0) != FBL_PATHF_FOUND);


	/* make top left square walkable */
	 
	fbl_pathf_set_walkability(0, 0, FBL_PATHF_WALKABLE);


	/* creates sprites with correct coordinates as obstacles */

	for (i = 0; i < fbl_pathf_get_map_w(); i++)
		for (j = 0; j < fbl_pathf_get_map_h(); j++) {
			if (fbl_pathf_get_walkability(i, j) == FBL_PATHF_UNWALKABLE) {

				id = fbl_create_sprite(96, 320, 32, 32, 0);
				fbl_set_sprite_xy(id, i * fbl_pathf_get_tile_size(), j * fbl_pathf_get_tile_size());

			}
		}



	/* put sprite target here */

	id = fbl_create_sprite(96, 416, 32, 32, 0);
	fbl_set_sprite_xy(id, 11 * fbl_pathf_get_tile_size(), 7 * fbl_pathf_get_tile_size());

	/* put sprite (pathfinder) there */

	pathfinder = fbl_create_sprite(192, 224, 32, 32, 0);
	fbl_set_sprite_animation(pathfinder, true, 192, 224, 32, 32, 2, 10, true);
	fbl_set_sprite_xy(pathfinder, 0 * fbl_pathf_get_tile_size(), 0 * fbl_pathf_get_tile_size());


	printf("found path!\n");
	printf("path len: %d\n", fbl_pathf_get_path_length(0));


	speed[0] = 1;

	// create small light around the player
	light1 = fbl_create_sprite(384, 0, 128, 128, 0);
	fbl_set_sprite_is_light(light1, true);

	// create the big light
	light2 = fbl_create_sprite(384, 0, 128, 128, 0);
	fbl_set_sprite_is_light(light2, true);
	fbl_set_sprite_scale(light2, 3.0);

	// the lights aren't visible undtil you set the fullscreen lighting tint. (create evening or night-vibe)
	fbl_set_lighting_tint(true, 50, 50, 50);

}

void run_demo_4()
{

	fbl_pathf_read_path(0, loc_x[0], loc_y[0], speed[0]);

	if (loc_x[0] > fbl_pathf_get_x_path(0)) loc_x[0] = loc_x[0] - speed[0];
	if (loc_x[0] < fbl_pathf_get_x_path(0)) loc_x[0] = loc_x[0] + speed[0];
	if (loc_y[0] > fbl_pathf_get_y_path(0)) loc_y[0] = loc_y[0] - speed[0];
	if (loc_y[0] < fbl_pathf_get_y_path(0)) loc_y[0] = loc_y[0] + speed[0];

	/* pathfinder */

	fbl_set_sprite_xy(pathfinder, loc_x[0], loc_y[0]);
	fbl_set_sprite_xy(light1, loc_x[0] - 48, loc_y[0] - 48);
	

	/* let the big light follow the mouse */
	fbl_set_sprite_xy(light2, fbl_get_mouse_x() - (128/2)*3, fbl_get_mouse_y() - (128/2)*3);


}


/***************************************************************************************************/

/* Demo 5 */

void setup_demo_5()
{

	// the window title is set in assets/main.lua

	fbl_lua_init("main.lua");

}

void run_demo_5()
{

	// everything's controlled by lua (main.lua)

	check_reload_lua(); // you can edit lua while program is running and just reload with F5!

}


/***************************************************************************************************/

/* Demo 6 */

#define nums 500
int pos_x[nums];
int pos_y[nums];
int dir_x[nums];
int dir_y[nums];
float scale[nums];

int frams;

void setup_demo_6()
{

	int i;

	fbl_set_window_title("Demo 6: batching benchmark, blendmodes, culling");

	fbl_load_texture("p.png");

	fbl_set_clear_color(168, 230, 255, 255);

	fbl_set_camera_xy(0, 0);

	printf("Camera x, y: %d, %d", fbl_get_camera_x(), fbl_get_camera_y());

	/*
	for (i = 0; i < nums; i++) {
		fbl_create_sprite(0, 0, 64, 64, 0);
		pos_x[i] = rand() % fbl_get_screen_w();
		pos_y[i] = rand() % fbl_get_screen_h();
		fbl_set_sprite_xy(i, pos_x[i], pos_y[i]);
		//fbl_set_sprite_xy(i, -200, 100);
		//fbl_set_sprite_blendmode(i, rand() % 2 + 1);
		fbl_set_sprite_alpha(i, rand() % 255);
		fbl_set_sprite_color(i, 255, rand() % 255, rand() % 255);

		dir_x[i] = 1;
		if (rand() % 2 == 0) dir_x[i] = -1;

		dir_y[i] = 1;
		if (rand() % 2 == 0) dir_y[i] = -1;

		scale[i] = (float)(((rand() % 10) + 5) / 10.0);

		//fbl_set_sprite_scale(i, scale[i]);

		//fbl_set_sprite_active(i, false);


	}


	fbl_set_sprite_alpha(nums-1, 255);
	fbl_set_sprite_color(nums-1, 0, 0, 0);
	fbl_set_sprite_xy(nums-1, 200, 200);
	fbl_fix_sprite_to_screen(nums - 1, true);
	*/

	//fbl_sort_sprites(FBL_SORT_BY_LAYER | FBL_SORT_BY_BLENDMODE);

	
	for (i = 0; i < nums; i++) {
		fbl_create_prim(FBL_RECT, 0, 0, 32, 32, 0, false, true);
		pos_x[i] = rand() % fbl_get_screen_w();
		pos_y[i] = rand() % fbl_get_screen_h();
		fbl_set_prim_xy(i, pos_x[i], pos_y[i]);
		fbl_set_prim_color(i, 255, rand() % 255, rand() % 255, rand() % 255);

		dir_x[i] = 1;
		if (rand() % 2 == 0) dir_x[i] = -1;

		dir_y[i] = 1;
		if (rand() % 2 == 0) dir_y[i] = -1;

	}
	

	flag = 1;
	frams = 0;
	start_time = fbl_timer_get_ticks();


	//printf("1 | 2 = %d", 1|2); // 3

	//fbl_set_camera_xy(1000, 0);

}

void run_demo_6()
{

	int i;
	// unsigned char r, g, b;



	if (fbl_timer_get_ticks() > start_time + 10000) {
		if (flag) {
			printf("10 seconds passed.\n");
			printf("total frames: %d\n", fbl_get_raw_frames_count());
			printf("fps last 10 sec: %d\n", frams);
			flag = 0;
		}
	}

	for (i = 0; i < nums; i++) {

		fbl_set_prim_xy(i, pos_x[i], pos_y[i]);

		pos_x[i] += dir_x[i];
		pos_y[i] += dir_y[i];

		if (pos_x[i] > fbl_get_screen_w()) dir_x[i] = -1;
		if (pos_x[i] < 0) dir_x[i] = 1;
		if (pos_y[i] > fbl_get_screen_h()) dir_y[i] = -1;
		if (pos_y[i] < 0) dir_y[i] = 1;

	}

	/*
	for (i = 0; i < nums; i++) {

		fbl_set_sprite_xy(i, pos_x[i], pos_y[i]);

		pos_x[i] += dir_x[i];
		pos_y[i] += dir_y[i];

		if (pos_x[i] > fbl_get_screen_w()) dir_x[i] = -1;
		if (pos_x[i] < 0) dir_x[i] = 1;
		if (pos_y[i] > fbl_get_screen_h()) dir_y[i] = -1;
		if (pos_y[i] < 0) dir_y[i] = 1;
	
	}
	*/
	/*
	for (i = 0; i < nums; i++) {

		fbl_set_sprite_xy(i, fbl_get_sprite_x(i) + dir_x[i], fbl_get_sprite_y(i) + dir_y[i]);

		if (fbl_get_sprite_x(i) > fbl_get_screen_w()) dir_x[i] = -1;
		if (fbl_get_sprite_x(i) < 0) dir_x[i] = 1;
		if (fbl_get_sprite_y(i) > fbl_get_screen_h()) dir_y[i] = -1;
		if (fbl_get_sprite_y(i) < 0) dir_y[i] = 1;

	
		fbl_set_sprite_scale(i, scale[i] + 0.1);
		fbl_get_sprite_color(i, &r, &g, &b);
		r--;
		g--;
		b--;
		fbl_set_sprite_color(i, r, g, b);

	}
	*/
	
	
	/* move camera */

	if (fbl_get_key_down(FBLK_UP)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - 1);
	if (fbl_get_key_down(FBLK_DOWN)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + 1);
	if (fbl_get_key_down(FBLK_LEFT)) fbl_set_camera_xy(fbl_get_camera_x() - 1, fbl_get_camera_y());
	if (fbl_get_key_down(FBLK_RIGHT)) fbl_set_camera_xy(fbl_get_camera_x() + 1, fbl_get_camera_y());


	frams++;
	
}


/*********************************************************************************************************/

/* Demo 7 */

void setup_demo_7()
{

	fbl_set_window_title("Demo 7: Particles!");
	fbl_set_clear_color(0, 0, 0, 255);

	fbl_load_texture("p.png");

	fbl_create_sprite(0, 0, 64, 64, 0);
	fbl_create_sprite(0, 0, 64, 64, 0);
	fbl_set_sprite_xy(0, 300, 300);
	fbl_set_sprite_xy(1, 400, 300);
	fbl_fix_sprite_to_screen(0, true);


	fbl_create_emitter(10, 10, 500, 100, 1, 3, 1.0, 6.0);	// w, h, num, life, rate, density, scale_start, scale_end
	fbl_set_emitter_xy(0, 200, 200);

	fbl_create_emitter(10, 10, 500, 100, 1, 3, 1.0, 5.0);
	fbl_set_emitter_xy(1, 600, 200);
	fbl_set_emitter_color(1, rand() % 255, rand() % 255, rand() % 255, 255, true);
	fbl_set_emitter_color(1, rand() % 255, rand() % 255, rand() % 255, 0, false);

	
	//fbl_set_fps_locked(false);
	//fbl_set_system_delay_ms(0);

	flag = 1;
	frams = 0;
	start_time = fbl_timer_get_ticks();

}

void run_demo_7()
{

	//if (fbl_get_mouse_release(FBLMB_LEFT)) printf("left mouse up! %d\n", fbl_get_raw_frames_count());
	//if (fbl_get_mouse_click(FBLMB_RIGHT)) printf("right mouse down! %d\n", fbl_get_raw_frames_count());

	if (fbl_get_key_down(FBLK_A)) printf("A key down!%d\n", fbl_get_num_active_particles(0));
	if (fbl_get_key_up(FBLK_A)) printf("A key up!%d\n", fbl_get_raw_frames_count());

	fbl_set_emitter_xy(1, fbl_get_mouse_x() + fbl_get_camera_x(), fbl_get_mouse_y() + fbl_get_camera_y());

	
	if (fbl_timer_get_ticks() > start_time + 10000) {
		if (flag) {
			printf("10 seconds passed.\n");
			printf("total frames: %d\n", fbl_get_raw_frames_count());
			printf("fps last 10 sec: %d\n", frams);
			flag = 0;
		}
	}

	frams++;


	/* move camera */

	if (fbl_get_key_down(FBLK_UP)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() - 2);
	else if (fbl_get_key_down(FBLK_DOWN)) fbl_set_camera_xy(fbl_get_camera_x(), fbl_get_camera_y() + 2);
	else if (fbl_get_key_down(FBLK_LEFT)) fbl_set_camera_xy(fbl_get_camera_x() - 2, fbl_get_camera_y());
	else if (fbl_get_key_down(FBLK_RIGHT)) fbl_set_camera_xy(fbl_get_camera_x() + 2, fbl_get_camera_y());

}


/*********************************************************************************************************/

/* Demo 8 */

int x_offset;
int y_offset;
int circle_radius = 500;
int num_rays = 50;
double pie_slice = 45.0;	// the wideness of the light-cone thing

void setup_demo_8()
{

	fbl_set_window_title("Demo 8: Raycasting");
	fbl_set_clear_color(80, 80, 80, 255);


	fbl_load_ttf_font("edosz.ttf", 20);
	fbl_create_text(0, 255, 0, 255, "Left click (hold)");
	fbl_set_text_xy(0, 10, 20);


	// FBL_RAY is a special type of primitive

	fbl_phys_init();	// have to call this before using rays

	fbl_load_texture("spritesheet_.png");

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);


	for (int i = 0; i < 20; i++) {

		int lol = fbl_create_sprite(96, 320, 32, 32, 0);
		fbl_set_sprite_xy(lol, rand() % 900 + 20, rand() % 500);
		fbl_set_sprite_phys(lol, true, FBL_RECT, FBL_PHYS_KINEMATIC, true);
		
	}

	x_offset = fbl_get_screen_w() / 2;
	y_offset = fbl_get_screen_h() / 2;

	angle = 0.0;

	// create a bunch of rays
	for (int i = 0; i < num_rays; i++)
	{
		double angle_rad = angle * (3.141592654 / 180);
		double x = circle_radius * cos(angle_rad) + x_offset;
		double y = circle_radius * sin(angle_rad) + y_offset;
		fbl_create_prim(FBL_RAY, x_offset, y_offset, (int)x, (int)y, 1, false, false);
		angle = angle + pie_slice / num_rays; // the number after the
	}

	angle = 0.0;

	// note that fbl_set_prim_phys() on rays have no effect at all, and fbl_phys_init() must be called before use.


}

void run_demo_8()
{
	
	if (fbl_get_mouse_click(FBLMB_LEFT)) {

		angle += 0.2;	//spin slowly
			if (angle > 360) angle = 0;

		double tmp = angle;

		for (int i = 0; i < num_rays; i++)
		{
			double angle_rad = angle * (3.141592654 / 180);
			double x = circle_radius * cos(angle_rad) + x_offset;
			double y = circle_radius * sin(angle_rad) + y_offset;
			angle = angle + pie_slice / num_rays;
			fbl_set_prim_xy(i, fbl_get_mouse_x(), fbl_get_mouse_y());
			fbl_set_prim_size(i, x + fbl_get_mouse_x() - x_offset, y + fbl_get_mouse_y() - y_offset, 0);
		}
		
		angle = tmp;

		//if (rand() % 10 == 1) printf("%d", fbl_get_num_prims());

	}


}


/*********************************************************************************************************/


void cycle_demos()
{

	static int a_flag = 0;

	if ((fbl_get_key_down(FBLK_F1) || fbl_get_ui_elem_val(left_button)) && a_flag == 0) {
		if (cur_demo > 0) {
			printf("f1\n");
			unload_all();
			cur_demo--;
			(*demo_setup[cur_demo])();
			a_flag = 20; // can only press with this many frames in between
		}
	}
	if ((fbl_get_key_down(FBLK_F2) || fbl_get_ui_elem_val(right_button)) && a_flag == 0) {
		if (cur_demo < NUM_DEMOS - 1) {
			printf("f2\n");
			unload_all();
			cur_demo++;
			(*demo_setup[cur_demo])();
			a_flag = 20;
		}
	}

	a_flag--;
	if (a_flag < 0) a_flag = 0;

}

void check_reload_lua()
{

	static int a_flag = 0;

	if (fbl_get_key_down(FBLK_F5) && a_flag == 0) {
		printf("Reloaded!\n");
		unload_all();
		setup_demo_5();
		a_flag = 30; // can only press with this many frames in between
	}

	a_flag--;
	if (a_flag < 0) a_flag = 0;

}

void unload_all()
{

	fbl_destroy_texture();
	fbl_destroy_all_sprites();
	fbl_destroy_all_prims();
	fbl_destroy_all_text_objects();
	fbl_destroy_ttf_font();
	//fbl_destroy_ui_texture();	// we want to keep the buttons (down left/right) to switch demos
	fbl_destroy_all_ui_elems();
	fbl_destroy_all_emitters();
	fbl_destroy_all_sounds();
	fbl_destroy_music();
	fbl_pathf_shutdown();
	fbl_lua_shutdown();
	fbl_phys_shutdown();


	/* reset some variables */

	x = 10;
	y = 10;
	flag = 0;
	angle = 0.0;

	loc_x[0] = 0;
	loc_y[0] = 0;
	id = 0;

	//fbl_pathf_set_path_status(0, FBL_PATHF_NOT_STARTED);

	fbl_set_camera_xy(0, 0);	// otherwise the other demos suffer a tad

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);

	left_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);
	right_button = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);

	fbl_set_ui_elem_xy(left_button, 64, fbl_get_screen_h() - 64);
	fbl_set_ui_elem_xy(right_button, fbl_get_screen_w() -64, fbl_get_screen_h() - 64);

	// set time of day to normal (day)
	fbl_set_lighting_tint(false, 0, 0, 0);

}

int pressed_button(int x, int y)
{


	printf("lol!! %d, %d\n", x, y);

	return 0;

}
