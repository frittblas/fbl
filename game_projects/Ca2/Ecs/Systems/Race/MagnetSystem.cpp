/*
*
*	Ecs
*
*	MagnetSystem.cpp
*
*	The magnet system. Makes robot attract coins.
*
*
*	Hans Str�mquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../GameState/Race/Maze.hpp"

#include "MagnetSystem.hpp"

void MagnetSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& mag = ecs.GetComponent<Magnet>(entity);

		// create the magnetic effect sprite
		mag.spriteId = fbl_create_sprite(255, 0, 64, 64, 0);
		fbl_set_sprite_animation(mag.spriteId, true, 255, 0, 64, 64, 2, 15, true);
		fbl_set_sprite_layer(mag.spriteId, 8);			// on top of the robots
		fbl_set_sprite_active(mag.spriteId, false);		// set the sprite to off to begin with

	}


	std::cout << "Magnet component system initialized!" << std::endl;

}

void MagnetSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& sta = g.mEcs->GetComponent<Stats>(entity);
		auto& mag = g.mEcs->GetComponent<Magnet>(entity);
		
		bool isClose = false;

		// check if any coins are in within the "strength" (distance in pixels) param and attract them to the player.
		
		if (sta.hp > 0.1 && !Maze::sStartingOut) {

			if (mag.active) {

				for (int i = 0; i < Maze::cMaxCoins; i++) {

					if (Maze::sCoin[i].id != -1) {

						int cx = fbl_get_sprite_x(Maze::sCoin[i].id);
						int cy = fbl_get_sprite_y(Maze::sCoin[i].id);

						if (distance(pos.x + 16, pos.y + 16, cx, cy) < mag.strength + 2) {

							if (cx < pos.x + 16) fbl_set_sprite_xy(Maze::sCoin[i].id, cx + 1, cy);
							if (cx > pos.x + 16) fbl_set_sprite_xy(Maze::sCoin[i].id, cx - 1, cy);
							if (cy < pos.y + 16) fbl_set_sprite_xy(Maze::sCoin[i].id, fbl_get_sprite_x(Maze::sCoin[i].id), cy + 1);
							if (cy > pos.y + 16) fbl_set_sprite_xy(Maze::sCoin[i].id, fbl_get_sprite_x(Maze::sCoin[i].id), cy - 1);

							//std::cout << "Magnet active! distance: " << distance(pos.x + 16, pos.y + 16, cx, cy) << std::endl;

							isClose = true;

						}

					}

				}

			}
			else fbl_set_sprite_active(mag.spriteId, false);

			if (isClose) {
				fbl_set_sprite_xy(mag.spriteId, pos.x + 16, pos.y + 16);
				fbl_set_sprite_active(mag.spriteId, true);
			}
			else fbl_set_sprite_active(mag.spriteId, false);

		}
		else fbl_set_sprite_active(mag.spriteId, false);
		

	}

	
}

int MagnetSystem::distance(int px, int py, int cx, int cy) {

	int dx = cx - px;
	int dy = cy - py;

	return std::sqrt(dx * dx + dy * dy);
}
