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

extern Maze::aFlag gFlag[Maze::cMaxFlags];
extern Maze::aCoin gCoin[Maze::cMaxCoins];

void PathLogicSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& plog = ecs.GetComponent<PathLogic>(entity);

		plog.hasFlag = Maze::FlagState::Center;

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
		

		// handle flag colissions
		for (int i = 0; i < Maze::cMaxFlags; i++) {

			if (fbl_get_sprite_collision(spr.id[0], gFlag[i].id)) {

				//std::cout << "collided with flag = " << std::endl;

				// if in center or dropped (not in base or held by a robot)
				if (gFlag[i].state == Maze::FlagState::Center || gFlag[i].state == Maze::FlagState::Dropped) {

					// pick up flag
					gFlag[i].state = entity;	// values equal to or over 0 is state == held by that robot entity
					plog.hasFlag = gFlag[i].id;	// the hasFlag is set to the sprite index of the flag

					// set course to the base
					path.goalX = plog.baseX;
					path.goalY = plog.baseY;
					path.newPath = true;

					std::cout << "Picked up flag!" << std::endl;

					i = Maze::cMaxFlags; // break from the loop (already got a flag)

				}

			}

			// move flags that are being held by robots
			if (gFlag[i].state == entity) {

				fbl_set_sprite_xy(gFlag[i].id, pos.x, pos.y);
				i = Maze::cMaxFlags; // break
			}

		}

		
		// handle coin colissions
		for (int i = 0; i < Maze::cMaxCoins; i++) {

			if (gCoin[i].id != -1) {
				if (fbl_get_sprite_collision(spr.id[0], gCoin[i].id)) {

					fbl_set_sprite_active(gCoin[i].id, false);
					gCoin[i].id = -1;
					plog.coins++;
					fbl_log("player %d has %d coins.", entity, plog.coins);

				}
			}


		}

	}

}

bool PathLogicSystem::hasFlag(Entity e) {

	for (int i = 0; i < Maze::cMaxFlags; i++)
		if (gFlag[i].state == e) return true;
		
	return false;

}