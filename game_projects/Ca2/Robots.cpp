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

	// init to unassigned
	for (int i = 0; i < NumRobots; i++) {
		mAllRobots[i] = Unassigned;
		mOwnedRobots[i] = Unassigned;
	}

	// create all the robot entities
	for (int i = 0; i < NumRobots; i++) {

		tmpRobot = mEcs->CreateEntity();

		// add components to the entity
											  // x  y
		mEcs->AddComponent(tmpRobot, Position{ 400, 200 });
										// id gX gY newPath
		mEcs->AddComponent(tmpRobot, Path{ 0, 0, 0, false });

		switch (i) {

			case Charmy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 0, 96, 32, 32, false, 0, 0, 0, 0, 4 });	// robots are on layer 4
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight
				mEcs->AddComponent(tmpRobot, Stats{ "Charmy", 1, 0, 4, 10, 10, 10, true, 20, 20, 7 });
				break;
			case Alarmy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 32, 96, 32, 32, true, 2, 12, 0, 0, 4 });
												   // name   lv xp next mxHp hp speed diag mxNrg nrg weight
				mEcs->AddComponent(tmpRobot, Stats{ "Alarmy", 1, 0, 4, 10, 10, 10, true, 20, 19, 17 });
				break;
			case Boingy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 96, 96, 32, 32, true, 2, 12, 0, 0, 4 });
												   // name   lv xp next mxHp hp speed diag mxNrg nrg weight
				mEcs->AddComponent(tmpRobot, Stats{ "Boingy", 2, 0, 4, 11, 9, 10, true, 20, 20, 7 });
				break;

		}

		// add the entity to the array containing all robots
		mAllRobots[i] = tmpRobot;

	}

	claimRobot(Charmy);
	claimRobot(Alarmy);
	claimRobot(Boingy);

}

void Robots::removeRobots(Coordinator* mEcs) {

	for (Entity e : mAllRobots) {
		if (e != Unassigned) {
			mEcs->DestroyEntity(e);
			std::cout << "Robot removed from All-list." << std::endl;
		}
	}

	for (int i = 0; i < NumRobots; i++) {
		if (mOwnedRobots[i] != Unassigned) {
			mEcs->DestroyEntity(mOwnedRobots[i]);
			std::cout << "Robot removed from Owned-list." << std::endl;
		}
	}

}

void Robots::hideRobots(Coordinator* mEcs) {

	for (Entity e : mAllRobots) {
		if (e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			fbl_set_sprite_active(spr.id[0], false);	// don't show the sprite (robots don't have different direction sprites)
		}
	}

	for (Entity e : mOwnedRobots) {
		if (e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			fbl_set_sprite_active(spr.id[0], false);
		}
	}

}

void Robots::showRobot(Coordinator* mEcs, int nameIndex) {

	int x = Game::DeviceResW / 2 + 185;
	int y = Game::DeviceResH / 2 - 140;

	auto& pos = mEcs->GetComponent<Position>(mOwnedRobots[nameIndex]);
	auto& spr = mEcs->GetComponent<Sprite>(mOwnedRobots[nameIndex]);

	pos.x = x;
	pos.y = y;

	fbl_set_sprite_xy(spr.id[0], x, y);
	fbl_set_sprite_active(spr.id[0], true);

}

void Robots::claimRobot(Name name) {

	// remove entity from the "All" list and put it in the "Owned" list

	if (mAllRobots[name] != Unassigned) {
	
		mOwnedRobots[name] = mAllRobots[name];
		mAllRobots[name] = Unassigned;

	}

	for (Entity e : mOwnedRobots) std::cout << e << std::endl;

}
