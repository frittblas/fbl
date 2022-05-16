/*
*
*	FBL ENGINE
*
*	lua_template.c
*
*	Contains the minimal setup to use Lua.
*	Reload the Lua template with F5 after making changes to assets/main.lua
*
*	Hans Strömquist 2020
*
*/

#include "../src/fbl.h"


void setup_template(void);
void run_template(void);
void check_reload_lua(void);
void unload_all(void);

/* for timing */

unsigned int start_time;


void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	fbl_set_clear_color(168, 230, 255, 255);

	setup_template();

	
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
