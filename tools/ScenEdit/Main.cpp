/*
*
*	FBL ENGINE
*
*	Main.cpp
*
*	The entry point for ScenEdit, contains the fbl functions 
*
*	Hans Strömquist 2022
*
*/

#include "ScenEdit.hpp"

ScenEdit* gEditor;


void fbl_start()
{

	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	fbl_set_clear_color(50, 50, 150, 255);

	fbl_set_window_title((char*)"ScenEdit 1.0");

	gEditor = new ScenEdit(true);


}

void fbl_game_loop()
{

	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9))
		fbl_set_window_mode(0);
	if (fbl_get_key_down(FBLK_F10))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
	if (fbl_get_key_down(FBLK_F11))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);

	gEditor->tick();

}

void fbl_end()
{

	gEditor->resetMap(0, 0);	// free tile-mem
	delete gEditor;

	std::cout << "Bye!" << std::endl;

}
