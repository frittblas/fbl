/*
*
*
*	CaptureFlags.hpp
*
*	The capture flags game mode. Handles rules for this game mode.
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../../Ecs/Ecs.hpp"
#include "../../../Ecs/Components.hpp"

#include "../../../../Ca2/Efx.hpp"
#include "../../../../Ca2/Game.hpp"
#include "../../../../Ca2/Progress.hpp"
#include "../../../../Ca2/SoundManager.hpp"
#include "../../../../Ca2/GameState/Race/Race.hpp"
#include "../../../../Ca2/GameState/Race/Maze.hpp"

#include "CaptureFlags.hpp"

void CaptureFlags::handleFlags(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	// if the current robot has a flag, let the flag follow the robot, otherwise check for flag collisions
	int flagIndex = hasFlag(e);
	if (flagIndex >= 0) {
		fbl_set_sprite_xy(Maze::sFlag[flagIndex].id, pos.x, pos.y);
		return;
	}

	for (int i = 0; i < Maze::cMaxFlags; i++) {

		// handle flag colissions, skip if no colission
		if (!fbl_get_sprite_collision(spr.id[0], Maze::sFlag[i].id)) continue;

		std::cout << "collided with flag = " << i << std::endl;

		// if in center or dropped (not in base or held by a robot)
		if (Maze::sFlag[i].state == Maze::FlagState::Center || Maze::sFlag[i].state == Maze::FlagState::Dropped) {

			// pick up flag
			Maze::sFlag[i].state = e;	// values equal to or over 0 is state == held by that robot entity

			// set course to the base
			path.goalX = plog.baseX;
			path.goalY = plog.baseY;
			path.newPath = true;

			std::cout << "Picked up flag! entity: " << e << std::endl;

			Maze::sUpdatePaths = true;	// the other robots may need to find new path now.

			break; // break from the loop (already got a flag)

		}


	}

}

int CaptureFlags::hasFlag(Entity e) {

	for (int i = 0; i < Maze::cMaxFlags; i++)
		if (Maze::sFlag[i].state == e) return i;

	return -1;

}

void CaptureFlags::handleCoins(Entity e, Sprite& spr, PathLogic& plog) {

	// handle coin colissions
	for (int i = 0; i < Maze::cMaxCoins; i++) {

		if (Maze::sCoin[i].id == -1) continue;
		if (!fbl_get_sprite_collision(spr.id[0], Maze::sCoin[i].id)) continue;

		int x = fbl_get_sprite_x(Maze::sCoin[i].id);
		int y = fbl_get_sprite_y(Maze::sCoin[i].id);

		fbl_set_sprite_active(Maze::sCoin[i].id, false);
		Maze::sCoin[i].id = -1;
		plog.coins++;
		std::cout << "Player " << e << " has " << (int)plog.coins << std::endl;

		// coin effect
		Efx::getInstance().startCoinEfx(x, y);

		SoundManager::getInstance().playSfx(SoundManager::getInstance().mSfxCoin, SoundManager::Channel::Ui, 0);

	}

}

void CaptureFlags::handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	// if a robot collides with it's base, drop any flags carried, charge the battery and set out for nearest flag

	// check to see if a robot has collided with a 20x20 square around the base coords, otherwise skip
	if (!(pos.x < (plog.baseX + 10) && pos.x >(plog.baseX - 10) &&
		pos.y < (plog.baseY + 10) && pos.y >(plog.baseY - 10))) return;

	//std::cout << "Collided with da base!" << std::endl;

	// charge battery
	auto& stat = g.mEcs->GetComponent<Stats>(e);
	stat.energy = stat.maxEnergy;

	int flagIndex = hasFlag(e);
	if (flagIndex >= 0) {
		plog.flags++;
		fbl_set_sprite_active(Maze::sFlag[flagIndex].id, false);
		Maze::sFlag[flagIndex].state = Maze::FlagState::Base;
		std::cout << "Dropped flag in base! entity: " << e << std::endl;
	}

	if (Maze::sStartingOut) return;

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

void CaptureFlags::findClosestFlag(Position& pos, Path& path, PathLogic& plog) {

	bool flagsAvailable = false;
	int flagX, flagY;
	int shortestDistance = 1000;	// start with a very high number
	int nearestFlagId;

	// first check the distance to all the dropped (if any) flags and pick the shortest path.

	for (int i = 0; i < Maze::cMaxFlags; i++) {

		// are any flags available?
		if (Maze::sFlag[i].state == Maze::FlagState::Center || Maze::sFlag[i].state == Maze::FlagState::Dropped) {

			flagsAvailable = true;

			flagX = fbl_get_sprite_x(Maze::sFlag[i].id);
			flagY = fbl_get_sprite_y(Maze::sFlag[i].id);

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
		path.goalX = fbl_get_sprite_x(Maze::sFlag[nearestFlagId].id);
		path.goalY = fbl_get_sprite_y(Maze::sFlag[nearestFlagId].id);
	}
	else {
		// if no flags available, set course to the base
		path.goalX = plog.baseX;
		path.goalY = plog.baseY;
	}

	path.newPath = true;

}

void CaptureFlags::checkWinCondition(Game& g) {

	if (Race::sRaceState == Race::Undecided) {


		// now check if all flags are returned to base
		bool allTaken = true;
		for (int i = 0; i < Maze::cMaxFlags; i++) {
			if (Maze::sFlag[i].state != Maze::FlagState::Base) {
				allTaken = false;
				break;
			}
		}

		if (allTaken) {
		
			// find out how you placed
			int flag[4] = {};
			int playerFlags = 0;

			for (int i = 0; i < g.mRobots->mNumRacers; i++) {
				auto& plog = g.mEcs->GetComponent<PathLogic>(g.mRobots->mRacingRobots[i]);
				flag[i] = plog.flags;
				if (i == 0) playerFlags = plog.flags;
			}

			// Sort the array
			std::sort(flag, flag + g.mRobots->mNumRacers);

			if(playerFlags == flag[3]) Race::sRaceState = Race::First;
			else if (playerFlags == flag[2]) Race::sRaceState = Race::Second;
			else if (playerFlags == flag[1]) Race::sRaceState = Race::Third;
			else if (playerFlags == flag[0]) Race::sRaceState = Race::Fourth;
		
		}


		// also check if all other robots are dead = win
		bool allDead = true;
		for (int i = 1; i < g.mRobots->mNumRacers; i++) {
			auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[i]);
			if (sta.hp > 0.1) {
				allDead = false;
				break;
			}
		}
		if (allDead) {
			Race::sRaceState = Race::First;
		}

		// check if player is dead
		auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[0]);

		if (sta.hp < 0.1) {

			// if you still have robots that are alive left it's not game over.
			if (g.mRobots->ownedRobotsLeft(g) > 0) {

				Race::sRaceState = Race::Fourth;

			}
			else
				Race::sRaceState = Race::Dead;
		}

	}

}

void CaptureFlags::switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {

	// This is used for setting up the correct behaviour when the player
	// clicks the passive RobotCtrl addon in a race.

	// update behaviour only for robots with a MouseCtrl component (i.e. the player)
	if (!g.mEcs->HasComponent<RobotCtrl>(e)) return;

	auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

	if (!ctrl.justSwitched) return;		// don't bother is player didn't just switch (from ctrl to automatic)

	std::cout << "JUST SWITCHED! for robot-entity: " << e << std::endl;

	if (!ctrl.active) {		// if the robot ctrl is deactivated, control goes back to auto

		if (hasFlag(e) >= 0) {

			// if carrying flag, set course to the base
			if (!Maze::sStartingOut) {
				path.goalX = plog.baseX;
				path.goalY = plog.baseY;
				path.newPath = true;
			}

		}
		else {
			if (!Maze::sStartingOut)
				findClosestFlag(pos, path, plog);
		}

	}
	else {	// here it's active again

		// make robot stop
		fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);

	}

	ctrl.justSwitched = false;

}

void CaptureFlags::updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {


	// this only applies to robots that are not carrying a flag
	int flagIndex = hasFlag(e);
	if (flagIndex < 0) {

		// update paths for robots without a MouseCtrl component
		if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
			findClosestFlag(pos, path, plog);
		}
		else {
			auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

			if (!ctrl.active && !Maze::sStartingOut)
				findClosestFlag(pos, path, plog);
		}

		std::cout << "New path for robot-entity: " << e << std::endl;

	}


}
