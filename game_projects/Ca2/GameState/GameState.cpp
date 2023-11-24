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
#include "../SoundManager.hpp"

#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"
#include "../Ecs/Systems/CameraSystem.hpp"
#include "../Ecs/Systems/LightSystem.hpp"

#include "../Ecs/Systems/Race/PathLogicSystem.hpp"
#include "../Ecs/Systems/Race/BasicAISystem.hpp"
#include "../Ecs/Systems/Race/AutoAimSystem.hpp"
#include "../Ecs/Systems/Race/LaserSystem.hpp"
#include "../Ecs/Systems/Race/MagnetSystem.hpp"
#include "../Ecs/Systems/Race/TurboSystem.hpp"
#include "../Ecs/Systems/Race/ShieldSystem.hpp"
#include "../Ecs/Systems/Race/HealSystem.hpp"
#include "../Ecs/Systems/Race/DiagSystem.hpp"
#include "../Ecs/Systems/Race/RobotCtrlSystem.hpp"

#include "GameState.hpp"
#include "Title.hpp"
#include "Settings.hpp"
#include "Explore.hpp"
#include "Dialogue.hpp"
#include "RobotCollection.hpp"

#include "Race/Race.hpp"

#include "Maintenance.hpp"

#include "../Chars.hpp"
#include "../Efx.hpp"
#include "../Robots.hpp"
#include "../Addons.hpp"
#include "../Location.hpp"
#include "../Progress.hpp"
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
				toTitle(g);
			}
			if (mState == StateType::RobotCollection) {	// if coming from Robot menu
				toTitle(g);	// works, bc all resources are destroyed and that's what we want :)
			}
			if (mState == StateType::Race) {	// if coming from Race menu
				toTitle(g);						// toTitle() works from anywhere
			}
			if (mState == StateType::Maintenance) {
				toTitle(g);
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
			else if (mState == StateType::Maintenance) {	// if coming from maintenance
				maintenanceToExplore(g);
			}
			else if (mState == StateType::Dialogue) {	// if coming from dialogue
				auto& mctrl = g.mEcs->GetComponent<MouseCtrl>(g.mChars->mBrodo);
				mctrl.access = 40;	// slight delay before move after dialogue
			}
			else if (mState == StateType::RobotCollection) {	// if coming from robotCollection
				hideCollectionMenu();
				SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxTick, SoundManager::Channel::Ui, 0);
			}

			g.mRobots->hideRobots(g.mEcs);		 // don't show the robot-sprites in explore mode (or in beginning of race)
			g.mAddons->hideAddons(g.mEcs);		 // don't show addons either (ui buttons)
			fbl_sort_sprites(FBL_SORT_BY_LAYER); // layers : Ground tiles 0, Player 1, Tunnel tiles(pl. goes under) 2, Clouds 3, Gray colMenu BG 4
												 // layers contd. white robotBg 5, powerups 6, robots 7.
			mCurrentStateInstance = new Explore();

			fbl_load_ttf_font("font/garamond.ttf", 22);	// use this font for explore mode (and dialogue)

			break;

		case StateType::Dialogue:

			mCurrentStateInstance = new Dialogue();

			break;

		case StateType::Race:

			setupRace(g);

			{
				Race* race = new Race();
				race->assignRobots(g);
				mCurrentStateInstance = race;
			}

			g.mSysManager->mPathLogicSystem->Init(*g.mEcs);		// set up path logic (flags and stuff)
			g.mSysManager->mBasicAISystem->Init(*g.mEcs);		// set up ai
			g.mSysManager->mAutoAimSystem->Init(*g.mEcs);		// create rays for entities with AutoAim  component.
			g.mSysManager->mLaserSystem->Init(*g.mEcs);			// create rays and particles for all entities with a Laser component.
			g.mSysManager->mMagnetSystem->Init(*g.mEcs);		// create magnet sprites and stuff.
			g.mSysManager->mTurboSystem->Init(*g.mEcs);			// set up turbo
			g.mSysManager->mShieldSystem->Init(*g.mEcs);		// set up shield sprite etc.
			g.mSysManager->mHealSystem->Init(*g.mEcs);			// set up healing particle system etc.
			g.mSysManager->mDiagSystem->Init(*g.mEcs);			// set up diagonal movement
			g.mSysManager->mRobotCtrlSystem->Init(*g.mEcs);		// set up robot control access time.


			break;

		case StateType::Maintenance:

			setupMaintenance(g);

			{
				Maintenance* maintenance = new Maintenance();
				maintenance->setupMaintenance(g);
				mCurrentStateInstance = maintenance;
			}

			break;

		case StateType::RobotCollection:

			if (mState == StateType::Dialogue) {	// if coming from dialogue
				setLuaWaitResponse();
				luaHideDialog(NULL);
			}

			showTalkButton(false);

			g.mRobots->showRobotInMenu(g.mEcs, Robots::Name::Charmy, -1);	// make the robots visible in the menu
			g.mAddons->showAddonsInMenu(g.mEcs);							// same with addons

			RobotCollection* rc = new RobotCollection();
			fbl_load_ttf_font("font/roboto.ttf", 18);	// use this font for the menu
			rc->cyclePages(g, 0);			// call this to update the first robots stats in the menu

			// always start showing the fav robot
			for (int i = 0; i < g.mRobots->ownedRobotsLeft(g); i++) {
				
				if (rc->mCurrentRobotPage == g.mProgress->mFavRobot)
					break;

				rc->cyclePages(g, 1);
			}

			rc->updateAddonInfo(g, true);	// update addon info to empty
			mCurrentStateInstance = rc;

			SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxTick, SoundManager::Channel::Ui, 0);

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

void GameState::toTitle(Game& g) {

	destroyAllGfx();				 // remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->unLoadLocation(g.mMap);
	g.mChars->removePlayer(g.mEcs);	 // delete the player completely
	g.mChars->removeNpc(g.mEcs);	 // also delete all npcs in the current scene
	g.mChars->clearRestoreList();
	g.mRobots->removeRobots(g.mEcs); // delete all the robots
	g.mAddons->removeAddons(g.mEcs); // delete all addons
	fbl_set_clear_color(11, 168, 230, 255);	// blue sky for the title
	g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 14, 80, false);	// timeOfDay, rainLevel, snowLevel, numClouds, lightningOn
	fbl_lua_shutdown();				 // so the dialogues get reset

	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSummer, SoundManager::Channel::Ambient, 1);
	SoundManager::getInstance().loadAndPlayMusic("music/title.ogg", 40, 0);

}

void GameState::titleToExplore(Game& g) {

	destroyAllGfx();				// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->loadLocation(g.mMap);
	initLuaDialog();				// set up prims and text and ui for the dialog box.
	g.mChars->setupPlayer(g.mEcs);	// create the player entity and add the right components
	g.mChars->setupNpc(g);			// add all npcs based on the map file

	g.mProgress->resetProgress();	// reset funds and completed races etc.

	g.mRobots->setupRobots(g.mEcs); // create the robot entities and add the basic components
	g.mAddons->setupAddons(g.mEcs); // create the addon entities

	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component
	//g.mSysManager->mCameraSystem->Init(*g.mEcs);	// creates debug rect for camera deadzone
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	g.mWeather->setWeather(Weather::TimeOfDay::Late, 1, 0, 50, true);

	initCollectionMenu();	// set up prims and text and ui for the collection-menu, sprite draw-order is important
	g.mAddons->initAddons(g.mEcs); // create the addon ui elements (buttons)

	fbl_lua_init("Ca2Dialogue.lua", registerFuncsToLua);	// set this up each new game, so the dialogues restart

	std::cout << "player id(e): " << g.mChars->mBrodo << std::endl;

	g.mChars->resetFadeCounter();

	SoundManager::getInstance().stopMusic();
	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxRainStorm, SoundManager::Channel::Ambient, 1);

}

void GameState::raceToExplore(Game& g) {

	destroyAllGfx();					// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->loadLocation(g.mMap);	// this will destroy all sprites, then load map
	initLuaDialog();					// set up prims and text and ui for the dialog box.
	g.mChars->restoreNpcs(g);
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	// remove the path and pathLogic components from the racing robots
	for (int i = 0; i < g.mRobots->mNumRacers; i++) {
		g.mEcs->RemoveComponent<Path>(g.mRobots->mRacingRobots[i]);
		g.mEcs->RemoveComponent<PathLogic>(g.mRobots->mRacingRobots[i]);
		if (i > 0) {	// only remove for non players
			g.mEcs->RemoveComponent<BasicAI>(g.mRobots->mRacingRobots[i]);
			// also remove any addons they might have (not Sprite, Stats, AutoAim, Light)
			if(g.mEcs->HasComponent<Laser>(g.mRobots->mRacingRobots[i]))
				g.mEcs->RemoveComponent<Laser>(g.mRobots->mRacingRobots[i]);
			if (g.mEcs->HasComponent<Magnet>(g.mRobots->mRacingRobots[i]))
				g.mEcs->RemoveComponent<Magnet>(g.mRobots->mRacingRobots[i]);
			if (g.mEcs->HasComponent<Turbo>(g.mRobots->mRacingRobots[i]))
				g.mEcs->RemoveComponent<Turbo>(g.mRobots->mRacingRobots[i]);
			if (g.mEcs->HasComponent<Shield>(g.mRobots->mRacingRobots[i]))
				g.mEcs->RemoveComponent<Shield>(g.mRobots->mRacingRobots[i]);
			if (g.mEcs->HasComponent<Heal>(g.mRobots->mRacingRobots[i]))
				g.mEcs->RemoveComponent<Heal>(g.mRobots->mRacingRobots[i]);
			// turn off auto aim
			auto& aim = g.mEcs->GetComponent<AutoAim>(g.mRobots->mRacingRobots[i]);
			aim.active = false;
		}
	}

	// add path component back to the player
	g.mEcs->AddComponent(g.mChars->mBrodo, Path{ 0, 0, 0, false, 2.0, FBL_PATHF_USE_DIAG, 1 });

	initCollectionMenu();	// set up prims and text and ui for the collection-menu, sprite draw-order is important
	g.mAddons->initAddons(g.mEcs); // create the addon ui elements (buttons)

	setAtmosphere(g);	// set music, ambience and weather based on progress

	// delete the event-slime you talked to
	g.mChars->checkNPC(g, g.mChars->EventSlime);
	// play silly sound
	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSnap, SoundManager::Channel::Ui, 0);

}

void GameState::maintenanceToExplore(Game& g) {

	destroyAllGfx();					// remove resources
	g.mLocation->loadLocation(g.mMap);
	initLuaDialog();
	g.mChars->restoreNpcs(g);
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	// add path component back to the player
	g.mEcs->AddComponent(g.mChars->mBrodo, Path{ 0, 0, 0, false, 2.0, FBL_PATHF_USE_DIAG, 1 });

	initCollectionMenu();
	g.mAddons->initAddons(g.mEcs);

	setAtmosphere(g);

	// delete the info-slime you talked to
	g.mChars->checkNPC(g, g.mChars->InfoSlime);

}

void GameState::setupRace(Game& g) {

	destroyAllGfx();								// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->unLoadLocation(g.mMap);			// this destroys ALL sprites
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

	g.mRobots->mapSpriteIdToEntity(g.mEcs);
	g.mRobots->hideRobots(g.mEcs);
	g.mAddons->initRaceAddons(g.mEcs);	 // set up the ui elements
	g.mAddons->hideAddons(g.mEcs);		 // don't show addons immediately

	// temporarily remove path component from the player
	g.mEcs->RemoveComponent<Path>(g.mChars->mBrodo);
	g.mChars->hidePlayer(g.mEcs);

	// hide the player light
	auto& light = g.mEcs->GetComponent<Light>(g.mChars->mBrodo);
	fbl_set_sprite_active(light.id, false);

	// explosion particles
	Efx::getInstance().initExplosion();
	Efx::getInstance().initCoinEfx();

	g.mWeather->setWeather(Weather::TimeOfDay::Evening, 0, 0, 0, false);

	SoundManager::getInstance().loadAndPlayMusic("music/dm.ogg", 80, 0);

	// play snap on ambient channel to stop the ambient sound.
	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSnap, SoundManager::Channel::Ambient, 0);

}

void GameState::setupMaintenance(Game& g) {

	destroyAllGfx();								// remove resources (ALL sprites, prims, text, ui and emitters)
	g.mLocation->unLoadLocation(g.mMap);			// this destroys ALL sprites
	g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
	g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component
	g.mRobots->hideRobots(g.mEcs);

	// temporarily remove path component from the player
	g.mEcs->RemoveComponent<Path>(g.mChars->mBrodo);
	g.mChars->hidePlayer(g.mEcs);

	g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 0, 0, false);

	SoundManager::getInstance().loadAndPlayMusic("music/maintenance.ogg", 80, 2);


	// play powerup sound on Ambient channel to stop the ambient sounds
	SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxPowerup, SoundManager::Channel::Ambient, 0);

}

void GameState::setAtmosphere(Game& g) {

	SoundManager::getInstance().stopMusic();

	// set music, ambience and weather based on completed races and maintenance sessions

	int totalProgress = g.mProgress->mCompletedRaces + g.mProgress->mCompletedMaint;

	switch(totalProgress) {
	
		case 0:
		case 1:
		case 2:
			g.mWeather->setWeather(Weather::TimeOfDay::Late, 1, 0, 50, true);
			SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxRainStorm, SoundManager::Channel::Ambient, -1);
			break;
		case 3:
		case 4:
		case 5:
			g.mWeather->setWeather(Weather::TimeOfDay::Morning, 1, 0, 50, false);
			SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSummer, SoundManager::Channel::Ambient, 2);
			SoundManager::getInstance().loadAndPlayMusic("music/overworld1.ogg", 100, 1);
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 50, true);
			SoundManager::getInstance().loadAndPlayMusic("music/overworld2.ogg", 100, 1);
			break;
		case 10:
		case 11:
			g.mWeather->setWeather(Weather::TimeOfDay::Night, 0, 0, 50, false);
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			g.mWeather->setWeather(Weather::TimeOfDay::Morning, 1, 0, 50, false);
			SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSummer, SoundManager::Channel::Ambient, 1);
			SoundManager::getInstance().loadAndPlayMusic("music/overworld1.ogg", 100, 1);
			break;

		default:
			g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 50, true);
			SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxSummer, SoundManager::Channel::Ambient, 1);
			SoundManager::getInstance().loadAndPlayMusic("music/overworld2.ogg", 100, 1);
			break;
	
	
	}


}