/*
*
*	Lotr
*
*	Lotr.cpp
*
*	Entry point and game loop, contains the 3 required fbl functions start() loop() and end().
*	
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "Init.hpp"
#include "GameState/GameState.hpp"

extern GameState* gState;

void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	Init::getInstance().initLotr();

}

void fbl_game_loop()
{

	static int access = 0;

	if (fbl_get_key_down(FBLK_1) && access == 0) {
		gState->change(GameState::StateType::Title);
		access = 20;
	}
	if (fbl_get_key_down(FBLK_2) && access == 0) {
		gState->change(GameState::StateType::Settings);
		access = 20;
	}
	if (fbl_get_key_down(FBLK_3) && access == 0) {
		gState->change(GameState::StateType::Explore);
		access = 20;
	}
	if (fbl_get_key_down(FBLK_4) && access == 0) {
		gState->change(GameState::StateType::Dialogue);
		access = 20;
	}

	access--;
	if (access < 0) access = 0;

	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9))
		fbl_set_window_mode(0);
	if (fbl_get_key_down(FBLK_F10))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
	if (fbl_get_key_down(FBLK_F11))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);

	gState->tick();

}

void fbl_end()
{

	Init::getInstance().unInitLotr();

	std::cout<<"Bye!"<<std::endl;

}
