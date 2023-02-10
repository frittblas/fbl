/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	GameState.cpp
*
*	GameState class implementation, state maching dealing with game states.
*
*	Hans Strömquist 2022
*
*/

#include "../../../src/fbl.hpp"

#include "../Game.hpp"

#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"
#include "../Ecs/Systems/CameraSystem.hpp"
#include "../Ecs/Systems/LightSystem.hpp"

#include "../Ecs/Systems/Race/AutoAimSystem.hpp"
#include "../Ecs/Systems/Race/LaserSystem.hpp"

#include "GameState.hpp"
#include "Title.hpp"
#include "Settings.hpp"
#include "Explore.hpp"
#include "Dialogue.hpp"
#include "RobotCollection.hpp"
#include "Race/Race.hpp"

#include "../Chars.hpp"
#include "../Robots.hpp"
#include "../Addons.hpp"
#include "../Location.hpp"
#include "../Weather.hpp"
#include "../SysManager.hpp"
#include "../LuaDialogue.hpp"

// GameState-class implementation

GameState::GameState() {

	mState = StateType::Title;

	mCurrentStateInstance = new Title();	// Start by default to title screen

}

GameState::~GameState() {

	// delete the current state instance

	delete mCurrentStateInstance;

}

void GameState::change(Game& g, StateType newState) {

	// first deallocate any resources that are in play

	delete mCurrentStateInstance;	// delete the current state instance, calling the correct destructor

	// then set the new state and allocate resources for the new state

	switch (newState) {

		case StateType::Title:

			if (mState == StateType::Explore) {	// if coming from explore state
				exploreToTitle(g);
			}
			if (mState == StateType::RobotCollection) {	// if coming from Robot menu
				exploreToTitle(g);	// works, bc all resources are destroyed and that's what we want :)
			}

			mCurrentStateInstance = new Title();

			break;

		case StateType::Demo:
			break;

		case StateType::Settings:

			mCurrentStateInstance = new Settings();

			break;

		case StateType::Tutorial:
			break;

		case StateType::Explore:

			if (mState == StateType::Title) {	// if coming from title (new game)
				titleToExplore(g);
			}
			else if (mState == StateType::Race) {	// if coming from a race
				raceToExplore(g);
			}
			else if (mState == StateType::Dialogue) {	// if coming from dialogue
			}

			g.mRobots->hideRobots(g.mEcs);		 // don't show the robot-sprites in explore mode (or in beginning of race)
			g.mAddons->hideAddons(g.mEcs);		 // don't show addons either (ui buttons)
			fbl_sort_sprites(FBL_SORT_BY_LAYER); // layers : Ground tiles 0, Player 1, Tunnel tiles(pl. goes under) 2, Clouds 3, Gray colMenu BG 4
												 // layers contd. white robotBg 5, powerups 6, robots 7.
			mCurrentStateInstance = new Explore();

			fbl_load_ttf_font("font/garamond.ttf", 22);	// use this font for explore mode (and dialogue)

			break;

		case StateType::Dialogue:


			// temporarily remove component from the player
			//g.mEcs->RemoveComponent<Path>(g.mChars->mBrodo);

			mCurrentStateInstance = new Dialogue();

			break;

		case StateType::Shop:
			break;

		case StateType::Race:

			setupRace(g);

			{
				Race* race = new Race();
				race->assignRobots(g);
				mCurrentStateInstance = race;
			}

			break;

		case StateType::RobotCollection:
			g.mRobots->showRobotInMenu(g.mEcs, Robots::Name::Charmy);	// make the robots visible in the menu
			g.mAddons->showAddonsInMenu(g.mEcs);						// same with addons

			RobotCollection* rc = new RobotCollection();
			fbl_load_ttf_font("font/roboto.ttf", 18);	// use this font for the menu
			rc->cyclePages(g, 0);			// call this to update the first robots stats in the menu
			rc->updateAddonInfo(g, true);	// update addon info to empty
			mCurrentStateInstance = rc;

			break;

	}

	// finally set the new state
	mState = newState;

}

GameState::StateType GameState::get() {

	return mState;
}

void GameState::tick(Game& g) {

	mCurrentStateInstance->tick(g);

}

void GameState::exploreToTitle(Game& g) {

	destroyAllGfx();				 // remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->unLoadLocation(g.mMap);
	g.mChars->removePlayer(g.mEcs);	 // delete the player completely
	g.mChars->removeNpc(g.mEcs);	 // also delete all npcs in the current scene
	g.mRobots->removeRobots(g.mEcs); // delete all the robots
	g.mAddons->removeAddons(g.mEcs); // delete all addons
	g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 6, 0, false);	// timeOfDay, rainLevel, snowLevel, numClouds, lightningOn
	fbl_lua_shutdown();				 // so the dialogues gets reset

}

void GameState::titleToExplore(Game& g) {

	destroyAllGfx();				// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->loadLocation(g.mMap);
	initLuaDialog();				// set up prims and text and ui for the dialog box.
	g.mChars->setupPlayer(g.mEcs);	// create the player entity and add the right components
	g.mChars->setupNpc(g);			// add all npcs based on the map file

	g.mRobots->setupRobots(g.mEcs); // create the robot entities and add the basic components
	g.mAddons->setupAddons(g.mEcs); // create the addon entities

	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component
	//g.mSysManager->mCameraSystem->Init(*g.mEcs);	// creates debug rect for camera deadzone
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 50, true);

	initCollectionMenu();	// set up prims and text and ui for the collection-menu, sprite draw-order is important
	g.mAddons->initAddons(g.mEcs); // create the addon ui elements (buttons)

	fbl_lua_init("Ca2Dialogue.lua", registerFuncsToLua);	// set this up each new game, so the dialogues restart

	std::cout << "player id: " << g.mChars->mBrodo << std::endl;

}

void GameState::raceToExplore(Game& g) {

	destroyAllGfx();					// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->loadLocation(g.mMap);	// this will destroy all sprites, then load map
	initLuaDialog();					// set up prims and text and ui for the dialog box.
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	// remove the path components from the racing robots
	for(int i = 0; i < g.mRobots->mNumRacers; i++)
		g.mEcs->RemoveComponent<Path>(g.mRobots->mRacingRobots[i]);

	// add path component back to the player
	g.mEcs->AddComponent(g.mChars->mBrodo, Path{ 0, 0, 0, false, 2.0, FBL_PATHF_USE_DIAG, 1 });
	//g.mEcs->AddComponent(g.mChars->mBrodo, MouseCtrl{ false });

	// temporarily remove mousectrl component from a robot
	//g.mEcs->RemoveComponent<MouseCtrl>(g.mRobots->mRacingRobots[0]);

	g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 50, true);

	initCollectionMenu();	// set up prims and text and ui for the collection-menu, sprite draw-order is important
	g.mAddons->initAddons(g.mEcs); // create the addon ui elements (buttons)


}

void GameState::setupRace(Game& g) {

	destroyAllGfx();								// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->unLoadLocation(g.mMap);			// this destroys ALL sprites
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component
	g.mSysManager->mAutoAimSystem->Init(*g.mEcs);	// create rays for entities with AutoAim  component.
	g.mSysManager->mLaserSystem->Init(*g.mEcs);		// create rays and particles for all entities with a Laser component.
	g.mRobots->mapSpriteIdToEntity(g.mEcs);
	g.mRobots->hideRobots(g.mEcs);

	// temporarily remove path component from the player
	g.mEcs->RemoveComponent<Path>(g.mChars->mBrodo);
	g.mChars->hidePlayer(g.mEcs);

	// hide the player light
	auto& light = g.mEcs->GetComponent<Light>(g.mChars->mBrodo);
	fbl_set_sprite_active(light.id, false);


	g.mWeather->setWeather(Weather::TimeOfDay::Evening, 0, 0, 0, false);

}
