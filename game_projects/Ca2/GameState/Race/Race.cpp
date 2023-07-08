/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Race.cpp
*
*   Race class implementation, takes care of the Racing game-state.
*
*	Hans Strömquist 2022
*
*/


#include "../../../../src/fbl.hpp"
#include "../../Ecs/Ecs.hpp"
#include "../../Ecs/Components.hpp"
#include "../../Game.hpp"
#include "../../SysManager.hpp"
#include "../../Efx.hpp"

#include "../../Ecs/Systems/SpriteSystem.hpp"
#include "../../Ecs/Systems/PathSystem.hpp"
#include "../../Ecs/Systems/LightSystem.hpp"

#include "../../Ecs/Systems/Race/PathLogicSystem.hpp"
#include "../../Ecs/Systems/Race/AutoAimSystem.hpp"
#include "../../Ecs/Systems/Race/LaserSystem.hpp"
#include "../../Ecs/Systems/Race/MagnetSystem.hpp"
#include "../../Ecs/Systems/Race/RobotCtrlSystem.hpp"

#include "../../Chars.hpp"
#include "../../Weather.hpp"
#include "../../Robots.hpp"
#include "../../Addons.hpp"

#include "Race.hpp"

// Race-class implementation

Race::Race() {

	mMaze = new Maze();

	fbl_set_camera_xy(0, 0);

	std::cout << "Started Race state." << std::endl;

}

Race::~Race() {

	mMaze->stopPathing();	// not necessary as the path components get removed
	fbl_set_sprite_align(FBL_SPRITE_ALIGN_UP_LEFT);	// in explore mode sprites are drawn from the top left

	delete mMaze;

	//fbl_destroy_all_emitters();

	std::cout << "Destroyed Race state." << std::endl;

}

void Race::assignRobots(Game& g) {


	// for now just assign the robots we have (should be assigned fom teams and loop to find != Unassigned from mAll)

	g.mRobots->mRacingRobots[0] = g.mRobots->mOwnedRobots[Robots::Charmy];
	g.mRobots->mRacingRobots[1] = g.mRobots->mOwnedRobots[Robots::Alarmy];
	g.mRobots->mRacingRobots[2] = g.mRobots->mOwnedRobots[Robots::Boingy];
	g.mRobots->mRacingRobots[3] = g.mRobots->mOwnedRobots[Robots::Chompy];

	// set one of the robots as your team-member
	g.mRobots->mTeam[0] = g.mRobots->mOwnedRobots[Robots::Charmy];

	mNumRacers = 4;
	g.mRobots->mNumRacers = mNumRacers;		// this is needed when you remove path component on the racers.

	// first show the robots so they get the correct position
	for (int i = 0; i < mNumRacers; i++)
		g.mRobots->showRobotInRace(g.mEcs, g.mRobots->mRacingRobots[i], i);

	// then hide them again (will show up after the pick countdown)
	g.mRobots->hideRobots(g.mEcs);


	// add path components to the racing robots (these are removed after the race.)
	for (int i = 0; i < mNumRacers; i++) {
		auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[i]);
		float speed = (float)sta.speed / 10;
		uint8_t diag = sta.diag ? FBL_PATHF_USE_DIAG : FBL_PATHF_NO_DIAG;
															// id gX gY newPath speed diag pixelsPerFrame
		g.mEcs->AddComponent(g.mRobots->mRacingRobots[i], Path{ 0, 0, 0, false, speed, diag, 1 }); // last param should be 10 if you wanna use speed

		// add mousectrl to a robot IF it has the skill!!!! (just testing now)
				    												  // clicked
		//g.mEcs->AddComponent(g.mRobots->mRacingRobots[0], MouseCtrl{ false });

		// reset hp and energy
		sta.hp = sta.maxHp;
		sta.energy = sta.maxEnergy;
	}

	g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component

	//std::cout << "THE NEXT SPRITE ID IS : " << fbl_create_sprite(0, 0, 1, 1, 0) << std::endl;

	int blockDensity = (rand() % 20) + 19;	// under 40 is ok under 35 is very fast

	std::cout << "Block density: " << blockDensity << std::endl;

	mMaze->initMaze(g, blockDensity, mNumRacers);

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

void Race::unassignRobots(Game& g) {


}

void Race::getInput(Game& g) {

	// get the player input in the race (push the addon buttons or ASZX on pc)

	auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[0]);

	// first handle passive addons
	for (int i = 0; i < 2; i++) {

		if (sta.slot[i] != g.mAddons->notSet) {

			auto& add = g.mEcs->GetComponent<Addon>(sta.slot[i]);
			if (fbl_get_ui_elem_val(add.uiId)) {
				handleAddons(g, add, g.mRobots->mRacingRobots[0], true);
			}
			else handleAddons(g, add, g.mRobots->mRacingRobots[0], false);

		}

	}

	// then active (user interactable) addons
	int key = FBLK_A;
	for (int i = 2; i < 6; i++) {

		if (sta.slot[i] != g.mAddons->notSet) {

			switch (i) {
				case 3: key = FBLK_S; break;
				case 4: key = FBLK_Z; break;
				case 5: key = FBLK_X; break;
			}

			auto& add = g.mEcs->GetComponent<Addon>(sta.slot[i]);
			if (fbl_get_ui_elem_val(add.uiId) || fbl_get_key_down(key)) {
				handleAddons(g, add, g.mRobots->mRacingRobots[0], true);
			}
			else handleAddons(g, add, g.mRobots->mRacingRobots[0], false);

		
		}

	}

}

void Race::handleAddons(Game& g, Addon& add, Entity playingRobot, bool on) {

	static bool laserDirChangeOk = false;

	// passive addons are just activated/deactivated, active addons are set to "fire" or not.

	switch (add.type) {

		case Addons::Type::AutoAim :
			{
				auto& aim = g.mEcs->GetComponent<AutoAim>(playingRobot);
				aim.active = on;
			}
			break;
		case Addons::Type::Laser:
			{
				auto& las = g.mEcs->GetComponent<Laser>(playingRobot);
				las.isFiring = on;

				// change laser direction every time you release (clockwise)
				if (!las.isFiring && laserDirChangeOk) {

					// rotate directions
					if (las.dir == Addons::Dir::Up) las.dir = Addons::Dir::Right;
					else if (las.dir == Addons::Dir::Right) las.dir = Addons::Dir::Down;
					else if (las.dir == Addons::Dir::Down) las.dir = Addons::Dir::Left;
					else if (las.dir == Addons::Dir::Left) las.dir = Addons::Dir::Up;
					
					laserDirChangeOk = false;
				}

				if (las.isFiring) laserDirChangeOk = true;

			}
			break;
		case Addons::Type::Magnet:
			{
				auto& mag = g.mEcs->GetComponent<Magnet>(playingRobot);
				mag.active = on;
			}
			break;

	}

}

void Race::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(g);					// update the path system, note the g as argument
	g.mSysManager->mLightSystem->Update(g);					// update the light system
	
	g.mSysManager->mPathLogicSystem->Update(g);				// update the PathLogic system
	g.mSysManager->mAutoAimSystem->Update(g);				// update the AutoAim system
	g.mSysManager->mLaserSystem->Update(g);					// update the Laser system
	g.mSysManager->mMagnetSystem->Update(g);				// update the Magnet system
	g.mSysManager->mRobotCtrlSystem->Update(*g.mEcs);		// update the robot control system

	getInput(g);

	//g.mWeather->tick();

	Efx::getInstance().tickCameraShake();

	mMaze->tick(g);

	if (fbl_get_mouse_click(FBLMB_RIGHT)) Efx::getInstance().shakeCamera(20, 40);



	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick race!" << std::endl;

}
