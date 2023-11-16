/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Maintenance.cpp
*
*   Maintenance class implementation, takes care of the Maintenance game-state.
*
*	Hans Strömquist 2023
*
*/


#include "../../../src/fbl.hpp"
#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"
#include "../Game.hpp"
#include "../Progress.hpp"
#include "../SysManager.hpp"
#include "../Efx.hpp"


#include "../Robots.hpp"

#include "Race/Race.hpp"
#include "Race/PostRace.hpp"
#include "Maintenance.hpp"


// Race-class implementation

Maintenance::Maintenance() {


	mPostRace = new PostRace();
	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);
	fbl_set_camera_xy(0, 0);
	fbl_set_clear_color(50, 50, 50, 0);

	Race::sRaceState = Undecided;
	mPostRaceDelay = 60;	// one second delay before PostRace menu or game over is shown.



	std::cout << "Started Maintenance state." << std::endl;

}

Maintenance::~Maintenance() {

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);	// in explore mode sprites are drawn from the top left

	delete mPostRace;

	std::cout << "Destroyed Maintenance state." << std::endl;

}

void Maintenance::setupMaintenance(Game& g) {

	// replace this with the lowest level robot in your collection.
	g.mRobots->mRacingRobots[0] = g.mRobots->mOwnedRobots[g.mProgress->mFavRobot];

	// create all the ui elements for maintenance mode!

	int tmpId;

	// create the white outline as RECT
	tmpId = fbl_create_prim(FBL_RECT, Game::DeviceResW / 2, Game::DeviceResH / 2, 479, 269, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);

	// lines to divide into 4 parts
	tmpId = fbl_create_prim(FBL_LINE, 1, g.LogicalResH / 2, g.LogicalResW, g.LogicalResH / 2, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	tmpId = fbl_create_prim(FBL_LINE, g.LogicalResW / 2, 1, g.LogicalResW / 2, g.LogicalResH, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);

	// create the 4 timer bars
	mTimerBar[0].x = Game::DeviceResW / 4;
	mTimerBar[0].y = 250;
	mTimerBar[1].x = Game::DeviceResW / 4 + Game::DeviceResW / 2;
	mTimerBar[1].y = 250;
	mTimerBar[2].x = Game::DeviceResW / 4;
	mTimerBar[2].y = 520;
	mTimerBar[3].x = Game::DeviceResW / 4 + Game::DeviceResW / 2;
	mTimerBar[3].y = 520;
	for (int i = 0; i < 4; i++) {
		mTimerBar[i].red = 0;
		mTimerBar[i].totalTime = 7;
		mTimerBar[i].timeLeft = 7 * 30;
		mTimerBar[i].primId = fbl_create_prim(FBL_RECT, mTimerBar[i].x, mTimerBar[i].y, mTimerBar[i].totalTime * 30, 5, 0, false, true);
		fbl_set_prim_color(mTimerBar[i].primId, 0, 255, 0, 255);
	}


}

void Maintenance::getInput(Game& g) {



}

void Maintenance::processTimers(Game& g) {

	if (fbl_get_raw_frames_count() % 2 == 0) {
		for (int i = 0; i < 4; i++) {
			mTimerBar[i].timeLeft--;
			if (mTimerBar[i].timeLeft < 1) {
				mTimerBar[i].timeLeft = mTimerBar[i].totalTime * 30;
				mTimerBar[i].red = 0;
			}
			if (mTimerBar[i].timeLeft < mTimerBar[i].totalTime * 30) {
				fbl_set_prim_size(mTimerBar[i].primId, mTimerBar[i].timeLeft, 5, 0);
				int greenFade = mTimerBar[i].timeLeft * 2;
				if (greenFade > 255) greenFade = 255;
				if (greenFade < 1) greenFade = 0;
				fbl_set_prim_color(mTimerBar[i].primId, mTimerBar[i].red, greenFade, 0, 255);
				mTimerBar[i].red++;
				if (mTimerBar[i].red > 255) mTimerBar[i].red = 255;
			}
		}
	}

}

void Maintenance::tick(Game& g) {

	if (Race::sRaceState == Undecided) {
		getInput(g);
		processTimers(g);
	}
	else {
		mPostRaceDelay--;
		if (mPostRaceDelay == 0) {
			if(Race::sRaceState == Dead) mPostRace->gameOver();
				else mPostRace->initPostRaceMenu(g, false);
		}

		if (mPostRaceDelay < 0) {
			mPostRaceDelay = -1;
			mPostRace->tick(g);
		}
	}

	Efx::getInstance().tickCameraShake();


	if (fbl_get_mouse_click(FBLMB_RIGHT)) Efx::getInstance().shakeCamera(20, 40);

	if (fbl_get_key_down(FBLK_7)) Race::sRaceState = First;
	if (fbl_get_key_down(FBLK_8)) Race::sRaceState = Second;
	if (fbl_get_key_down(FBLK_9)) Race::sRaceState = Third;
	if (fbl_get_key_down(FBLK_0)) Race::sRaceState = Fourth;

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick maintenance!" << std::endl;

}
