/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Dialogue.cpp
*
*   Dialogue class implementation, takes care of the Dialogue game-state.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "../../../src/fbl.hpp"
#include "../Game.hpp"
#include "../Weather.hpp"
#include "../Chars.hpp"
#include "Dialogue.hpp"
#include "GameState.hpp"

// this is from RobotCollection.cpp
extern uint16_t gRobotCollectionMenuButton;

// Dialogue-class implementation

Dialogue::Dialogue() {

	std::cout << "Started Dialogue state." << std::endl;

}

Dialogue::~Dialogue() {

	std::cout << "Destroyed Dialogue state." << std::endl;

}

void Dialogue::tick(Game& g) {

	g.mWeather->tick();

	// the almighty menu button
	if (fbl_get_ui_elem_val(gRobotCollectionMenuButton) > 0) {
		g.mState->change(g, GameState::StateType::RobotCollection);
		g.mChars->stopPlayerPathing(g);
	}

	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49
	if (num == 0)
		std::cout << "Tick dialogue!" << std::endl;

}
