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
		fbl_set_sprite_phys(shield.spriteId, true, FBL_CIRCLE, FBL_PHYS_DYNAMIC, false);
		fbl_set_sprite_layer(shield.spriteId, 8);			// on top of the robots

		//fbl_set_sprite_active(shield.spriteId, false);

	}


	std::cout << "Shield component system initialized!" << std::endl;

}

void ShieldSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& stat = g.mEcs->GetComponent<Stats>(entity);
		auto& shield = g.mEcs->GetComponent<Shield>(entity);

		// check if healing has been activated
		if (shield.isShielding) {

			// check if there is life and energy left
			if (stat.energy > 0 && stat.hp > 0) {

				// activate shield effect
				//fbl_set_sprite_active(shield.spriteId, true);
				fbl_set_sprite_xy(shield.spriteId, pos.x, pos.y);

				stat.energy -= static_cast<double>(shield.energyCost) / 10;

			}
			else fbl_set_sprite_xy(shield.spriteId, -500, -500);

		}
		else fbl_set_sprite_xy(shield.spriteId, -500, -500);
	}

	
}
