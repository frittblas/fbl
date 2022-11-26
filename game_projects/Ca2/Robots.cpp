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

	Entity tmpRobot;
	int i = 0;

	// create all the robot entities
	tmpRobot = mEcs->CreateEntity();


	// add components to the entity
										  // x  y
	mEcs->AddComponent(tmpRobot, Position{ 400, 200 });
									 // id id id id num tx ty   w   h   anim fr spd dir dirLast
	mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 4, 0, 96, 32, 32, false, 0, 0, 0, 0 });
								   // id gX gY newPath
	mEcs->AddComponent(tmpRobot, Path{ 0, 0, 0, false });
										// name            lv mxHp hp mxNrg nrg weight
	mEcs->AddComponent(tmpRobot, Stats{ "Charming Alarming", 1, 10, 10, 20, 20, 7});

	mAllRobots.push_back(tmpRobot);

}

void Robots::removeRobots(Coordinator* mEcs) {

	for (Entity e : mAllRobots) {
		mEcs->DestroyEntity(e);
		std::cout << "Robot removed." << std::endl;
	}

	for (Entity e : mOwnedRobots) {
		mEcs->DestroyEntity(e);
		std::cout << "Robot removed." << std::endl;
	}

	mAllRobots.clear();
	mOwnedRobots.clear();

}

void Robots::hideRobots(Coordinator* mEcs) {

	for (Entity e : mAllRobots) {
		auto& spr = mEcs->GetComponent<Sprite>(e);
		fbl_set_sprite_active(spr.id[0], false);	// don't show the sprite (robots don't have different direction sprites)
	}

	for (Entity e : mOwnedRobots) {
		auto& spr = mEcs->GetComponent<Sprite>(e);
		fbl_set_sprite_active(spr.id[0], false);
	}

}

void Robots::claimRobot(RobotName name) {

	Entity tmpRobot;

	// remove entity from the "All" list and put it in the "Owned" list

	tmpRobot = mAllRobots.at(static_cast<int>(name));
	if(tmpRobot != Unassigned)
		mOwnedRobots.push_back(tmpRobot);

	mAllRobots.at(static_cast<int>(name)) = Unassigned;

	for (Entity e : mOwnedRobots) std::cout << e << std::endl;

}
