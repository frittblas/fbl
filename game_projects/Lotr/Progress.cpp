/*
*
*	Lotr
*
*	Progress.cpp
*
*	Progress class implemetation, takes care of all game progress, how far the player has come, levels, quests.
*
*	Hans Strömquist 2022
*
*/

#include "Progress.hpp"

// Progress-class implementation

Progress::Progress() {

	room = level = 1;
	std::cout << "Initialized Progress subsystem." << std::endl;
}

Progress::~Progress() {

	std::cout << "Destroyed progress subsystem." << std::endl;

}
