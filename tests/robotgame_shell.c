/*
*
*	FBL ENGINE
*
*	robotgame_shell.c
*
*	Contains helper functios for the robot game,
*	like setting up a gameboard, so you can script
*	everything on a higher level in Lua.
* 
*	Update: maybe I'll do everything in Lua..
*
*	Hans Strömquist 2021
*
*/

#include "../src/fbl.h"

#include "../dependencies/common/lua-5.4.1/include/lua.h"
#include "../dependencies/common/lua-5.4.1/include/lualib.h"
#include "../dependencies/common/lua-5.4.1/include/lauxlib.h"

void setup_template(void);
void run_template(void);
void check_reload_lua(void);
void unload_all(void);

/* for timing */

unsigned int start_time;


void fbl_start()
{

	// make sure that the name of the asset folder is set to what you want (default = "assets/")

	fbl_set_assets_folder_name("robot_assets/");

	fbl_engine_init(800, 864, 60);
	fbl_set_clear_color(168, 230, 255, 255);

	setup_template();

	//fbl_create_threadpool();

}

void fbl_game_loop()
{

	run_template();
	
	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9))
		fbl_set_window_mode(0);
	if (fbl_get_key_down(FBLK_F10))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
	if (fbl_get_key_down(FBLK_F11))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);

}

void fbl_end()
{

	//fbl_destroy_threadpool();
	printf("bye!");

}


/***************************************************************************************************/

void setup_template()
{
	
	
	fbl_lua_init("main.lua");
	
}

void run_template()
{
	
	// everything's controlled by lua (main.lua)

	check_reload_lua(); // you can edit lua while program is running and just reload with F5!	
	
}

void check_reload_lua()
{

	static int a_flag = 0;

	if (fbl_get_key_down(FBLK_F5) && a_flag == 0) {
		printf("Reloaded!\n");
		unload_all();
		setup_template();
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
	fbl_destroy_ui_texture();
	fbl_destroy_all_ui_elems();
	fbl_destroy_all_emitters();
	fbl_destroy_all_sounds();
	fbl_destroy_music();
	fbl_pathf_shutdown();
	fbl_lua_shutdown();
	fbl_phys_shutdown();


}


/***************************************************************************************************/

							/*	Helper functions for robot game (registered by Lua) */

/***************************************************************************************************/



