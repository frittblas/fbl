/*
*
*	Ecs
*
*	ShieldSystem.cpp
*
*	The shield system. Makes robot shield against laser.
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

#include "ShieldSystem.hpp"


void ShieldSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);


	}


	std::cout << "Shield component system initialized!" << std::endl;

}

void ShieldSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);


	}

	
}
