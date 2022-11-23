/*
*
*	Charming Alarming 2: Reasonable Robots
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
#include "Weather.hpp"

#include <iostream>

UserInput::UserInput() {

	std::cout << "Initialized UserInput subsystem." << std::endl;

};
UserInput::~UserInput() {

	std::cout << "Destroyed UserInput subsystem." << std::endl;

};

void UserInput::tick(Game& g) {

	static int access = 0;
	static bool isFullscreen = false;
	const int buttonDelay = 30;

	if (fbl_get_key_down(FBLK_1) && access == 0) {
		g.mState->change(g, GameState::StateType::Title);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_2) && access == 0) {
		g.mState->change(g, GameState::StateType::Settings);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_3) && access == 0) {
		g.mState->change(g, GameState::StateType::Explore);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_4) && access == 0) {
		g.mState->change(g, GameState::StateType::Dialogue);
		access = buttonDelay;
	}

	// for android, temporary :)
	if (fbl_get_mouse_release(FBLMB_LEFT) && access == 0 && g.mState->get() == GameState::StateType::Title) {
		g.mState->change(g, GameState::StateType::Explore);
		access = buttonDelay;
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

	// test weather
	if (fbl_get_key_down(FBLK_F1) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Morning, 0, 0, 150, false);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_F2) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Day, 2, 0, 100, false);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_F3) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 150, true);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_F4) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Late, 1, 0, 100, true);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_F5) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Night, 0, 0, 10, false);
		access = buttonDelay;
	}
	if (fbl_get_key_down(FBLK_F6) && access == 0) {
		g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 6, 50, false);
		access = buttonDelay;
	}

	// support for standard alt + enter for fullscreen
	if (fbl_get_key_down(FBLK_LALT) && fbl_get_key_down(FBLK_RETURN) && access == 0) {
		if (isFullscreen) {
			fbl_set_window_mode(0);	// windowed
			isFullscreen = false;
			access = buttonDelay;
		}
		else {
			fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);
			isFullscreen = true;
			access = buttonDelay;
		}
	}

	access--;
	if (access < 0) access = 0;

}
