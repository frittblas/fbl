/*
*
*	Ecs
*
*	PathLogicSystem.hpp
*
*	The path logic system header. Handles when robots get new paths and stuff.
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../GameState/Race/Maze.hpp"

#include "../../../Game.hpp"
#include "../../../GameState/GameState.hpp"
//#include "../../../Robots.hpp"
//#include "../../../Addons.hpp"
//#include "../../../Efx.hpp"	// remove this (observer pattern)

#include "PathLogicSystem.hpp"

extern Maze::aFlag gFlag[Maze::cMaxFlags];	// externed from Maze.cpp
extern Maze::aCoin gCoin[Maze::cMaxCoins];
extern bool gStartingOut;

void PathLogicSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& plog = ecs.GetComponent<PathLogic>(entity);

		// set coins and flags to zero
		plog.flags = 0;
		plog.coins = 0;

	}

	std::cout << "PathLogic component system initialized!" << std::endl;
	
}

void PathLogicSystem::Update(Game& g) {

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& plog = g.mEcs->GetComponent<PathLogic>(entity);
		

		handleFlags(entity, pos, spr, path, plog);
		handleCoins(entity, spr, plog);
		handleBases(entity, pos, spr, path, plog);

	}

}

void PathLogicSystem::handleFlags(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	// if the current robot has a flag, let the flag follow the robot, otherwise check for flag collisions
	int flagIndex = hasFlag(e);
	if (flagIndex >= 0) {
		fbl_set_sprite_xy(gFlag[flagIndex].id, pos.x, pos.y);
	}
	else {

		for (int i = 0; i < Maze::cMaxFlags; i++) {

			// handle flag colissions
			if (fbl_get_sprite_collision(spr.id[0], gFlag[i].id)) {

				std::cout << "collided with flag = " << i <<std::endl;

				// if in center or dropped (not in base or held by a robot)
				if (gFlag[i].state == Maze::FlagState::Center || gFlag[i].state == Maze::FlagState::Dropped) {

					// pick up flag
					gFlag[i].state = e;	// values equal to or over 0 is state == held by that robot entity

					// set course to the base
					path.goalX = plog.baseX;
					path.goalY = plog.baseY;
					path.newPath = true;

					std::cout << "Picked up flag!" << std::endl;

					break; // break from the loop (already got a flag)

				}

			}

		}

	}

}

int PathLogicSystem::hasFlag(Entity e) {

	for (int i = 0; i < Maze::cMaxFlags; i++)
		if (gFlag[i].state == e) return i;
		
	return -1;

}

void PathLogicSystem::handleCoins(Entity e, Sprite& spr, PathLogic& plog) {

	// handle coin colissions
	for (int i = 0; i < Maze::cMaxCoins; i++) {

		if (gCoin[i].id != -1) {
			if (fbl_get_sprite_collision(spr.id[0], gCoin[i].id)) {

				fbl_set_sprite_active(gCoin[i].id, false);
				gCoin[i].id = -1;
				plog.coins++;
				fbl_log("player %d has %d coins.", e, plog.coins);

			}
		}

	}

}

void PathLogicSystem::handleBases(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	// if a robot collides with it's base, drop any flags carried, charge the battery and set out for nearest flag

	// check to see if a robot has collided with a 20x20 square around the base coords.
	if (pos.x < (plog.baseX + 10) && pos.x > (plog.baseX - 10) &&
		pos.y < (plog.baseY + 10) && pos.y > (plog.baseY - 10)) {

		//std::cout << "Collided with da base!" << std::endl;

		int flagIndex = hasFlag(e);
		if (flagIndex >= 0) {
			plog.flags++;
			fbl_set_sprite_active(gFlag[flagIndex].id, false);
			gFlag[flagIndex].state = Maze::FlagState::Base;
			std::cout << "Dropped flag in base!" << std::endl;
		}

		if (!gStartingOut) {
			path.goalX = 15 * 32;
			path.goalY = 8 * 32;
			path.newPath = true;
			std::cout << "Heading out from base!" << std::endl;
		}

	}


}

void PathLogicSystem::findClosestFlag(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	bool noFlagsAvailable = true;
	int flagX, flagY;
	int shortestDistance = 1000;	// start with a very high number

	// check the distance to all the flags and pick the shortest path.

	for (int i = 0; i < Maze::cMaxFlags; i++) {
		
		// are any flags dropped?
		if (gFlag[i].state == Maze::FlagState::Dropped) {

			noFlagsAvailable = true;

			flagX = fbl_get_sprite_x(gFlag[i].id);
			flagY = fbl_get_sprite_y(gFlag[i].id);

		}

	}

	if(noFlagsAvailable) {

		// if no flags available, set course to the base
		path.goalX = plog.baseX;
		path.goalY = plog.baseY;
		path.newPath = true;

	}

}
