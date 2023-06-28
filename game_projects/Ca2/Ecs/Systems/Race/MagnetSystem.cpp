/*
*
*	Ecs
*
*	MagnetSystem.cpp
*
*	The magnet system. Makes robot attract coins.
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
/*
#include "../../../Robots.hpp"
#include "../../../Addons.hpp"
#include "../../../Efx.hpp"
*/
#include "MagnetSystem.hpp"

extern Maze::aCoin gCoin[Maze::cMaxCoins];	// from Maze.cpp

void MagnetSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& mag = ecs.GetComponent<Magnet>(entity);

		// create the magnetic effect sprite
		mag.spriteId = fbl_create_sprite(255, 0, 64, 64, 0);
		fbl_set_sprite_animation(mag.spriteId, true, 255, 0, 64, 64, 2, 15, true);
		fbl_set_sprite_active(mag.spriteId, false);		// set the sprite to off to begin with

	}

	std::cout << "Magnet component system initialized!" << std::endl;

}

void MagnetSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& mag = g.mEcs->GetComponent<Magnet>(entity);
		

		// check if any coins are in within the "strength" param and attract them to the player
		
		for (int i = 0; i < Maze::cMaxCoins; i++) {
		
			if (gCoin[i].id != -1) {

				int cx = fbl_get_sprite_x(gCoin[i].id);
				int cy = fbl_get_sprite_y(gCoin[i].id);

				if (distance(pos.x + 16, pos.y + 16, cx + 8, cy + 8) < mag.strength) {

					if (cx + 8 < pos.x + 16) fbl_set_sprite_xy(gCoin[i].id, cx + 1, cy);
					if (cx + 8 > pos.x + 16) fbl_set_sprite_xy(gCoin[i].id, cx - 1, cy);
					if (cy + 8 < pos.y + 16) fbl_set_sprite_xy(gCoin[i].id, fbl_get_sprite_x(gCoin[i].id), cy + 1);
					if (cy + 8 > pos.y + 16) fbl_set_sprite_xy(gCoin[i].id, fbl_get_sprite_x(gCoin[i].id), cy - 1);

					std::cout << "Magnet active! distance: " << distance(pos.x + 16, pos.y + 16, cx + 8, cy + 8) << std::endl;


				}

			}
		
		}
		

	}
	
}

int MagnetSystem::distance(int px, int py, int cx, int cy) {

	int dx = cx - px;
	int dy = cy - py;

	return std::sqrt(dx * dx + dy * dy);
}
