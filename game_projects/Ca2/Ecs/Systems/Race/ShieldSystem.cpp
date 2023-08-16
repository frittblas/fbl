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

		// create shield (sprite circle)
		shield.spriteId = fbl_create_sprite(406, 214, 42, 42, 21);
		fbl_set_sprite_layer(shield.spriteId, 8);			// on top of the robots

		// set color
		switch (shield.level) {
		case 1:
			fbl_set_sprite_color(shield.spriteId, 255, 70, 70);
			break;
		case 2:
			fbl_set_sprite_color(shield.spriteId, 70, 255, 70);
			break;
		case 3:
			fbl_set_sprite_color(shield.spriteId, 70, 70, 255);
			break;
		}

		fbl_set_sprite_active(shield.spriteId, false);

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
				fbl_set_sprite_active(shield.spriteId, true);
				fbl_set_sprite_xy(shield.spriteId, pos.x + 15, pos.y + 15);

				stat.energy -= static_cast<double>(shield.energyCost) / 20;

			}
			else fbl_set_sprite_active(shield.spriteId, false);

		}
		else fbl_set_sprite_active(shield.spriteId, false);
	}

	
}
