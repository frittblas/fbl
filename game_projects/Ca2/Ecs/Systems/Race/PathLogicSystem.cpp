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
#include "../../../../Ca2/GameState/Race/Maze.hpp"

#include "../../../../Ca2/GameState/Race/GameModes/CaptureFlags.hpp"
#include "../../../../Ca2/GameState/Race/GameModes/DeathMatch.hpp"

#include "PathLogicSystem.hpp"

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

	switch (Maze::sGameMode) {

	case Race::GM_CaptureFlags:
		tickCF(g);
		break;
	case Race::GM_DeathMatch:
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


		Maze::sCF->handleFlags(entity, pos, spr, path, plog);
		Maze::sCF->handleCoins(entity, spr, plog);
		Maze::sCF->handleBases(g, entity, pos, spr, path, plog);

		Maze::sCF->switchCtrl(g, entity, pos, path, plog);

	}

	if (Maze::sUpdatePaths) {

		for (auto const& entity : mEntities)
		{

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<PathLogic>(entity);

			Maze::sCF->updatePaths(g, entity, pos, path, plog);
		}

		Maze::sUpdatePaths = false;
	}

	Maze::sCF->checkWinCondition(g);

}

void PathLogicSystem::tickDM(Game& g) {

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);
		auto& plog = g.mEcs->GetComponent<PathLogic>(entity);


		Maze::sDM->handleTargets(g, entity, pos, spr, path, plog);
		Maze::sDM->handleCoins(entity, spr, plog);
		Maze::sDM->handleBases(g, entity, pos, spr, path, plog);

		Maze::sDM->switchCtrl(g, entity, pos, path, plog);

	}

	if (Maze::sUpdatePaths) {

		for (auto const& entity : mEntities)
		{

			auto& pos = g.mEcs->GetComponent<Position>(entity);
			auto& path = g.mEcs->GetComponent<Path>(entity);
			auto& plog = g.mEcs->GetComponent<PathLogic>(entity);

			Maze::sDM->updatePaths(g, entity, pos, path, plog);
		}

		Maze::sUpdatePaths = false;
	}

	Maze::sDM->checkWinCondition(g);

}
