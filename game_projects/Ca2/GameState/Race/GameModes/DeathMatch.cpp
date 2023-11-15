/*
*
*
*	DeathMatch.hpp
*
*	The death match game mode. Handles rules for this game mode.
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../../Ecs/Ecs.hpp"
#include "../../../Ecs/Components.hpp"

#include "../../../../Ca2/Game.hpp"
#include "../../../../Ca2/Progress.hpp"
#include "../../../../Ca2/GameState/Race/Race.hpp"
#include "../../../../Ca2/GameState/Race/Maze.hpp"

#include "DeathMatch.hpp"

DeathMatch::DeathMatch() {


	std::cout << "Initialized Deathmatch game mode." << std::endl;

}

DeathMatch::~DeathMatch() {

	std::cout << "Destroyed Deathmatch game mode." << std::endl;

}

void DeathMatch::handleTargets(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {


	// don't start directly
	if (Maze::sStartingOut) return;

	// if energy is out, head back to the base!
	auto& sta = g.mEcs->GetComponent<Stats>(e);
	if (sta.energy < 0.1) {
		plog.state = CheckPointBase;
	}

	if (plog.state == CheckPointBase) {
		goToBase(g, e, path, plog);
		return;
	}

	if (plog.state < 0) return;


	if (rand() % 100 == 0) {
		findClosestTarget(g, e, pos, path, plog);
		std::cout << "GETTING NEW PATH " << std::endl;
	}


	// if getting too close to the target head to the base
	int targetX = fbl_get_sprite_x(plog.state);
	int targetY = fbl_get_sprite_y(plog.state);
	if (distance((int)pos.x, (int)pos.x, targetX, targetY) < 64) {
		plog.state = CheckPointBase;
		std::cout << "TOO CLOSE! HEADING TO BASE!" << std::endl;
	}

}

void DeathMatch::handleCoins(Entity e, Sprite& spr, PathLogic& plog) {

	// handle coin colissions
	for (int i = 0; i < Maze::cMaxCoins; i++) {

		if (Maze::sCoin[i].id == -1) continue;
		if (!fbl_get_sprite_collision(spr.id[0], Maze::sCoin[i].id)) continue;

		fbl_set_sprite_active(Maze::sCoin[i].id, false);
		Maze::sCoin[i].id = -1;
		plog.coins++;
		std::cout << "Player " << e << " has " << (int)plog.coins << std::endl;

	}

}

void DeathMatch::goToBase(Game& g, Entity e, Path& path, PathLogic& plog) {

	if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
		path.goalX = plog.baseX;
		path.goalY = plog.baseY;
		path.newPath = true;
		return;
	}
	else {
		auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

		if (!ctrl.active) {
			path.goalX = plog.baseX;
			path.goalY = plog.baseY;
			path.newPath = true;
			return;
		}
	}

	plog.state = GoingToBase;

}

void DeathMatch::handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog) {

	// if a robot collides with it's base, charge the battery and set out for nearest target

	// check to see if a robot has collided with a 20x20 square around the base coords, otherwise skip
	if (!(pos.x < (plog.baseX + 10) && pos.x >(plog.baseX - 10) &&
		pos.y < (plog.baseY + 10) && pos.y >(plog.baseY - 10))) return;

	//std::cout << "Collided with da base!" << std::endl;

	// charge battery
	auto& stat = g.mEcs->GetComponent<Stats>(e);
	stat.energy = stat.maxEnergy;

	if (Maze::sStartingOut) return;

	if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
		findClosestTarget(g, e, pos, path, plog);
	}
	else {
		auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

		if (!ctrl.active)
			findClosestTarget(g, e, pos, path, plog);
	}

}

void DeathMatch::findClosestTarget(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {

	bool targetsAvailable = false;
	int  targetX, targetY;
	int  shortestDistance = 1000;	// start with a very high number
	int  nearestTargetId = 0;

	// first check the distance to all the targets and pick the shortest path.

	for (int i = 0; i < g.mRobots->mNumRacers; i++) {

		// skip robot checking itself
		if (g.mRobots->mRacingRobots[i] == e) continue;

		auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[i]);

		// are any targets still alive?
		if (sta.hp > 0.1) {

			targetsAvailable = true;

			auto& spr = g.mEcs->GetComponent<Sprite>(g.mRobots->mRacingRobots[i]);

			targetX = fbl_get_sprite_x(spr.id[0]);
			targetY = fbl_get_sprite_y(spr.id[0]);

			fbl_pathf_set_path_status(path.id, fbl_pathf_find_path(path.id, pos.x, pos.y, targetX, targetY, path.diag));

			int pathLen = fbl_pathf_get_path_length(path.id);

			if (pathLen < shortestDistance) {
				shortestDistance = pathLen;	// update this
				nearestTargetId = spr.id[0];
			}

			//std::cout << "Distance to target: " << pathLen << std::endl;

		}

	}

	if (targetsAvailable) {
		path.goalX = fbl_get_sprite_x(nearestTargetId);
		path.goalY = fbl_get_sprite_y(nearestTargetId);
		plog.state = nearestTargetId;
	}
	else {
		// if no targets available, go to base for now
		path.goalX = plog.baseX;
		path.goalY = plog.baseY;
	}

	snapToGrid(path.goalX, path.goalY);
	path.newPath = true;

}

void DeathMatch::checkWinCondition(Game& g) {

	if (Race::sRaceState == Race::Undecided) {


		// check if all other robots are dead = win
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

void DeathMatch::switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {

	// This is used for setting up the correct behaviour when the player
	// clicks the passive RobotCtrl addon in a race.

	// update behaviour only for robots with a MouseCtrl component (i.e. the player)
	if (!g.mEcs->HasComponent<RobotCtrl>(e)) return;

	auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

	if (!ctrl.justSwitched) return;		// don't bother is player didn't just switch (from ctrl to automatic)

	std::cout << "JUST SWITCHED! for robot-entity: " << e << std::endl;

	if (!ctrl.active) {		// if the robot ctrl is deactivated, control goes back to auto

		if (!Maze::sStartingOut)
			findClosestTarget(g, e, pos, path, plog);

	}
	else {	// here it's active again

		// make robot stop
		fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);

	}

	ctrl.justSwitched = false;

}

void DeathMatch::updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog) {

	plog.state = CheckPointBase;

	// update paths for robots without a MouseCtrl component
	if (!g.mEcs->HasComponent<RobotCtrl>(e)) {
		findClosestTarget(g, e, pos, path, plog);
	}
	else {
		auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(e);

		if (!ctrl.active && !Maze::sStartingOut)
			findClosestTarget(g, e, pos, path, plog);
	}

	std::cout << "New path for robot-entity: " << e << std::endl;

}

void DeathMatch::snapToGrid(uint16_t& x, uint16_t& y) {

	while (x % 32 != 0) x--;
	while (y % 32 != 0) y--;

}

int DeathMatch::distance(int px, int py, int cx, int cy) {

	int dx = cx - px;
	int dy = cy - py;

	return std::sqrt(dx * dx + dy * dy);
}
