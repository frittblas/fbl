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
#include "../SoundManager.hpp"
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
	mPostRaceDelay = 120;	// two second delay before PostRace menu or game over is shown.

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

	// find robot with lowest level and assign for maintenance
	if (g.mRobots->ownedRobotsLeft(g) > 1) {

		auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[g.mProgress->mFavRobot]);

		int lowestLevel = sta.level;	// start comparing with the fav robot

		for (int i = 0; i < g.mRobots->NumRobots; i++) {
			if (g.mRobots->mOwnedRobots[i] != g.mRobots->Unassigned) {
				auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[i]);
				if (sta.hp > 0.1) {
					if (sta.level <= lowestLevel) {
						lowestLevel = sta.level;	// set new lowest
						g.mRobots->mRacingRobots[0] = g.mRobots->mOwnedRobots[i];	// assign new robot
					}
				}
			}
		}

	}

	// show the current robot being maintined in the low right corner
	auto& spr = g.mEcs->GetComponent<Sprite>(g.mRobots->mRacingRobots[0]);
	fbl_set_sprite_xy(spr.id[0], Game::DeviceResW - 64, 64);
	fbl_set_sprite_active(spr.id[0], true);


	mTotalOps = g.mProgress->mCompletedRaces + g.mProgress->mCompletedMaint + 5;
	mOpsLeft = mTotalOps;

	// create all the ui elements for maintenance mode!

	// create the white outline as RECT
	tmpId = fbl_create_prim(FBL_RECT, Game::DeviceResW / 2, Game::DeviceResH / 2, 479, 269, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);

	// lines to divide into 4 parts
	tmpId = fbl_create_prim(FBL_LINE, 1, g.LogicalResH / 2, g.LogicalResW, g.LogicalResH / 2, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	tmpId = fbl_create_prim(FBL_LINE, g.LogicalResW / 2, 1, g.LogicalResW / 2, g.LogicalResH, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);


	// randomize where the minigames should be displayed

	pos[0].x = 0;
	pos[0].y = 0;
	pos[1].x = 0;
	pos[1].y = Game::DeviceResH / 2;
	pos[2].x = Game::DeviceResW / 2;
	pos[2].y = 0;
	pos[3].x = Game::DeviceResW / 2;
	pos[3].y = Game::DeviceResH / 2;

	//srand(1160);
	if (g.mProgress->mCompletedMaint > 3)
		std::random_shuffle(std::begin(pos), std::end(pos));

	mTimerBar[0].x = pos[0].x + Game::DeviceResW / 4;
	mTimerBar[0].y = pos[0].y + 250;
	mTimerBar[2].x = pos[1].x + Game::DeviceResW / 4;
	mTimerBar[2].y = pos[1].y + 250;
	mTimerBar[1].x = pos[2].x + Game::DeviceResW / 4;
	mTimerBar[1].y = pos[2].y + 250;
	mTimerBar[3].x = pos[3].x + Game::DeviceResW / 4;
	mTimerBar[3].y = pos[3].y + 250;

	// create the 4 timer bars
	/*
	mTimerBar[0].x = Game::DeviceResW / 4;
	mTimerBar[0].y = 250;
	mTimerBar[1].x = Game::DeviceResW / 4 + Game::DeviceResW / 2;
	mTimerBar[1].y = 250;
	mTimerBar[2].x = Game::DeviceResW / 4;
	mTimerBar[2].y = 520;
	mTimerBar[3].x = Game::DeviceResW / 4 + Game::DeviceResW / 2;
	mTimerBar[3].y = 520;
	*/

	mTimerBar[0].timeLeft = (7 - 1) * 30;
	mTimerBar[1].timeLeft = (7) * 30;
	mTimerBar[2].timeLeft = (7 - 3) * 30;
	mTimerBar[3].timeLeft = (7) * 30;

	for (int i = 0; i < 4; i++) {
		mTimerBar[i].red = 0;
		mTimerBar[i].totalTime = 7;
		//mTimerBar[i].timeLeft = mTimerBar[i].totalTime * 30;
		mTimerBar[i].primId = fbl_create_prim(FBL_RECT, mTimerBar[i].x, mTimerBar[i].y, mTimerBar[i].timeLeft, 5, 0, false, true);
		fbl_set_prim_color(mTimerBar[i].primId, 0, 255, 0, 255);
	}

	// create 3 red crosses for failure
	for (int i = 0; i < 3; i++) {
		mFailCrossId[i] = fbl_create_sprite(192, 480, 32, 32, 0);
		fbl_set_sprite_xy(mFailCrossId[i], Game::DeviceResW - (226 - 32 * i), 20);
		fbl_set_sprite_active(mFailCrossId[i], false);
	}

	
	setupAirPressure(pos[0].x, pos[0].y);
	setupColorCables(pos[1].x, pos[1].y);
	setupCalcChecksum(pos[2].x, pos[2].y);
	setupSequencer(pos[3].x, pos[3].y);

	/*
	setupAirPressure(Game::DeviceResW / 4 + 64, 30);
	setupColorCables(25, 400);
	setupCalcChecksum(Game::DeviceResW / 2, 0);
	setupSequencer(Game::DeviceResW / 2, Game::DeviceResH / 2);
	*/


	// intro black bg and MAINTENANCE text

	// black bg
	/*
	mBlackBgId = fbl_create_prim(FBL_NORMAL_RECT, 0, 0, Game::DeviceResW, Game::DeviceResH, 0, 0, 1);
	fbl_set_prim_color(mBlackBgId, 0, 0, 0, 255);
	mBlackBgFade = 255;

	// maintenance text
	fbl_load_ttf_font("font/garamond.ttf", 48);
	mMaintenanceTextId = fbl_create_text(255, 69, 0, 255, (char*)"MAINTENANCE MODE!");
	fbl_set_text_align(mMaintenanceTextId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(mMaintenanceTextId, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3);
	*/

	// wait a couple of seconds before start
	mStartTimer = cCheckTimeLimit * 5;

	fbl_load_ttf_font("font/roboto.ttf", 20);
	mOpsId = fbl_create_text(255, 255, 255, 255, "Ops left: %d", mOpsLeft);
	fbl_set_text_xy(mOpsId, Game::DeviceResW - 120, 20);

}

void Maintenance::setupAirPressure(int x, int y) {

	int tmpId;

	// create air pressure minigame ui
	// meter
	mAirMeter.x = x + 304;
	mAirMeter.y = y + 30;
	mAirMeter.meterId = fbl_create_prim(FBL_NORMAL_RECT, mAirMeter.x, mAirMeter.y + 6, 32, 198, 0, false, true);
	fbl_set_prim_color(mAirMeter.meterId, 11, 168, 230, 255);	// sky blue
	// sweet spot
	mAirMeter.sweetSpotY = mAirMeter.y + 100;
	mAirMeter.sweetSpotSize = 30;
	if (mTotalOps > 20)
		mAirMeter.sweetSpotSize = 20;
	mAirMeter.pointerY = mAirMeter.sweetSpotY + mAirMeter.sweetSpotSize / 2;
	mAirMeter.speed = static_cast<float>(rand() % 4 + 6) / 10;
	if (mTotalOps > 20)
		mAirMeter.speed = static_cast<float>(rand() % 5 + 10) / 10;
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
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Air Pressure Tuning:");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mAirMeter.x - 279, mAirMeter.y);
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
	mShortCut[0] = fbl_create_text(255, 255, 255, 0, (char*)"Up->");
	fbl_set_text_align(mShortCut[0], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[0], mAirMeter.x + 115, mAirMeter.y + 70);
	mShortCut[1] = fbl_create_text(255, 255, 255, 0, (char*)"Down->");
	fbl_set_text_align(mShortCut[1], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[1], mAirMeter.x + 114, mAirMeter.y + 120);

}

void Maintenance::setupColorCables(int x, int y) {

	int tmpId;

	// memorize and match colored cables
	mColorCables.x = x + 25;
	mColorCables.y = y + 130;
	mColorCables.color[0] = 0;
	mColorCables.color[1] = 1;
	mColorCables.color[2] = 2;
	mColorCables.mimicColor[0] = 0;
	mColorCables.mimicColor[1] = 1;
	mColorCables.mimicColor[2] = 2;
	mColorCables.checkDuration = 0;
	mColorCables.lineId[0] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[0], 255, 100, 30, 255);	// orange
	mColorCables.lineId[1] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y + 32, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[1], 255, 255, 255, 255); // white
	mColorCables.lineId[2] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x, mColorCables.y + 64, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.lineId[2], 0, 83, 255, 255);	// blue

	mColorCables.mimicLineId[0] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[0], 255, 100, 30, 255); // orange
	mColorCables.mimicLineId[1] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y + 32, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[1], 255, 255, 255, 255); // white
	mColorCables.mimicLineId[2] = fbl_create_prim(FBL_NORMAL_RECT, mColorCables.x + 200, mColorCables.y + 64, 120, 7, 0, false, true);
	fbl_set_prim_color(mColorCables.mimicLineId[2], 0, 83, 255, 255); // blue

	// Color cable buttons
	for (int i = 0; i < 3; i++) {
		mColorCables.button[i] = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
		fbl_set_ui_elem_access(mColorCables.button[i], cKeyDelay);
		fbl_set_ui_elem_xy(mColorCables.button[i], mColorCables.x + 360, mColorCables.y - 2 + (37 * i));
	}

	// Color cables instructions
	fbl_load_ttf_font("font/roboto.ttf", 20);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Match Colored Cables:");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 99);
	fbl_load_ttf_font("font/roboto.ttf", 16);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Match the colors of the cables");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 62);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"before the time runs out.");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mColorCables.x, mColorCables.y - 32);
	// shortcut keys
	mShortCut[2] = fbl_create_text(255, 255, 255, 0, (char*)"A");
	fbl_set_text_align(mShortCut[2], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[2], mColorCables.x + 390, mColorCables.y - 2);
	mShortCut[3] = fbl_create_text(255, 255, 255, 0, (char*)"S");
	fbl_set_text_align(mShortCut[3], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[3], mColorCables.x + 390, mColorCables.y + 35);
	mShortCut[4] = fbl_create_text(255, 255, 255, 0, (char*)"D");
	fbl_set_text_align(mShortCut[4], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[4], mColorCables.x + 390, mColorCables.y + 72);


}

void Maintenance::setupCalcChecksum(int x, int y) {

	int tmpId;

	mCalc.x = x;
	mCalc.y = y;

	mCalc.checkDuration = 0;

	// the calc
	fbl_load_ttf_font("font/roboto.ttf", 20);
	mCalc.calcTextId = fbl_create_text(255, 255, 255, 255, "1 + 1 = 2");
	fbl_set_text_xy(mCalc.calcTextId, mCalc.x + 150, mCalc.y + 145);

	// alternative text
	for (int i = 0; i < 3; i++) {
		mCalc.altTextId[i] = fbl_create_text(255, 255, 255, 255, "%d + 1", i);
		fbl_set_text_xy(mCalc.altTextId[i], mCalc.x + 255, mCalc.y + 105 + (37 * i));
	}

	// buttons
	for (int i = 0; i < 3; i++) {
		mCalc.button[i] = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
		fbl_set_ui_elem_xy(mCalc.button[i], mCalc.x + 320, mCalc.y + 105 + (37 * i));
	}

	// Color cables instructions
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Calculate Checksum:");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mCalc.x + 20, mCalc.y + 30);
	fbl_load_ttf_font("font/roboto.ttf", 16);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Select the correct solution");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mCalc.x + 20, mCalc.y + 70);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"from the alternatives.");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mCalc.x + 20, mCalc.y + 100);
	// shortcut keys
	mShortCut[5] = fbl_create_text(255, 255, 255, 0, (char*)"Q");
	fbl_set_text_align(mShortCut[5], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[5], mCalc.x + 350, mCalc.y + 105);
	mShortCut[6] = fbl_create_text(255, 255, 255, 0, (char*)"W");
	fbl_set_text_align(mShortCut[6], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[6], mCalc.x + 350, mCalc.y + 142);
	mShortCut[7] = fbl_create_text(255, 255, 255, 0, (char*)"E");
	fbl_set_text_align(mShortCut[7], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[7], mCalc.x + 350, mCalc.y + 179);

	genCalc();


}

void Maintenance::setupSequencer(int x, int y) {

	int tmpId;

	mSeq.x = x;
	mSeq.y = y;
	mSeq.checkDuration = 0;

	// sequence arrows
	for (int i = 0; i < 5; i++) {
		mSeq.seqId[i] = fbl_create_sprite(256, 304, 16, 16, 0);
		fbl_set_sprite_xy(mSeq.seqId[i], mSeq.x + 100 + (20 * i), mSeq.y + 150);
	}

	// mimic arrows
	for (int i = 0; i < 5; i++) {
		mSeq.mimicSeqId[i] = fbl_create_sprite(256, 304, 16, 16, 0);
		fbl_set_sprite_xy(mSeq.mimicSeqId[i], mSeq.x + 100 + (20 * i), mSeq.y + 180);
		fbl_set_sprite_active(mSeq.mimicSeqId[i], false);
	}


	mSeq.arrowLeftId = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 128, 192, 32, 32, NULL);
	fbl_set_ui_elem_access(mSeq.arrowLeftId, cKeyDelay);
	fbl_set_ui_elem_xy(mSeq.arrowLeftId, mSeq.x + 296, mSeq.y + 163);
	mSeq.arrowRightId = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 128, 160, 32, 32, NULL);
	fbl_set_ui_elem_access(mSeq.arrowRightId, cKeyDelay);
	fbl_set_ui_elem_xy(mSeq.arrowRightId, mSeq.x + 340, mSeq.y + 163);

	// Sequencer instructions
	fbl_load_ttf_font("font/roboto.ttf", 20);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Sequencer:");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mSeq.x + 20, mSeq.y + 30);
	fbl_load_ttf_font("font/roboto.ttf", 16);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Repeat the right/left sequence in order");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mSeq.x + 20, mSeq.y + 70);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"using the arrows.");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, mSeq.x + 20, mSeq.y + 100);

	// shortcut keys
	mShortCut[8] = fbl_create_text(255, 255, 255, 0, (char*)"<- left");
	fbl_set_text_align(mShortCut[8], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[8], mSeq.x + 267, mSeq.y + 194);
	mShortCut[9] = fbl_create_text(255, 255, 255, 0, (char*)"right ->");
	fbl_set_text_align(mShortCut[9], FBL_ALIGN_LEFT);
	fbl_set_text_xy(mShortCut[9], mSeq.x + 331, mSeq.y + 194);

	//fbl_load_ttf_font("font/roboto.ttf", 20);

	randomizeSequence();

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
	if (mAirMeter.pointerY < pos[0].y + 36) mAirMeter.pointerY = pos[0].y + 36;
	if (mAirMeter.pointerY > pos[0].y + 233) mAirMeter.pointerY = pos[0].y + 233;
	fbl_set_sprite_xy(mAirMeter.pointerId, mAirMeter.x - 15, mAirMeter.pointerY);

	// move the sweet spot, limit values and change dir
	if (mAirMeter.checkDuration == 0) {
		mAirMeter.sweetSpotY += mAirMeter.speed;
		if (mAirMeter.sweetSpotY > pos[0].y + 234 - mAirMeter.sweetSpotSize) mAirMeter.speed = -mAirMeter.speed;
		if (mAirMeter.sweetSpotY < pos[0].y + 37) mAirMeter.speed = -mAirMeter.speed;
		fbl_set_prim_xy(mAirMeter.sweetSpotId, mAirMeter.x, mAirMeter.sweetSpotY);
	}

	// check the pointer when the timer is done
	if (mTimerBar[0].timeLeft > cTimeStep) return;

	// times up!
	int y = fbl_get_sprite_y(mAirMeter.pointerId);
	if ((y + 1) >= mAirMeter.sweetSpotY && y <= mAirMeter.sweetSpotY + mAirMeter.sweetSpotSize) {

		// make sweet spot green for a while
		fbl_set_prim_color(mAirMeter.sweetSpotId, 0, 220, 0, 255);
		mAirMeter.checkDuration = cCheckTimeLimit;

		advance();

	}
	else {

		fbl_set_prim_color(mAirMeter.sweetSpotId, 220, 0, 0, 255);
		mAirMeter.checkDuration = cCheckTimeLimit;

		fail(g);

	}

}

void Maintenance::processColorCables(Game& g) {

	mColorCables.checkDuration--;

	if (mColorCables.checkDuration < 0)
		mColorCables.checkDuration = 0;

	if (mColorCables.checkDuration == 1) {

		// put back to where the colors were
		updateCableColors(0, true);
		updateCableColors(1, true);
		updateCableColors(2, true);

	}

	if (mTimerBar[2].timeLeft > cTimeStep) return;	// skip if time left

	// check if cables match

	bool match = true;
	for (int i = 0; i < 3; i++)
		if (mColorCables.color[i] != mColorCables.mimicColor[i])
			match = false;

	if (match) {

		for (int i = 0; i < 3; i++)
			fbl_set_prim_color(mColorCables.mimicLineId[i], 0, 220, 0, 255);

		mColorCables.checkDuration = cCheckTimeLimit;

		advance();

	}
	else {

		for(int i = 0; i < 3; i++)
			fbl_set_prim_color(mColorCables.mimicLineId[i], 220, 0, 0, 255);

		mColorCables.checkDuration = cCheckTimeLimit;

		fail(g);

	}

	// randomize colors
	for (int i = 0; i < 3; i++) {
		mColorCables.color[i] = rand() % 3;
		updateCableColors(i, false);
	}

}

void Maintenance::processCalcChecksum(Game& g) {

	mCalc.checkDuration--;

	if (mCalc.checkDuration < 0)
		mCalc.checkDuration = 0;

	if (mCalc.checkDuration == 1) {

		// put back to where the colors were (white)

		fbl_update_text(mCalc.altTextId[0], 255, 255, 255, 255, "%d", mCalc.finalAlt[0]);
		fbl_update_text(mCalc.altTextId[1], 255, 255, 255, 255, "%d", mCalc.finalAlt[1]);
		fbl_update_text(mCalc.altTextId[2], 255, 255, 255, 255, "%d", mCalc.finalAlt[2]);

		genCalc();

	}

	if (mTimerBar[1].timeLeft > cTimeStep) return;	// skip if time left


	fail(g);

	// generate new calculation
	genCalc();

}

void Maintenance::processSequencer(Game& g) {

	if (mSeq.checkDuration == cCheckTimeLimit) {

		//std::cout << "CHECKING SEQUENCE" << std::endl;

		// check if sequence is correct
		bool correct = true;
		for (int i = 0; i < 5; i++) {
			if (mSeq.seq[i] != mSeq.mimicSeq[i]) {
				correct = false;
				break;
			}
		}

		if (correct) {

			// make arrows green
			for (int i = 0; i < 5; i++)
				fbl_set_sprite_color(mSeq.mimicSeqId[i], 0, 220, 0);

			mTimerBar[3].timeLeft = mTimerBar[3].totalTime * 30;
			advance();
		}
		else {

			// make arrows red
			for (int i = 0; i < 5; i++)
				fbl_set_sprite_color(mSeq.mimicSeqId[i], 220, 0, 0);

			mTimerBar[3].timeLeft = mTimerBar[3].totalTime * 30;
			fail(g);
		}


	}

	mSeq.checkDuration--;

	if (mSeq.checkDuration < 0)
		mSeq.checkDuration = 0;

	if (mSeq.checkDuration == 1) {

		// put back to where the colors were (white)

		for (int i = 0; i < 5; i++)
			fbl_set_sprite_color(mSeq.mimicSeqId[i], 255, 255, 255);

		randomizeSequence();

	}

	if (mTimerBar[3].timeLeft > cTimeStep) return;	// skip if time left


	fail(g);

	// generate new sequence
	randomizeSequence();



















}

void Maintenance::updateCableColors(int index, bool mimic) {

	if (mimic) {
		if (mColorCables.mimicColor[index] == 0)
			fbl_set_prim_color(mColorCables.mimicLineId[index], 255, 100, 30, 255);
		if (mColorCables.mimicColor[index] == 1)
			fbl_set_prim_color(mColorCables.mimicLineId[index], 255, 255, 255, 255);
		if (mColorCables.mimicColor[index] == 2)
			fbl_set_prim_color(mColorCables.mimicLineId[index], 0, 83, 255, 255);
	}
	else {
		if (mColorCables.color[index] == 0)
			fbl_set_prim_color(mColorCables.lineId[index], 255, 100, 30, 255);
		if (mColorCables.color[index] == 1)
			fbl_set_prim_color(mColorCables.lineId[index], 255, 255, 255, 255);
		if (mColorCables.color[index] == 2)
			fbl_set_prim_color(mColorCables.lineId[index], 0, 83, 255, 255);
	}
}

void Maintenance::genCalc() {

	// Generate a random operation (addition, subtraction, multiplication, division)
	mCalc.operation = randNum(0, 3);

	// Generate random operands
	switch (mCalc.operation) {
		case 0:
			mCalc.operand1 = randNum(1, 20);
			mCalc.operand2 = randNum(1, 20);
			break;
		case 1:
			mCalc.operand1 = randNum(1, 20);
			mCalc.operand2 = randNum(1, 20);
			break;
		case 2:
			mCalc.operand1 = randNum(1, 11);
			mCalc.operand2 = randNum(1, 11);
			break;
		case 3:
			do {
				mCalc.operand1 = randNum(1, 100);
				mCalc.operand2 = randNum(1, 10);
			} while (mCalc.operand1 % mCalc.operand2 != 0);
			break;
	}

	// calc correct answer
	switch (mCalc.operation) {
	case 0:
		mCalc.correctAnswer = mCalc.operand1 + mCalc.operand2;
		mCalc.operationChar = '+';
		break;
	case 1:
		mCalc.correctAnswer = mCalc.operand1 - mCalc.operand2;
		mCalc.operationChar = '-';
		break;
	case 2:
		mCalc.correctAnswer = mCalc.operand1 * mCalc.operand2;
		mCalc.operationChar = '*';
		break;
	case 3:
		mCalc.correctAnswer = mCalc.operand1 / mCalc.operand2;
		mCalc.operationChar = '/';
		break;
	}

	// make alternatives
	switch (mCalc.operation) {
	case 0:
	case 1:
		genAddSubAlt(mCalc.correctAnswer, mCalc.alt1, mCalc.alt2);
		break;
	case 2:
	case 3:
		genMulDivAlt(mCalc.correctAnswer, mCalc.alt1, mCalc.alt2);
		break;
	}

	mCalc.finalAlt[0] = mCalc.correctAnswer;
	mCalc.finalAlt[1] = mCalc.alt1;
	mCalc.finalAlt[2] = mCalc.alt2;

	// shuffle the alternatives
	std::random_shuffle(mCalc.finalAlt, mCalc.finalAlt + 3);

	fbl_update_text(mCalc.calcTextId, 255, 255, 255, 255, "%d %c %d =", mCalc.operand1, mCalc.operationChar, mCalc.operand2);
	fbl_update_text(mCalc.altTextId[0], 255, 255, 255, 255, "%d", mCalc.finalAlt[0]);
	fbl_update_text(mCalc.altTextId[1], 255, 255, 255, 255, "%d", mCalc.finalAlt[1]);
	fbl_update_text(mCalc.altTextId[2], 255, 255, 255, 255, "%d", mCalc.finalAlt[2]);


	std::cout << "genCalc(): " << mCalc.operand1 << mCalc.operationChar << mCalc.operand2 << " = " << mCalc.finalAlt[0] << " or " << mCalc.finalAlt[1] << " or " << mCalc.finalAlt[2] << std::endl;

}

int Maintenance::randNum(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void Maintenance::genAddSubAlt(int correct, int& alt1, int& alt2) {
	alt1 = correct + randNum(1, 10);
	alt2 = correct - randNum(1, 10);
}

void Maintenance::genMulDivAlt(int correct, int& alt1, int& alt2) {
	alt1 = correct * randNum(2, 5);
	alt2 = correct / randNum(2, 5);
}

void Maintenance::randomizeSequence() {

	// 0 = left, 1 = right

	for (int i = 0; i < 5; i++) {
		mSeq.seq[i] = rand() % 2;

		if (mSeq.seq[i] == 0) {
			if (fbl_get_sprite_flip(mSeq.seqId[i]) == 0)
				fbl_set_sprite_flip(mSeq.seqId[i], 1);
		}
		if (mSeq.seq[i] == 1) {
			if (fbl_get_sprite_flip(mSeq.seqId[i]) == 1)
				fbl_set_sprite_flip(mSeq.seqId[i], 0);
		}


		// set mimic seq to -1 and deactivate sprites
		mSeq.mimicSeq[i] = -1;
		fbl_set_sprite_active(mSeq.mimicSeqId[i], false);

	}

















}

void Maintenance::upDifficulty() {

	// every 5 successful op lower the max time by 1 sec

	if (mTotalOps - mOpsLeft == 10 ||
		mTotalOps - mOpsLeft == 15 ||
		mTotalOps - mOpsLeft == 20) {

		for (int i = 0; i < 4; i++) {
			mTimerBar[i].totalTime--;
			mTimerBar[i].timeLeft = mTimerBar[i].totalTime * 30;
		}

		SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxTurbo, SoundManager::Channel::Turbo, 0);

	}
		

}

void Maintenance::processTimers() {


	for (int i = 0; i < 4; i++) {
		mTimerBar[i].timeLeft -= cTimeStep;
		if (mTimerBar[i].timeLeft < cTimeStep) {
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

void Maintenance::advance() {

	mOpsLeft--;
	if (mOpsLeft < 0) mOpsLeft = 0;

	// update the text
	fbl_update_text(mOpsId, 255, 255, 255, 255, "Ops left: %d ", mOpsLeft);

	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxPass, SoundManager::Channel::Ui, 0);

	upDifficulty();

}

void Maintenance::fail(Game& g) {

	mFails++;
	if (mFails > 3) mFails = 3;

	fbl_set_sprite_active(mFailCrossId[mFails - 1], true);

	if (mFails > 2) {

		SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxExplosion, SoundManager::Channel::Expl, 0);

		if (g.mRobots->ownedRobotsLeft(g) > 1) {

			auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[0]);
			sta.hp = 0;

			Race::sRaceState = Fourth;

		}
		else {
			for (int i = 0; i < 10; i++) fbl_set_text_active(mShortCut[i], false);	// remove the letters
			Race::sRaceState = Dead;	// oh yes!
		}

	}

	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxDenied, SoundManager::Channel::Shield, 0);
	Efx::getInstance().shakeCamera(20, 40);

}

void Maintenance::checkWinCondition() {

	if (mOpsLeft > 0) return;

	//Race::sRaceState = mFails + 1;

	switch (mFails) {

		case 0:
			Race::sRaceState = First;
			break;
		case 1:
			Race::sRaceState = Second;
			break;
		case 2:
			Race::sRaceState = Third;
			break;

	}

	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxShutdown, SoundManager::Channel::Expl, 0);

}

void Maintenance::hideSprites() {

	// fail red cross
	for (int i = 0; i < 3; i++)
		fbl_set_sprite_active(mFailCrossId[i], false);

	// the air pressure pointer
	fbl_set_sprite_active(mAirMeter.pointerId, false);

	// sequence arrows
	for (int i = 0; i < 5; i++) {
		fbl_set_sprite_active(mSeq.seqId[i], false);
		fbl_set_sprite_active(mSeq.mimicSeqId[i], false);
	}

}

void Maintenance::getInput(Game& g) {

	int key;

	//Efx::getInstance().shakeCamera(10, 10);

	// Air pressure input
	if (fbl_get_ui_elem_val(mAirMeter.arrowUpId) || fbl_get_key_down(FBLK_UP)) {
		if (mAirMeter.checkDuration == 0)
			mAirMeter.pointerY -= 2;
	}
	if (fbl_get_ui_elem_val(mAirMeter.arrowDownId) || fbl_get_key_down(FBLK_DOWN)) {
		if (mAirMeter.checkDuration == 0)
			mAirMeter.pointerY += 2;
	}

	// color cables input
	for (int i = 0; i < 3; i++) {

		if (i == 0) key = FBLK_A;
		if (i == 1) key = FBLK_S;
		if (i == 2) key = FBLK_D;

		if (fbl_get_ui_elem_val(mColorCables.button[i]) || (fbl_get_key_down(key) && mKeyDelayLeft[i] == 0)) {
			if (mColorCables.checkDuration == 0) {
				mColorCables.mimicColor[i]++;
				if (mColorCables.mimicColor[i] > 2)
					mColorCables.mimicColor[i] = 0;

				updateCableColors(i, true);
				mKeyDelayLeft[i] = cKeyDelay;
			}
		}

	}

	// Calc checksum input
	for (int i = 0; i < 3; i++) {

		if (i == 0) key = FBLK_Q;
		if (i == 1) key = FBLK_W;
		if (i == 2) key = FBLK_E;

		if (fbl_get_ui_elem_val(mCalc.button[i]) || (fbl_get_key_down(key) && mKeyDelayLeft[i] == 0)) {
			if (mCalc.checkDuration == 0) {
				if (mCalc.finalAlt[i] == mCalc.correctAnswer) {
					mTimerBar[1].timeLeft = mTimerBar[1].totalTime * 30;
					fbl_update_text(mCalc.altTextId[i], 0, 220, 0, 255, "%d", mCalc.finalAlt[i]);
					advance();
				}
				else {
					mTimerBar[1].timeLeft = mTimerBar[1].totalTime * 30;
					fbl_update_text(mCalc.altTextId[i], 220, 0, 0, 255, "%d", mCalc.finalAlt[i]);
					fail(g);
				}
				mCalc.checkDuration = cCheckTimeLimit;
				mKeyDelayLeft[0] = cKeyDelay;
			}

		}

	}

	// Sequencer input
	if (fbl_get_ui_elem_val(mSeq.arrowLeftId) || fbl_get_key_down(FBLK_LEFT) && mKeyDelayLeft[0] == 0) {
		if (mSeq.checkDuration == 0) {

			// find next mimic arrow
			int i;
			for (i = 0; i < 4; i++)
				if (mSeq.mimicSeq[i] == -1) break;

			std::cout << "Next free mimic arrow index = " << i << std::endl;

			mSeq.mimicSeq[i] = 0;	// 0 = left
			if (fbl_get_sprite_flip(mSeq.mimicSeqId[i]) == 0)
				fbl_set_sprite_flip(mSeq.mimicSeqId[i], 1);

			// activate the sprite
			fbl_set_sprite_active(mSeq.mimicSeqId[i], true);

			mKeyDelayLeft[0] = cKeyDelay;
			if(i == 4) mSeq.checkDuration = cCheckTimeLimit;

		}

	}
	if (fbl_get_ui_elem_val(mSeq.arrowRightId) || fbl_get_key_down(FBLK_RIGHT) && mKeyDelayLeft[1] == 0) {
		if (mSeq.checkDuration == 0) {

			// find next mimic arrow
			int i;
			for (i = 0; i < 4; i++)
				if (mSeq.mimicSeq[i] == -1) break;

			std::cout << "Next free mimic arrow index = " << i << std::endl;

			mSeq.mimicSeq[i] = 1;	// 1 = right
			if (fbl_get_sprite_flip(mSeq.mimicSeqId[i]) == 1)
				fbl_set_sprite_flip(mSeq.mimicSeqId[i], 0);

			// activate the sprite
			fbl_set_sprite_active(mSeq.mimicSeqId[i], true);

			mKeyDelayLeft[1] = cKeyDelay;
			if (i == 4) mSeq.checkDuration = cCheckTimeLimit;

		}

	}

	for (int i = 0; i < 3; i++) {
		mKeyDelayLeft[i]--;
		if (mKeyDelayLeft[i] < 0) mKeyDelayLeft[i] = 0;
	}

}

void Maintenance::tick(Game& g) {

	mStartTimer--;
	if (mStartTimer < 0) mStartTimer = 0;
	if (mStartTimer > 0) return;

	if (Race::sRaceState == Undecided) {
		getInput(g);
		processAirPressure(g);
		processColorCables(g);
		processCalcChecksum(g);
		processSequencer(g);
		processTimers();
		checkWinCondition();
	}
	else {
		mPostRaceDelay--;
		if (mPostRaceDelay == 0) {
			if (Race::sRaceState == Dead) {
				fbl_destroy_all_ui_elems();
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
