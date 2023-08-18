/*
*
*	Ecs
*
*	DeathMatchSystem.hpp
*
*	The death match. Handles rules for this game mode.
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../../src/fbl.hpp"

#include "../../../Ecs.hpp"
#include "../../../Components.hpp"

#include "../../../../GameState/Race/Maze.hpp"

#include "../../../../Game.hpp"
#include "../../../../GameState/GameState.hpp"

#include "DeathMatchSystem.hpp"

extern Maze::aCoin gCoin[Maze::cMaxCoins]; // from Maze.cpp
extern bool gStartingOut;
extern bool gUpdatePaths;	// from Laser.cpp

void DeathMatchSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& plog = ecs.GetComponent<DeathMatch>(entity);

		// set coins to zero
		plog.coins = 0;

	}

	std::cout << "DeathMatch component system initialized!" << std::endl;
	
}

void DeathMatchSystem::Update(Game& g) {

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& dm = g.mEcs->GetComponent<DeathMatch>(entity);
		

		handleCoins(entity, spr, dm);
		handleBases(g, entity, pos, spr, path, dm);

	}

	updatePaths(g);
	switchCtrl(g);

}


void DeathMatchSystem::handleCoins(Entity e, Sprite& spr, DeathMatch& dm) {

	// handle coin colissions
	for (int i = 0; i < Maze::cMaxCoins; i++) {

		if (gCoin[i].id != -1) {
			if (fbl_get_sprite_collision(spr.id[0], gCoin[i].id)) {

				fbl_set_sprite_active(gCoin[i].id, false);
				gCoin[i].id = -1;
				dm.coins++;
				std::cout << "Player " << e << " has " << (int)dm.coins << std::endl;

			}
		}

	}

}

void DeathMatchSystem::handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, DeathMatch& plog) {

	// if a robot collides with it's base, drop any flags carried, charge the battery and set out for nearest flag

	// check to see if a robot has collided with a 20x20 square around the base coords.
	if (pos.x < (plog.baseX + 10) && pos.x > (plog.baseX - 10) &&
		pos.y < (plog.baseY + 10) && pos.y > (plog.baseY - 10)) {

		//std::cout << "Collided with da base!" << std::endl;


	}


}


void DeathMatchSystem::updatePaths(Game& g) {

	if (gUpdatePaths) {

		for (auto const& entity : mEntities)
		{

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<DeathMatch>(entity);




		}

		gUpdatePaths = false;

	}

}

void DeathMatchSystem::switchCtrl(Game& g) {

	// This is used for setting up the correct behaviour when the player
	// clicks the passive RobotCtrl addon in a race.

	for (auto const& entity : mEntities)
	{

		// update behaviour only for robots with a MouseCtrl component (i.e. the player)
		if (g.mEcs->HasComponent<RobotCtrl>(entity)) {

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<DeathMatch>(entity);
			auto& ctrl = g.mEcs->GetComponent<RobotCtrl>(entity);

			if (ctrl.justSwitched) {
				std::cout << "JUST SWITCHED! for robot-entity: " << entity << std::endl;
				if (!ctrl.active) {		// if the robot ctrl is deactivated, control goes back to auto

					// find path to weakest robot to kill :)

				}
				else {	// here it's active again

					// make robot stop
					fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);

				}

				ctrl.justSwitched = false;

			}

		}

	}

}
