/*
*
*	Ecs
*
*	PathLogicSystem.hpp
*
*	The path logic system header. Handles when robots get new paths and stuff. Became more of a game mode coordinator.
*
*	Hans Strömquist 2023
*
*/

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../../Ca2/GameState/Race/Race.hpp"

#include "../../../../Ca2/GameState/Race/GameModes/CaptureFlags.hpp"
#include "../../../../Ca2/GameState/Race/GameModes/DeathMatch.hpp"

#include "PathLogicSystem.hpp"

// from Maze.cpp
extern bool gUpdatePaths;
extern int  gGameMode;
extern CaptureFlags* gCF;
extern DeathMatch*   gDM;

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

	switch (gGameMode) {

	case Race::CaptureFlags:
		tickCF(g);
		break;
	case Race::DeathMatch:
		tickDM(g);
		break;

	}

}

void PathLogicSystem::tickCF(Game& g) {

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& plog = g.mEcs->GetComponent<PathLogic>(entity);


		gCF->handleFlags(entity, pos, spr, path, plog);
		gCF->handleCoins(entity, spr, plog);
		gCF->handleBases(g, entity, pos, spr, path, plog);

		gCF->switchCtrl(g, entity, pos, path, plog);

	}

	if (gUpdatePaths) {

		for (auto const& entity : mEntities)
		{

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<PathLogic>(entity);

			gCF->updatePaths(g, entity, pos, path, plog);
		}

		gUpdatePaths = false;
	}

}

void PathLogicSystem::tickDM(Game& g) {

}
