/*
*
*	Lotr
*
*	UserInput.cpp
*
*	UserInput class implementation, takes care of all the user input.
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "GameState/GameState.hpp"
#include "UserInput.hpp"

extern GameState* gState;

UserInput::UserInput() {};
UserInput::~UserInput() {};

void UserInput::tick() {


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

}
