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

// GameState-class implementation

GameState::GameState() {

	mState = State::Title;

}

GameState::~GameState() {}

void GameState::change(State newState) {

	// first check the current state and deallocate any resources that are in play
	switch (mState) {

		case State::Title:
			break;

		case State::Settings:
			break;

		case State::Tutorial:
			break; 
		
		case State::Exploring:
			break;

		case State::Dialogue:
			break;

		case State::Shopping:
			break;

		case State::Fighting:
			break;

		case State::CardCollection:
			break;

	}

	// then set the new state and allocate resources for the new state

	mState = newState;

	switch (mState) {

	case State::Title:
		break;

	case State::Settings:
		break;

	case State::Tutorial:
		break;

	case State::Exploring:
		break;

	case State::Dialogue:
		break;

	case State::Shopping:
		break;

	case State::Fighting:
		break;

	case State::CardCollection:
		break;

	}

}

GameState::State GameState::get() {

	return mState;
}
