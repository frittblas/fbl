/*
*
*	Ecs
*
*	HealSystem.cpp
*
*	The heal system. Makes robot gain hp.
*
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../GameState/Race/Maze.hpp"

#include "../../../Game.hpp"

#include "HealSystem.hpp"


void HealSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);


	}


	std::cout << "Heal component system initialized!" << std::endl;

}

void HealSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);


	}

	
}
