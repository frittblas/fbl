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

#include "GameState.hpp"
#include "Title.hpp"
#include "Settings.hpp"
#include "Explore.hpp"
#include "Dialogue.hpp"

#include "../Chars.hpp"
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

				g.unLoadLevel();	// reset map if we're coming from the game
				unInitLuaDialog();	// also remove resources for dialogue (prims, text etc)
				g.mChars->removePlayer(g.mEcs);	// delete the player completely
				g.mChars->removeNpc(g.mEcs);	// also delete all npcs in the current scene

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

				g.loadLevel();		// init first level
				initLuaDialog();	// set up prims and text and ui for the dialog box.
				g.mChars->setupPlayer(g.mEcs);	// create the player entity and add the right components
				g.mChars->setupNpc(g);			// add all npcs based on the map file

				g.mSysManager->mSpriteSystem->Init(*g.mEcs);	// create sprites for all entities with a sprite component
				g.mSysManager->mPathSystem->Init(*g.mEcs);		// assign a unique path id to the entities with a path component
				//g.mSysManager->mCameraSystem->Init(*g.mEcs);	// creates debug rect for camera deadzone

				g.mWeather->initClouds(200);		// add weather on top :)
				g.mWeather->initRain(2, 100);			// add rain (add alpha when it gets dark)

				
				fbl_set_lighting_tint(true, 120, 100, 100);
				int light = fbl_create_sprite(384, 0, 128, 128, 0);
				fbl_set_sprite_is_light(light, true);
				fbl_set_sprite_scale(light, 3.0);
				

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
