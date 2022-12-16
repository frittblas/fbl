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

#include "../../Ecs/Systems/SpriteSystem.hpp"
#include "../../Ecs/Systems/PathSystem.hpp"
#include "../../Ecs/Systems/MouseCtrlSystem.hpp"
#include "../../Ecs/Systems/CameraSystem.hpp"
#include "../../Ecs/Systems/LightSystem.hpp"

#include "../../Ecs/Systems/Race/LaserSystem.hpp"

#include "../../Chars.hpp"
#include "../../Weather.hpp"
#include "../../Robots.hpp"

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
		g.mEcs->AddComponent(g.mRobots->mRacingRobots[i], Path{ 0, 0, 0, false, speed, diag, 10 });

		// add mousectrl to a robot IF it has the skill!!!! (just testing now)
				    												  // clicked
		//g.mEcs->AddComponent(g.mRobots->mRacingRobots[0], MouseCtrl{ false });
	}

	g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component

	mMaze->initMaze(g, 5, mNumRacers);

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

void Race::unassignRobots(Game& g) {
}

void Race::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(g);					// update the path system, note the g as argument
	g.mSysManager->mMouseCtrlSystem->Update(*g.mEcs);		// update the mouse control system
	//g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system

	//g.mSysManager->mLightSystem->Update(*g.mEcs);			// update the light system
	
	g.mSysManager->mLaserSystem->Update(g);					// update the laser system

	//g.mWeather->tick();

	mMaze->tick(g);	// needed for the pick-start positions-"state" in the beginning of the race

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick race!" << std::endl;

}
