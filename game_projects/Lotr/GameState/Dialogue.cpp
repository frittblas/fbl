/*
*
*	Lotr
*
*	Dialogue.cpp
*
*   Dialogue class implementation, takes care of the Dialogue game-state.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "../Game.hpp"
#include "Dialogue.hpp"

// Dialogue-class implementation

Dialogue::Dialogue() {

	std::cout << "Started Dialogue state." << std::endl;

}

Dialogue::~Dialogue() {


	std::cout << "Destroyed Dialogue state." << std::endl;

}

void Dialogue::tick(Game& g) {



	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49
	if (num == 0)
		std::cout << "Tick dialogue!" << std::endl;

}
