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

	std::cout << "Alloc Dialogue!" << std::endl;

}

Dialogue::~Dialogue() {


	std::cout << "Destroy Dialogue!" << std::endl;

}

void Dialogue::tick(Game& g) {

	int num = std::rand() / ((RAND_MAX + 1u) / 20); // random numbers from 0-19

	if (num == 0)
		std::cout << "Tick dialogue!" << std::endl;

}
