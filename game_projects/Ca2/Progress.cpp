/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Progress.cpp
*
*	Progress class header, takes care of all game progress, how far the player has come,
*   current location, quests, coins etc. Also save and load game.
*
*	Hans Strömquist 2022
*
*/

//#include "Robots.hpp"
#include "Progress.hpp"

// Progress-class implementation

Progress::Progress() {

	mFunds = 0;
	mFavRobot = 0;	// always start with charmy as fav (Robots::Charmy == 0)
	mCurrentLocation = 0;
	mCompletedRaces = 0;

	std::cout << "Initialized Progress subsystem." << std::endl;

}

Progress::~Progress() {

	std::cout << "Destroyed Progress subsystem." << std::endl;

}

void Progress::resetProgress() {

	mFunds = 20;
	mFavRobot = 0;
	mCurrentLocation = 0;
	mCompletedRaces = 0;

}