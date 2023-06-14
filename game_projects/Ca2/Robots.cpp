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

#include "../../src/fbl.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Robots.hpp"
#include "Addons.hpp"

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
		mRacingRobots[i] = Unassigned;
	}

	// create all the robot entities
	for (int i = 0; i < NumRobots; i++) {

		tmpRobot = mEcs->CreateEntity();

		// add components to the entity
											  // x  y
		mEcs->AddComponent(tmpRobot, Position{ 400, 200 });


		switch (i) {

			case Charmy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 0, 96, 32, 32, false, 0, 0, 0, 0, 7 });	// robots are on layer 7
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Charmy", 1, 0, 4, 40, 30, 8, true, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												  // baseX Y flg coin
				mEcs->AddComponent(tmpRobot, PathLogic{ 0, 0, 0, 0 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// rid pid len        dir	    dmg lv  isFiring
				//mEcs->AddComponent(tmpRobot, Laser{ 0, 0, 800, Addons::Dir::Up, 1, 1, false });
											  // id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				mEcs->AddComponent(tmpRobot, MouseCtrl{ 300 });
				break;
			case Alarmy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 32, 96, 32, 32, true, 2, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Alarmy", 1, 0, 4, 40, 40, 6, true, 20, 19, 17, -1, -1, -1, -1, -1, -1 });
											   // baseX Y flg coin
				mEcs->AddComponent(tmpRobot, PathLogic{ 0, 0, 0, 0 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, true });
												// rid pid len        dir	  dmg lv isFiring
				mEcs->AddComponent(tmpRobot, Laser{ 0, 0, 800, Addons::Dir::Up, 1, 1, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Boingy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 96, 96, 32, 32, true, 2, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Boingy", 2, 0, 4, 30, 30, 4, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												  // baseX Y flg coin
				mEcs->AddComponent(tmpRobot, PathLogic{ 0, 0, 0, 0 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, true });
												// rid pid len        dir	  dmg lv isFiring
				mEcs->AddComponent(tmpRobot, Laser{ 0, 0, 800, Addons::Dir::Up, 1, 1, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Chompy:
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 160, 96, 32, 32, true, 3, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Chompy", 2, 0, 4, 30, 30, 5, true, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
											   // baseX Y flg coin
				mEcs->AddComponent(tmpRobot, PathLogic{ 0, 0, 0, 0 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, true });
												// rid pid len        dir	  dmg lv isFiring
				mEcs->AddComponent(tmpRobot, Laser{ 0, 0, 800, Addons::Dir::Up, 1, 1, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;

		}

		// add the entity to the array containing all robots
		mAllRobots[i] = tmpRobot;

	}

	claimRobot(Charmy);
	claimRobot(Alarmy);
	claimRobot(Boingy);
	claimRobot(Chompy);

}

void Robots::addAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType) {

	switch (addonType) {

		case Addons::AutoAim :
			// in this case we only set the auto aim to active (component is always on)
			{
				auto& aim = mEcs->GetComponent<AutoAim>(robot);
				aim.active = true;
			}
			break;
		case Addons::Laser:
										 // rid pid len        dir	   dmg lv  isFiring
			mEcs->AddComponent(robot, Laser{ 0, 0, 800, Addons::Dir::Up, 1, 1, false });
			break;
		case Addons::Magnet:
			break;

	}

}

void Robots::removeAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType) {

	switch (addonType) {

		case Addons::AutoAim:
			// in this case we only set the auto aim to inactive (component is always on)
			{
				auto& aim = mEcs->GetComponent<AutoAim>(robot);
				aim.active = false;
			}
			break;
		case Addons::Laser:
			mEcs->RemoveComponent<Laser>(robot);
			break;
		case Addons::Magnet:
			break;

	}

}

void Robots::mapSpriteIdToEntity(Coordinator* mEcs) {

	// first clear the spriteId->Entity mapping
	mSpriteIdToEntityMap.clear();

	// then add entries to the spriteId->Entity mapping, must be called after the sprites have been created
	

	for (Entity e : mAllRobots)
	{

		if (e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			mSpriteIdToEntityMap[spr.id[0]] = e;
			std::cout << "SpriteId->Entity map (all): " << spr.id[0] << "->" << e << std::endl;
		}
	}

	for (Entity e : mOwnedRobots)
	{

		if(e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			mSpriteIdToEntityMap[spr.id[0]] = e;
			std::cout << "SpriteId->Entity map (owned): " << spr.id[0] << "->" << e << std::endl;
		}
	}

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
			auto& light = mEcs->GetComponent<Light>(e);
			fbl_set_sprite_active(spr.id[0], false);	// don't show the sprite (robots don't have different direction sprites)
			fbl_set_sprite_active(light.id, false);		// also hide the light
		}
	}

	for (Entity e : mOwnedRobots) {
		if (e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			auto& light = mEcs->GetComponent<Light>(e);
			fbl_set_sprite_active(spr.id[0], false);
			fbl_set_sprite_active(light.id, false);		// also hide the light
		}
	}

}

void Robots::showRobotInMenu(Coordinator* mEcs, int nameIndex) {

	int x = Game::DeviceResW / 2 + 185;
	int y = Game::DeviceResH / 2 - 140;

	auto& pos = mEcs->GetComponent<Position>(mOwnedRobots[nameIndex]);
	auto& spr = mEcs->GetComponent<Sprite>(mOwnedRobots[nameIndex]);

	pos.x = x;
	pos.y = y;

	fbl_set_sprite_xy(spr.id[0], x, y);
	fbl_set_sprite_active(spr.id[0], true);
	fbl_fix_sprite_to_screen(spr.id[0], true);

}

void Robots::showRobotInRace(Coordinator* mEcs, Entity robot, int position) {

	int x, y;

	switch (position) {

		case 0:
			x = Game::TileSize * 3;
			y = 0;
			break;
		case 1:
			x = Game::LogicalResW - Game::TileSize * 4;
			y = 0;
			break;
		case 2:
			x = Game::TileSize * 3;
			y = Game::TileSize * 16;
			break;
		case 3:
			x = Game::LogicalResW - Game::TileSize * 4;
			y = Game::TileSize * 16;
			break;

	}

	auto& pos = mEcs->GetComponent<Position>(robot);
	auto& spr = mEcs->GetComponent<Sprite>(robot);
	auto& light = mEcs->GetComponent<Light>(robot);

	pos.x = x;
	pos.y = y;

	fbl_set_sprite_xy(spr.id[0], x, y);
	fbl_set_sprite_active(spr.id[0], true);
	fbl_set_sprite_phys(spr.id[0], true, FBL_RECT_PHYS, FBL_PHYS_DYNAMIC, false);
	fbl_fix_sprite_to_screen(spr.id[0], false);

	fbl_set_sprite_active(light.id, true);		// also show the light

}

void Robots::claimRobot(int nameIndex) {

	// remove entity from the "All" list and put it in the "Owned" list

	if (mAllRobots[nameIndex] != Unassigned) {
	
		mOwnedRobots[nameIndex] = mAllRobots[nameIndex];
		mAllRobots[nameIndex] = Unassigned;

	}

	for (Entity e : mOwnedRobots) std::cout << "Owned robot entity-id: " << e << std::endl;

}
