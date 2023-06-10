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

extern int gFlagId;
extern int gCoinId[50];

void PathLogicSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& path = ecs.GetComponent<Path>(entity);
		auto& pos = ecs.GetComponent<Position>(entity);

	}

	std::cout << "PathLogic component system initialized!" << std::endl;
	
}

void PathLogicSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& plog = g.mEcs->GetComponent<PathLogic>(entity);
		
		if (fbl_get_sprite_collision(spr.id[0], gFlagId)) {

			path.goalX = plog.baseX;
			path.goalY = plog.baseY;
			path.newPath = true;

		}
		
		for (int i = 0; i < 50; i++) {

			if (fbl_get_sprite_collision(spr.id[0], gCoinId[i])) {

				fbl_set_sprite_active(gCoinId[i], false);

			}


		}

	}

}
