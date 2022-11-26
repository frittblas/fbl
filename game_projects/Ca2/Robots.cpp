/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Robots.cpp
*
*	Robots class implementation, handles the robots in the game, storing entity id's etc.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Robots.hpp"

// Robots-class implementation

Robots::Robots() {

	std::cout << "Initialized Robots subsystem." << std::endl;

}

Robots::~Robots() {

	std::cout << "Destroyed Robots subsystem." << std::endl;

}

void Robots::setupRobots(Coordinator* mEcs) {


}

void Robots::removeRobots(Coordinator* mEcs) {


	
}
