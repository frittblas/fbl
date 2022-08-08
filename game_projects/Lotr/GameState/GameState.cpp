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

#include "../Game.hpp"
#include "GameState.hpp"

#include "Title.hpp"
#include "Settings.hpp"
#include "Explore.hpp"
#include "Dialogue.hpp"

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
		if (mState >= StateType::Explore)
			g.unLoadLevel();	// reset map if we're coming from the game
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
		mCurrentStateInstance = new Explore();
		if (mState == StateType::Title)
			g.loadLevel();	// init first level if we're coming from the title screen
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

void GameState::tick() {

	mCurrentStateInstance->tick();

}
