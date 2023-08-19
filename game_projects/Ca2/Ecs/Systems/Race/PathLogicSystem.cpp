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

#include "PathLogicSystem.hpp"

extern Maze::aFlag gFlag[Maze::cMaxFlags];	// from Maze.cpp
extern Maze::aCoin gCoin[Maze::cMaxCoins];
extern bool gStartingOut;
extern bool gUpdatePaths;	// from Laser.cpp

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

	tickCF(g);

}

void PathLogicSystem::tickCF(Game& g) {

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& plog = g.mEcs->GetComponent<PathLogic>(entity);


		handleFlags(entity, pos, spr, path, plog);
		handleCoins(entity, spr, plog);
		handleBases(g, entity, pos, spr, path, plog);

		switchCtrl(g, entity, pos, path, plog);

	}

	if (gUpdatePaths) {

		for (auto const& entity : mEntities)
		{

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<PathLogic>(entity);

			updatePaths(g, entity, pos, path, plog);
		}

		gUpdatePaths = false;
	}

}

void PathLogicSystem::tickDM(Game& g) {

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

					std::cout << "Picked up flag! entity: " << e << std::endl;

					gUpdatePaths = true;	// the other robots may need to find new path now.

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
				std::cout << "Player " << e << " has " << (int)plog.coins << std::endl;

			}
		}

	}

}

void PathLogicSystem::handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

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
			std::cout << "Dropped flag in base! entity: " << e << std::endl;
		}

		if (!gStartingOut) {
			//findClosestFlag(pos, path, plog);
			//std::cout << "Heading out from base! entity: " << e << std::endl;

			if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
				findClosestFlag(pos, path, plog);
			}
			else {
				auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

				if (!ctrl.active)
					findClosestFlag(pos, path, plog);
			}

		}

	}


}

void PathLogicSystem::findClosestFlag(Position& pos, Path& path, PathLogic& plog) {

	bool flagsAvailable = false;
	int flagX, flagY;
	int shortestDistance = 1000;	// start with a very high number
	int nearestFlagId;

	// first check the distance to all the dropped (if any) flags and pick the shortest path.

	for (int i = 0; i < Maze::cMaxFlags; i++) {
		
		// are any flags available?
		if (gFlag[i].state == Maze::FlagState::Center || gFlag[i].state == Maze::FlagState::Dropped) {

			flagsAvailable = true;

			flagX = fbl_get_sprite_x(gFlag[i].id);
			flagY = fbl_get_sprite_y(gFlag[i].id);

			fbl_pathf_set_path_status(path.id, fbl_pathf_find_path(path.id, pos.x, pos.y, flagX, flagY, path.diag));

			int pathLen = fbl_pathf_get_path_length(path.id);

			if (pathLen < shortestDistance) {
				shortestDistance = pathLen;	// update this
				nearestFlagId = i;
			}

			//std::cout << "Distance to dropped flag: " << pathLen << std::endl;

		}

	}

	if (flagsAvailable) {

		path.goalX = fbl_get_sprite_x(gFlag[nearestFlagId].id);
		path.goalY = fbl_get_sprite_y(gFlag[nearestFlagId].id);
		path.newPath = true;

	}

	if(!flagsAvailable) {

		// if no flags available, set course to the base
		path.goalX = plog.baseX;
		path.goalY = plog.baseY;
		path.newPath = true;

	}

}

void PathLogicSystem::switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {

	// This is used for setting up the correct behaviour when the player
	// clicks the passive RobotCtrl addon in a race.

	// update behaviour only for robots with a MouseCtrl component (i.e. the player)
	if (g.mEcs->HasComponent<RobotCtrl>(e)) {

		auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

		if (ctrl.justSwitched) {
			std::cout << "JUST SWITCHED! for robot-entity: " << e << std::endl;
			if (!ctrl.active) {		// if the robot ctrl is deactivated, control goes back to auto

				if (hasFlag(e) >= 0) {

					// if carrying flag, set course to the base
					if (!gStartingOut) {
						path.goalX = plog.baseX;
						path.goalY = plog.baseY;
						path.newPath = true;
					}

				}
				else {
					if (!gStartingOut)
						findClosestFlag(pos, path, plog);
				}

			}
			else {	// here it's active again

				// make robot stop
				fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);

			}

			ctrl.justSwitched = false;

		}

	}


}

void PathLogicSystem::updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {


	// this only applies to robots that are not carrying a flag
	int flagIndex = hasFlag(e);
	if (flagIndex < 0) {

		// update paths for robots without a MouseCtrl component
		if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
			findClosestFlag(pos, path, plog);
		}
		else {
			auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

			if (!ctrl.active && !gStartingOut)
				findClosestFlag(pos, path, plog);
		}

		std::cout << "New path for robot-entity: " << e << std::endl;

	}


}
