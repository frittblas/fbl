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

	mMaze->exitMaze();

	delete mMaze;

	std::cout << "Destroyed Race state." << std::endl;

}

void Race::assignRobots(Game& g) {


	// for now just assign the robots we have (should be assigned fom teams and loop to find != Unassigned from mAll)

	g.mRobots->mRacingRobots[0] = g.mRobots->mOwnedRobots[Robots::Charmy];

	g.mRobots->mRacingRobots[1] = g.mRobots->mOwnedRobots[Robots::Alarmy];
	g.mRobots->mRacingRobots[2] = g.mRobots->mOwnedRobots[Robots::Boingy];
	g.mRobots->mRacingRobots[3] = g.mRobots->mOwnedRobots[Robots::Chompy];

	g.mRobots->showRobotInRace(g.mEcs, Robots::Charmy, 1);
	g.mRobots->showRobotInRace(g.mEcs, Robots::Alarmy, 2);
	g.mRobots->showRobotInRace(g.mEcs, Robots::Boingy, 3);
	g.mRobots->showRobotInRace(g.mEcs, Robots::Chompy, 4);

	mNumRacers = 4;

	mMaze->initMaze(g, 30, mNumRacers);

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

	// add mousectrl to a robot IF it has the skill!!!! (just testing now)
															  // clicked
	//g.mEcs->AddComponent(g.mRobots->mRacingRobots[0], MouseCtrl{ false });

}

void Race::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(g);					// update the path system, note the g as argument
	g.mSysManager->mMouseCtrlSystem->Update(*g.mEcs);		// update the mouse control system
	//g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system

	//g.mSysManager->mLightSystem->Update(*g.mEcs);			// update the light system

	//g.mWeather->tick();

	mMaze->tick(g);

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick race!" << std::endl;

}
