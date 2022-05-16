/*
*
*	FBL ENGINE
*
*	Cpp_test.cpp
*
*	Contains the minimal setup to use C++.
*	
*
*	Hans Strömquist 2022
*
*/

#include "../src/fbl.hpp"

#include <iostream>


void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	fbl_set_clear_color(168, 230, 255, 255);

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

}

void fbl_end()
{

	std::cout<<"Bye!"<<std::endl;

}
