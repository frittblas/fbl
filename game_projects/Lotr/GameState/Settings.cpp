/*
*
*	Lotr
*
*	Settings.cpp
*
*   Settings class implementation, takes care of the settings screen.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "Settings.hpp"

// Settings-class implementation

Settings::Settings() {

	std::cout << "Alloc Settings!" << std::endl;

}

Settings::~Settings() {

	std::cout << "Destroy Settings!" << std::endl;

}


// implement the virtual tick() function
void Settings::tick() {

	int num = std::rand() / ((RAND_MAX + 1u) / 20); // random numbers from 0-19

	if (num == 0)
		std::cout << "Tick settings!" << std::endl;

}
