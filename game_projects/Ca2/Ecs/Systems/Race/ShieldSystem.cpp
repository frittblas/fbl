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
		auto& shield = ecs.GetComponent<Shield>(entity);

		// create shield (sprite circle with physics)
		shield.spriteId = fbl_create_sprite(406, 214, 42, 42, 0);
		
		//fbl_set_sprite_active(shield.spriteId, false);

	}


	std::cout << "Shield component system initialized!" << std::endl;

}

void ShieldSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);


	}

	
}
