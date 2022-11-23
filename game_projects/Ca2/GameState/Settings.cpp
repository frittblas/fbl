/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Settings.cpp
*
*   Settings class implementation, takes care of the settings screen.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

#include "../Game.hpp"
#include "Settings.hpp"


// Settings-class implementation

Settings::Settings() {

	std::cout << "Started Settings state." << std::endl;

}

Settings::~Settings() {

	std::cout << "Destroyed Settings state." << std::endl;

}


// implement the virtual tick() function
void Settings::tick(Game& g) {

	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49

	if (num == 0)
		std::cout << "Tick settings!" << std::endl;

}
