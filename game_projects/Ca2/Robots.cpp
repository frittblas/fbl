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
#include "Progress.hpp"
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
				mEcs->AddComponent(tmpRobot, Stats{ "Charmy", 1, 0, 7, 25, 25, 8, false, 25, 25, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
											  // id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });

				break;
			case Alarmy :
												 // id id id id num tx 96   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 32, 96, 32, 32, true, 2, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Alarmy", 1, 0, 7, 25, 25, 6, false, 25, 25, 17, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Boingy :
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 96, 96, 32, 32, true, 2, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Boingy", 1, 0, 7, 20, 20, 6, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Chompy:
												 // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 160, 96, 32, 32, true, 3, 12, 0, 0, 7 });
												  // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Chompy", 1, 0, 7, 25, 25, 7, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Dancy:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 256, 96, 32, 32, true, 4, 12, 0, 0, 7 });
												   // name  lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Dancy", 1, 0, 7, 30, 20, 11, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Floppy:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 0, 128, 32, 32, true, 2, 12, 0, 0, 7 });
												   // name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Floppy", 1, 0, 7, 30, 20, 10, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Levitaty:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 64, 128, 32, 32, true, 8, 12, 0, 0, 7 });
												   // name     lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Levitaty", 1, 0, 7, 30, 20, 9, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Necky:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 320, 128, 32, 32, true, 2, 12, 0, 0, 7 });
												   // name  lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Necky", 1, 0, 7, 30, 20, 9, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Partybot:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 0, 160, 32, 32, true, 7, 12, 0, 0, 7 });
												    // name    lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Partybot", 1, 0, 7, 30, 20, 12, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
												// id  tx  ty   w    h  scale
				mEcs->AddComponent(tmpRobot, Light{ 0, 384, 0, 128, 128, 2.0 });
				break;
			case Sprinty:
												  // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
				mEcs->AddComponent(tmpRobot, Sprite{ 0, 0, 0, 0, 1, 224, 160, 32, 32, true, 4, 4, 0, 0, 7 });
													// name   lv xp next mxHp hp speed diag mxNrg nrg weight slot[6] (-1 = notSet)
				mEcs->AddComponent(tmpRobot, Stats{ "Sprinty", 1, 0, 7, 30, 20, 14, false, 20, 20, 7, -1, -1, -1, -1, -1, -1 });
												//  rid len dir ivalM ivalC hasTarg active
				mEcs->AddComponent(tmpRobot, AutoAim{ 0, 800, 0, 10, 0, false, false });
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
	claimRobot(Dancy);

}

bool Robots::addAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType) {

	switch (addonType) {

		case Addons::AutoAim1:
		case Addons::AutoAim2:
		case Addons::AutoAim3:
			// in this case we only set the auto aim to active (component is always on)
			{
				auto& aim = mEcs->GetComponent<AutoAim>(robot);
				aim.active = true;

				if (addonType == Addons::AutoAim1) aim.intervalMax = 15;
				else if (addonType == Addons::AutoAim2) aim.intervalMax = 10;
				else if (addonType == Addons::AutoAim3) aim.intervalMax = 5;
			}
			break;
		case Addons::Laser1:
			if (mEcs->HasComponent<Laser>(robot)) return false;
										// rid pid cid len        dir	   dmg lv eCost isFiring
			mEcs->AddComponent(robot, Laser{ 0, 0, 0, 800, Addons::Dir::Up, 5, 1, 3, false });
			break;
		case Addons::Laser2:
			if (mEcs->HasComponent<Laser>(robot)) return false;
										// rid pid cid len        dir	   dmg lv eCost isFiring
			mEcs->AddComponent(robot, Laser{ 0, 0, 0, 800, Addons::Dir::Up, 10, 2, 2, false });
			break;
		case Addons::Laser3:
			if (mEcs->HasComponent<Laser>(robot)) return false;
										// rid pid cid len        dir	   dmg lv eCost isFiring
			mEcs->AddComponent(robot, Laser{ 0, 0, 0, 800, Addons::Dir::Up, 15, 3, 1, false });
			break;
		case Addons::Magnet1:
			if (mEcs->HasComponent<Magnet>(robot)) return false;
										  // sid str active
			mEcs->AddComponent(robot, Magnet{ 0, 32, true });
			break;
		case Addons::Magnet2:
			if (mEcs->HasComponent<Magnet>(robot)) return false;
										 // sid str active
			mEcs->AddComponent(robot, Magnet{ 0, 64, true });
			break;
		case Addons::Magnet3:
			if (mEcs->HasComponent<Magnet>(robot)) return false;
										 // sid str active
			mEcs->AddComponent(robot, Magnet{ 0, 128, true });
			break;
		case Addons::Turbo1:
			if (mEcs->HasComponent<Turbo>(robot)) return false;
										 // amnt eCost activated
			mEcs->AddComponent(robot, Turbo{ 1.4, 3, false });
			break;
		case Addons::Turbo2:
			if (mEcs->HasComponent<Turbo>(robot)) return false;
										// amnt eCost activated
			mEcs->AddComponent(robot, Turbo{ 1.7, 2, false });
			break;
		case Addons::Turbo3:
			if (mEcs->HasComponent<Turbo>(robot)) return false;
										 // amnt eCost activated
			mEcs->AddComponent(robot, Turbo{ 2.0, 1, false });
			break;
		case Addons::Shield1:
			if (mEcs->HasComponent<Shield>(robot)) return false;
										// sid, eCost lv isShielding
			mEcs->AddComponent(robot, Shield{ 0, 3, 1, false });
			break;
		case Addons::Shield2:
			if (mEcs->HasComponent<Shield>(robot)) return false;
									    // sid, eCost lv isShielding
			mEcs->AddComponent(robot, Shield{ 0, 2, 2, false });
			break;
		case Addons::Shield3:
			if (mEcs->HasComponent<Shield>(robot)) return false;
										// sid, eCost lv isShielding
			mEcs->AddComponent(robot, Shield{ 0, 1, 3, false });
			break;
		case Addons::Heal1:
			if (mEcs->HasComponent<Heal>(robot)) return false;
									  // pid eCost amnt maxAmnt activated
			mEcs->AddComponent(robot, Heal{ 0, 5, 50, 50, false });
			break;
		case Addons::Heal2:
			if (mEcs->HasComponent<Heal>(robot)) return false;
										// pid eCost amnt maxAmnt activated
			mEcs->AddComponent(robot, Heal{ 0, 4, 70, 70, false });
			break;
		case Addons::Heal3:
			if (mEcs->HasComponent<Heal>(robot)) return false;
									   // pid eCost amnt maxAmnt activated
			mEcs->AddComponent(robot, Heal{ 0, 3, 90, 90, false });
			break;
		case Addons::Diag:
									    // active justSw
			mEcs->AddComponent(robot, Diag{ true, false });
			break;
		case Addons::RobotCtrl:
										// access active justSw
			mEcs->AddComponent(robot, RobotCtrl{ 0, true, false });
			break;

	}

	return true;

}

void Robots::removeAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType) {

	switch (addonType) {

		case Addons::AutoAim1:
		case Addons::AutoAim2:
		case Addons::AutoAim3:
			// in this case we only set the auto aim to inactive (component is always on)
			{
				auto& aim = mEcs->GetComponent<AutoAim>(robot);
				aim.active = false;
			}
			break;
		case Addons::Laser1:
		case Addons::Laser2:
		case Addons::Laser3:
			mEcs->RemoveComponent<Laser>(robot);
			break;
		case Addons::Magnet1:
		case Addons::Magnet2:
		case Addons::Magnet3:
			mEcs->RemoveComponent<Magnet>(robot);
			break;
		case Addons::Turbo1:
		case Addons::Turbo2:
		case Addons::Turbo3:
			mEcs->RemoveComponent<Turbo>(robot);
			break;
		case Addons::Shield1:
		case Addons::Shield2:
		case Addons::Shield3:
			mEcs->RemoveComponent<Shield>(robot);
			break;
		case Addons::Heal1:
		case Addons::Heal2:
		case Addons::Heal3:
			mEcs->RemoveComponent<Heal>(robot);
			break;
		case Addons::Diag:
			mEcs->RemoveComponent<Diag>(robot);
			break;
		case Addons::RobotCtrl:
			mEcs->RemoveComponent<RobotCtrl>(robot);
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

void Robots::showRobotInMenu(Coordinator* mEcs, int nameIndex, int entity) {

	int x = Game::DeviceResW / 2 + 185;
	int y = Game::DeviceResH / 2 - 140;
	Entity e;

	if (nameIndex > -1) e = mOwnedRobots[nameIndex];
	if (entity > -1) e = entity;

	auto& pos = mEcs->GetComponent<Position>(e);
	auto& spr = mEcs->GetComponent<Sprite>(e);

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

void Robots::assignAIrobots(Game& g) {

	// select randomly which robots from the mAllRobots array should be racing.
	// Also set an appropriate level for the robots based on mProgress->completedRaces.
	// Also give the robots random addons appropriate to their level.


	// populate the racing robots array with random robots from all-robots array
	std::vector<int> availableIndices;
	// 1
	for (int i = 0; i < NumRobots; ++i)
		if (g.mRobots->mAllRobots[i] != g.mRobots->Unassigned)
			availableIndices.push_back(i);
	// 2
	std::random_shuffle(availableIndices.begin(), availableIndices.end());
	int numCopies = std::min(3, static_cast<int>(availableIndices.size()));
	// 3
	for (int i = 0; i < numCopies; i++)
		g.mRobots->mRacingRobots[i + 1] = g.mRobots->mAllRobots[availableIndices[i]];


	// now set levels and addons for the AI robots based on progress
	switch (g.mProgress->mCompletedRaces) {

		case 0:
		case 1:
			break;
		case 2:
		case 3:
		case 4:


			equipAIaddon(g, 2, Addons::Laser1);

			break;

		case 5:

			levelUpAllFreeRobots(g);



			break;
		case 6:
		case 7:
		case 8:
		case 9:


			break;
		case 10:
		case 11:
		case 12:

			break;

		default:
			break;


	}

	std::cout << "COMPLETED RACES:::::::::::::::::::: " << g.mProgress->mCompletedRaces << std::endl;

}

void Robots::equipAIaddon(Game& g, int probability, int type) {

	for (int i = 1; i < 4; i++) {
		if (rand() % probability == 0) {

			addAddonComponent(g.mEcs, g.mRobots->mRacingRobots[i], type);

			// turn on auto aim
			if (type == Addons::Laser1 || type == Addons::Laser2 || type == Addons::Laser3) {
				auto& aim = g.mEcs->GetComponent<AutoAim>(g.mRobots->mRacingRobots[i]);
				aim.active = true;
			}
		}
	}

}

int Robots::ownedRobotsLeft(Game& g) {

	int count = 0;

	for (int i = 0; i < NumRobots; i++)
		if (mOwnedRobots[i] != Unassigned)
			count++;

	return count;

}

int* Robots::levelUpRobot(Game& g, int nameIndex, bool owned) {

	Stats* sta = nullptr;

	if(owned)
		sta = &g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[nameIndex]);
	else
		sta = &g.mEcs->GetComponent<Stats>(g.mRobots->mAllRobots[nameIndex]);

	// if the robot already is at the max level just return
	if (sta->level > 4)
		return nullptr;

	int* bonus = new int[5]; // Create an array of 5 integers

	sta->level++;
	bonus[0] = 1;

	bonus[1] = rand() % 3 + 3;
	sta->maxHp += bonus[1];
	sta->hp = sta->maxHp;	// refill life at level up = nice!

	bonus[2] = rand() % 3 + 2;
	sta->speed += bonus[2];

	if (sta->level == 2) {
		sta->diag = true;	// all robots will get diag at level 2
		bonus[3] = 1;
	}
	else bonus[3] = 0;

	bonus[4] = rand() % 3 + 3;
	sta->maxEnergy += bonus[4];
	sta->energy = sta->maxEnergy;	// refill energy (this gets done anyway in Race.cpp)

	std::cout << "Leveled up!" << std::endl;

	// now return the changed values to be shown in the post race menu
	return bonus;

}

void Robots::levelUpAllFreeRobots(Game& g) {

	// level up all the robots one level, the ones that are not owned, for difficulty

	for (int i = 0; i < NumRobots; i++) {

		if (mAllRobots[i] != Unassigned)
			levelUpRobot(g, i, false);

	}

}

bool Robots::assignRobotXP(Game& g, int nameIndex) {

	auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[nameIndex]);

	// already max level
	if (sta.level > 4) {
		std::cout << "Already max level! " << std::endl;
		return false;
	}

	// check level up, skip if no level up
	if (sta.xp < sta.nextLv) {
		std::cout << "xp lower than nextLv" << std::endl;
		return false;
	}

	sta.xp = sta.xp - sta.nextLv;

	switch (sta.level) {

		// on level 1 nextLv = 7
		case 1:
			sta.nextLv = 10;
			break;
		case 2:
			sta.nextLv = 15;
			break;
		case 3:
			sta.nextLv = 20;
			break;
		case 4:
			sta.nextLv = 25;
			break;

	}

	std::cout << "Enough xp to level up!" << std::endl;

	return true;

}
