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

	int tmpId;

	// replace this with the lowest level robot in your collection.
	g.mRobots->mRacingRobots[0] = g.mRobots->mOwnedRobots[g.mProgress->mFavRobot];

	// create all the ui elements for maintenance mode!

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
		mTimerBar[i].totalTime = rand() % 4 + 4;
		mTimerBar[i].timeLeft = 7 * 30;
		mTimerBar[i].primId = fbl_create_prim(FBL_RECT, mTimerBar[i].x, mTimerBar[i].y, mTimerBar[i].totalTime * 30, 5, 0, false, true);
		fbl_set_prim_color(mTimerBar[i].primId, 0, 255, 0, 255);
	}

	setupAirPressure(Game::DeviceResW / 4 + 64, 30);
	setupColorCables(25, 400);




}

void Maintenance::setupAirPressure(int x, int y) {

	int tmpId;

	// create air pressure minigame ui
	// meter
	mAirMeter.x = x;
	mAirMeter.y = y;
	mAirMeter.meterId = fbl_create_prim(FBL_NORMAL_RECT, mAirMeter.x, mAirMeter.y, 32, 200, 0, false, true);
	fbl_set_prim_color(mAirMeter.meterId, 11, 168, 230, 255);	// sky blue
	// sweet spot
	mAirMeter.sweetSpotY = 100;
	mAirMeter.sweetSpotSize = 30;
	mAirMeter.pointerY = mAirMeter.sweetSpotY + mAirMeter.sweetSpotSize / 2;
	mAirMeter.speed = 0.8;
	mAirMeter.checkDuration = 0;
	mAirMeter.sweetSpotId = fbl_create_prim(FBL_NORMAL_RECT, mAirMeter.x, mAirMeter.sweetSpotY, 32, mAirMeter.sweetSpotSize, 0, false, true);
	fbl_set_prim_color(mAirMeter.sweetSpotId, 255, 255, 255, 255);	// white sweet spot
	// pointer
	mAirMeter.pointerId = fbl_create_sprite(256, 288, 16, 16, 0);
	fbl_set_sprite_xy(mAirMeter.pointerId, mAirMeter.x - 15, mAirMeter.pointerY);
	fbl_set_sprite_animation(mAirMeter.pointerId, true, 256, 288, 16, 16, 4, 7, true);
	// up/down arrows
	mAirMeter.arrowUpId = fbl_create_ui_elem(FBL_UI_BUTTON_HOLD, 128, 96, 32, 32, NULL);
	fbl_set_ui_elem_xy(mAirMeter.arrowUpId, mAirMeter.x + 80, mAirMeter.y + 70);
	mAirMeter.arrowDownId = fbl_create_ui_elem(FBL_UI_BUTTON_HOLD, 128, 128, 32, 32, NULL);
	fbl_set_ui_elem_xy(mAirMeter.arrowDownId, mAirMeter.x + 80, mAirMeter.y + 120);
	// Air pressure instructions
	fbl_load_ttf_font("font/roboto.ttf", 20);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Air Pressure Tuning (APT):");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x - 279, mAirMeter.y + 7);
	fbl_load_ttf_font("font/roboto.ttf", 16);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"When the timer is done");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x - 279, mAirMeter.y + 37);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"make sure the green arrow");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x - 279, mAirMeter.y + 67);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"is in the sweet spot.");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x - 279, mAirMeter.y + 97);
	// shortcut keys
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"W");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x + 120, mAirMeter.y + 70);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"S");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x + 122, mAirMeter.y + 120);

}

void Maintenance::setupColorCables(int x, int y) {

	int tmpId;

	// memorize and match colored cables
	mColorCables.x = x;
	mColorCables.y = y;
	mColorCables.lineId[0] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[0], 255, 100, 30, 255);	// orange
	mColorCables.lineId[1] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y + 32, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[1], 255, 255, 255, 255); // white
	mColorCables.lineId[2] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y + 64, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[2], 0, 83, 255, 255);	// blue

	mColorCables.mimicLineId[0] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[0], 255, 100, 30, 255);
	mColorCables.mimicLineId[1] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y + 32, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[1], 255, 255, 255, 255);
	mColorCables.mimicLineId[2] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y + 64, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[2], 0, 83, 255, 255);

	mColorCables.button[0] = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(mColorCables.button[0], mColorCables.x + 360, mColorCables.y - 2);
	mColorCables.button[1] = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(mColorCables.button[1], mColorCables.x + 360, mColorCables.y + 35);
	mColorCables.button[2] = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(mColorCables.button[2], mColorCables.x + 360, mColorCables.y + 72);

	// Color cables instructions
	fbl_load_ttf_font("font/roboto.ttf", 20);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Match Colored Cables (MCC):");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 92);
	fbl_load_ttf_font("font/roboto.ttf", 16);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Memorize and match the colors of the cables");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 62);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"before the time runs out.");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 32);
	// shortcut keys
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"E");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x + 390, mColorCables.y);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"D");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x + 390, mColorCables.y + 35);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"C");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x + 390, mColorCables.y + 72);


}

void Maintenance::setupCalcChecksum(int x, int y) {

}
void Maintenance::setupSequencer(int x, int y) {

}

void Maintenance::getInput(Game& g) {

	//Efx::getInstance().shakeCamera(10, 10);

	// Air pressure input
	if (fbl_get_ui_elem_val(mAirMeter.arrowUpId) || fbl_get_key_down(FBLK_W)) {
		if(mAirMeter.checkDuration == 0)
			mAirMeter.pointerY -= 2;
	}
	if (fbl_get_ui_elem_val(mAirMeter.arrowDownId) || fbl_get_key_down(FBLK_S)) {
		if (mAirMeter.checkDuration == 0)
			mAirMeter.pointerY += 2;
	}

	// color cables input
	if (fbl_get_ui_elem_val(mColorCables.button[0]) || fbl_get_key_down(FBLK_E)) {
		mColorCables.mimicColor[0]++;
		if (mColorCables.mimicColor[0] > 2)
			mColorCables.mimicColor[0] = 0;
	}
	if (fbl_get_ui_elem_val(mColorCables.button[1]) || fbl_get_key_down(FBLK_D)) {
		mColorCables.mimicColor[1]++;
		if (mColorCables.mimicColor[1] > 2)
			mColorCables.mimicColor[1] = 0;
	}
	if (fbl_get_ui_elem_val(mColorCables.button[2]) || fbl_get_key_down(FBLK_C)) {
		mColorCables.mimicColor[2]++;
		if (mColorCables.mimicColor[2] > 2)
			mColorCables.mimicColor[2] = 0;
	}


}

void Maintenance::processAirPressure(Game& g) {

	mAirMeter.checkDuration--;

	if (mAirMeter.checkDuration < 0)
		mAirMeter.checkDuration = 0;

	if (mAirMeter.checkDuration == 1) {

		// put back to white
		fbl_set_prim_color(mAirMeter.sweetSpotId, 255, 255, 255, 255);

	}

	// limit values and set pointerY
	if (mAirMeter.pointerY < 30) mAirMeter.pointerY = 30;
	if (mAirMeter.pointerY > 229) mAirMeter.pointerY = 229;
	fbl_set_sprite_xy(mAirMeter.pointerId, mAirMeter.x - 15, mAirMeter.pointerY);

	// move the sweet spot, limit values and change dir
	if (mAirMeter.checkDuration == 0) {
		mAirMeter.sweetSpotY += mAirMeter.speed;
		if (mAirMeter.sweetSpotY > 230 - mAirMeter.sweetSpotSize) mAirMeter.speed = -mAirMeter.speed;
		if (mAirMeter.sweetSpotY < 31) mAirMeter.speed = -mAirMeter.speed;
		fbl_set_prim_xy(mAirMeter.sweetSpotId, mAirMeter.x, mAirMeter.sweetSpotY);
	}

	// check the pointer when the timer is done
	if (mTimerBar[0].timeLeft > 1) return;

	// times up!
	int y = fbl_get_sprite_y(mAirMeter.pointerId);
	if ((y + 1) >= mAirMeter.sweetSpotY && y <= mAirMeter.sweetSpotY + mAirMeter.sweetSpotSize) {

		// play sound

		// make sweet spot green for a while

		fbl_set_prim_color(mAirMeter.sweetSpotId, 0, 220, 0, 255);
		mAirMeter.checkDuration = 60;

	}
	else {

		fbl_set_prim_color(mAirMeter.sweetSpotId, 220, 0, 0, 255);
		mAirMeter.checkDuration = 60;

	}

}

void Maintenance::processColorCables(Game& g) {


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
				if (greenFade > 230) greenFade = 230;
				if (greenFade < 1) greenFade = 0;
				fbl_set_prim_color(mTimerBar[i].primId, mTimerBar[i].red, greenFade, 0, 255);
				mTimerBar[i].red++;
				if (mTimerBar[i].red > 255) mTimerBar[i].red = 255;
			}
		}
	}

}

void Maintenance::hideSprites() {

	fbl_set_sprite_active(mAirMeter.pointerId, false);

}

void Maintenance::tick(Game& g) {

	if (Race::sRaceState == Undecided) {
		getInput(g);
		processAirPressure(g);
		processTimers(g);
	}
	else {
		mPostRaceDelay--;
		if (mPostRaceDelay == 0) {
			if (Race::sRaceState == Dead) {
				mPostRace->gameOver();
			}
			else {
				hideSprites();
				mPostRace->initPostRaceMenu(g, false);
			}
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
