/*
*
*	Lotr
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

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"
#include "../Ecs/Systems/CameraSystem.hpp"
#include "../Ecs/Systems/LightSystem.hpp"

#include "GameState.hpp"
#include "Title.hpp"
#include "Settings.hpp"
#include "Explore.hpp"
#include "Dialogue.hpp"

#include "../Chars.hpp"
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

				g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 0, 0, false);	// reset weather before unload level (destroys all sprites and emitters)
				g.mLocation->unLoadLocation(g.mMap);
				unInitLuaDialog();	// also remove resources for dialogue (prims, text etc)
				g.mChars->removePlayer(g.mEcs);	// delete the player completely
				g.mChars->removeNpc(g.mEcs);	// also delete all npcs in the current scene
				g.mWeather->setWeather(Weather::TimeOfDay::Day, 0, 6, 0, false);	// timeOfDay, rainLevel, snowLevel, numClouds, lightningOn

				fbl_lua_shutdown();	// so the dialogues gets reset

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

				g.mLocation->loadLocation(g.mMap);
				initLuaDialog();	// set up prims and text and ui for the dialog box.
				g.mChars->setupPlayer(g.mEcs);	// create the player entity and add the right components
				g.mChars->setupNpc(g);			// add all npcs based on the map file

				g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
				g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component
				//g.mSysManager->mCameraSystem->Init(*g.mEcs);	// creates debug rect for camera deadzone
				g.mSysManager->mLightSystem->Init(*g.mEcs);		// create lights for all entities with a light component

				g.mWeather->setWeather(Weather::TimeOfDay::Evening, 1, 0, 100, true);

				fbl_lua_init("LotrDialogue.lua", registerFuncsToLua);	// set this up each new game, so the dialogues restart

			}

			mCurrentStateInstance = new Explore();

			break;

		case StateType::Dialogue:

			mCurrentStateInstance = new Dialogue();

			break;

		case StateType::Shop:
			break;

		case StateType::Fight:
			break;

		case StateType::CardCollection:
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
