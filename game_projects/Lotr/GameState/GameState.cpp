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

#include "GameState.hpp"

#include "Title.hpp"
#include "Explore.hpp"

// GameState-class implementation

GameState::GameState() {

	mState = StateType::Title;

	mCurrentStateInstance = new Title();	// Start by default to title screen

}

GameState::~GameState() {

	// delete the current state instance

	delete mCurrentStateInstance;

}

void GameState::change(StateType newState) {

	// first deallocate any resources that are in play

	delete mCurrentStateInstance;	// delete the current state instance, calling the correct destructor

	// then set the new state and allocate resources for the new state

	mState = newState;

	switch (mState) {

	case StateType::Title:
		mCurrentStateInstance = new Title();
		break;

	case StateType::Demo:
		break;

	case StateType::Settings:
		break;

	case StateType::Tutorial:
		break;

	case StateType::Explore:
		mCurrentStateInstance = new Explore();
		break;

	case StateType::Dialogue:
		break;

	case StateType::Shop:
		break;

	case StateType::Fight:
		break;

	case StateType::CardCollection:
		break;

	}

}

GameState::StateType GameState::get() {

	return mState;
}

void GameState::tick() {

	mCurrentStateInstance->tick();

}