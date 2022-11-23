/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Progress.cpp
*
*	Progress class implemetation, takes care of all game progress, how far the player has come, current location, quests etc.
*
*	Hans Strömquist 2022
*
*/

#include "Progress.hpp"

// Progress-class implementation

Progress::Progress() {

	mCurrentLocation = 0;

	std::cout << "Initialized Progress subsystem." << std::endl;

}

Progress::~Progress() {

	std::cout << "Destroyed Progress subsystem." << std::endl;

}
