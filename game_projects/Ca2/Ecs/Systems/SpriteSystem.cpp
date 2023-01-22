/*
*
*	Ecs
*
*	SpriteSystem.cpp
*
*	The sprite system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "SpriteSystem.hpp"

void SpriteSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& spr = ecs.GetComponent<Sprite>(entity);

		// loop through all the sprites (max 4), create them and set the required parameters
		for (int i = 0; i < spr.num; i++) {
			spr.id[i] = fbl_create_sprite(i * (spr.w * spr.frames), spr.textureY, spr.w, spr.h, 0);	// create a sprite
			fbl_set_sprite_xy(spr.id[i], pos.x, pos.y);	// set the coordinates
			if (spr.animated)	// turn on animation if it's requested
				fbl_set_sprite_animation(spr.id[i], true, spr.textureX + (i * (spr.w * spr.frames)), spr.textureY, spr.w, spr.h, spr.frames, spr.speed, true);
			
			// set layer
			fbl_set_sprite_layer(spr.id[i], spr.layer);

			// deactivate all sprites
			fbl_set_sprite_active(spr.id[i], false);
			
		}

		// for components with only one sprite use the [0]
		if (spr.num == 1) {
			spr.id[1] = spr.id[0];
			spr.id[2] = spr.id[0];
			spr.id[3] = spr.id[0];
		}

		// turn on the one facing the current direction
		fbl_set_sprite_active(spr.id[spr.dir], true);

	}

	std::cout << "Sprite component system initialized!" << std::endl;

}

void SpriteSystem::Update(Coordinator& ecs) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& spr = ecs.GetComponent<Sprite>(entity);

		// update sprite direction only if it's different from last frame
		// if spr.num > 1 (player for instance) take care of directions etc. otherwise (1 sprite for robots), draw offset sprite
		if (spr.num > 1) {
			if (spr.dir != spr.dirLast) {

				// first set all sprites to inactive
				for (int i = 0; i < spr.num; i++)
					fbl_set_sprite_active(spr.id[i], false);

				// then turn on the sprite facing the current dir
				fbl_set_sprite_active(spr.id[spr.dir], true);

				spr.dirLast = spr.dir;	// update the last frame dir to current dir

				std::cout << "Changed dir!" << std::endl;

			}
			fbl_set_sprite_xy(spr.id[spr.dir], pos.x, pos.y);
		}
		else
			fbl_set_sprite_xy(spr.id[0], pos.x + 16, pos.y + 16);	// robots get drawn from center in the race

	}

}
