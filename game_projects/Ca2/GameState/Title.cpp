/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Title.cpp
*
*   Title class implementation, takes care of the title screen.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

#include "../../../src/fbl.hpp"
#include "../Game.hpp"
#include "../Efx.hpp"
#include "../SoundManager.hpp"
#include "../Weather.hpp"
#include "Title.hpp"


 int firstRun = 3;

// Title-class implementation

Title::Title() {

	fbl_load_ttf_font("font/garamond.ttf", 48);
	mTitleId = fbl_create_text(255, 215, 0, 255, (char*)"Charming Alarming 2");
	//fbl_set_text_xy(mTitleId, Game::DeviceResW / 2, 100);
	fbl_set_text_align(mTitleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("font/roboto_italic.ttf", 27);
	mSubTitleId = fbl_create_text(226, 92, 161, 255, (char*)"Reasonable Robots");
	//fbl_set_text_xy(mSubTitleId, Game::DeviceResW / 2, 200);
	fbl_set_text_align(mSubTitleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("font/garamond.ttf", 17);
	mAuthorTextId = fbl_create_text(255, 255, 255, 255, (char*)"Hans Stromquist 2023");
	fbl_set_text_xy(mAuthorTextId, 10, Game::DeviceResH - 20);

	mWebsiteTextId = fbl_create_text(255, 255, 255, 255, (char*)"www.charmingalarming.se");
	fbl_set_text_xy(mWebsiteTextId, Game::DeviceResW - 200, Game::DeviceResH - 20);

	std::cout << "Started Title state." << std::endl;

	if (firstRun == 0) {
		mTweenIdTitle = Efx::getInstance().setupTween(-200, 100, 2000, Efx::getInstance().ElasticOut);
		mTweenIdSub = Efx::getInstance().setupTween(-300, Game::DeviceResW / 2, 2000, Efx::getInstance().ElasticOut);
	}

}

Title::~Title() {


	fbl_destroy_all_text_objects();

	std::cout << "Destroyed Title state." << std::endl;

}

void Title::hideTitle() {

	fbl_set_text_active(mTitleId, false);
	fbl_set_text_active(mSubTitleId, false);
	fbl_set_text_active(mAuthorTextId, false);
	fbl_set_text_active(mWebsiteTextId, false);

}

void Title::showTitle() {

	fbl_set_text_active(mTitleId, true);
	fbl_set_text_active(mSubTitleId, true);
	fbl_set_text_active(mAuthorTextId, true);
	fbl_set_text_active(mWebsiteTextId, true);

}

// implement the virtual tick() function
void Title::tick(Game& g) {

	if (firstRun == 3) {
		fbl_pause_music();
		hideTitle();

		fbl_load_ttf_font("font/garamond.ttf", 40);
		mIntroId = fbl_create_text(255, 215, 0, 255, (char*)"Kastanjen.net presents:");
		fbl_set_text_align(mIntroId, FBL_ALIGN_CENTER);
		fbl_set_text_xy(mIntroId, Game::DeviceResW / 2, 200);

		fbl_load_ttf_font("font/roboto_italic.ttf", 20);
		mSubIntroId = fbl_create_text(230, 230, 230, 255, (char*)"(click to start)");
		fbl_set_text_align(mSubIntroId, FBL_ALIGN_CENTER);
		fbl_set_text_xy(mSubIntroId, Game::DeviceResW / 2, 400);

		firstRun = 2;

	}

	if (firstRun == 1) {

		fbl_set_text_active(mIntroId, false);
		fbl_set_text_active(mSubIntroId, false);
	
		showTitle();

		SoundManager::getInstance().loadAndPlayMusic("music/title.ogg", 40, 0);
		SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSummer, SoundManager::Channel::Ambient, 1);

		g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 14, 80, false);

		mTweenIdTitle = Efx::getInstance().setupTween(-200, 100, 2000, Efx::getInstance().ElasticOut);
		mTweenIdSub = Efx::getInstance().setupTween(-300, Game::DeviceResW / 2, 2000, Efx::getInstance().ElasticOut);

		firstRun = 0;
	
	}

	if (firstRun == 0) {
		fbl_set_text_xy(mTitleId, Game::DeviceResW / 2, Efx::getInstance().getCurValue(mTweenIdTitle));
		fbl_set_text_xy(mSubTitleId, Efx::getInstance().getCurValue(mTweenIdSub), 200);
		Efx::getInstance().tickTweens();
		g.mWeather->tick();
	}



	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49
	if (num == 0)
		std::cout << "Tick title!" << std::endl;

}
