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

		auto& heal = ecs.GetComponent<Heal>(entity);

		// create the particles
		heal.particleId = fbl_create_emitter(200);
		fbl_set_emitter_params(heal.particleId, FBL_EMITTER_FLOWER, 2, 2, 80, 1, 1, 0.5, 2.0);
		fbl_set_emitter_particle_shape(heal.particleId, FBL_NO_PRIM, 375, 288, 9, 8);	// use particle image instead of prim

		// only activate when you use the healing
		fbl_set_emitter_active(heal.particleId, false);


	}


	std::cout << "Heal component system initialized!" << std::endl;

}

void HealSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& stat = g.mEcs->GetComponent<Stats>(entity);
		auto& heal = g.mEcs->GetComponent<Heal>(entity);

		// check if healing has been activated
		if (heal.activated) {

			// check if there's healing left to be had (and energy + not att full health and not dead) 
			if (heal.amountLeft > 0 && stat.energy > 0 && (stat.hp < stat.maxHp && stat.hp > 0)) {

				// activate healing particle effect
				fbl_set_emitter_active(heal.particleId, true);
				fbl_set_emitter_xy(heal.particleId, pos.x + 16, pos.y + 16);

				stat.hp += 0.4;	// fixed rate
				heal.amountLeft--;
				stat.energy -= static_cast<double>(heal.energyCost) / 10;

			}
			else fbl_set_emitter_active(heal.particleId, false);

		}
		else fbl_set_emitter_active(heal.particleId, false);

	}

	
}
