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
#include "Game.hpp"
#include "GameState/GameState.hpp"
#include "UserInput.hpp"

UserInput::UserInput() {};
UserInput::~UserInput() {};

void UserInput::tick(Game& g) {


	static int access = 0;
	static bool isFullscreen = false;

	if (fbl_get_key_down(FBLK_1) && access == 0) {
		g.mState->change(g, GameState::StateType::Title);
		access = 30;
	}
	if (fbl_get_key_down(FBLK_2) && access == 0) {
		g.mState->change(g, GameState::StateType::Settings);
		access = 30;
	}
	if (fbl_get_key_down(FBLK_3) && access == 0) {
		g.mState->change(g, GameState::StateType::Explore);
		access = 30;
	}
	if (fbl_get_key_down(FBLK_4) && access == 0) {
		g.mState->change(g, GameState::StateType::Dialogue);
		access = 30;
	}

	// for android, temporary :)
	if (fbl_get_mouse_release(FBLMB_LEFT) && access == 0 && g.mState->get() == GameState::StateType::Title) {
		g.mState->change(g, GameState::StateType::Explore);
		access = 30;
	}


	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9)) {
		fbl_set_window_mode(0);	// windowed
		isFullscreen = false;
	}
	if (fbl_get_key_down(FBLK_F11)) {
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);
		isFullscreen = true;
	}

	// support for standard alt + enter for fullscreen
	if (fbl_get_key_down(FBLK_LALT) && fbl_get_key_down(FBLK_RETURN) && access == 0) {
		if (isFullscreen) {
			fbl_set_window_mode(0);	// windowed
			isFullscreen = false;
			access = 30;
		}
		else {
			fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);
			isFullscreen = true;
			access = 30;
		}
	}

	access--;
	if (access < 0) access = 0;

}
