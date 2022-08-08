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

		// loop through all the sprites (max 4), create them and set som params
		for (int i = 0; i < spr.num; i++) {
			spr.id[i] = fbl_create_sprite(i * (spr.w * spr.frames), spr.textureY, spr.w, spr.h, 0);	// create a sprite
			fbl_set_sprite_xy(spr.id[i], pos.x, pos.y);	// set the coordinates
			if (spr.animated)	// turn on animation if requested
				fbl_set_sprite_animation(spr.id[i], true, spr.textureX + (i * (spr.w * spr.frames)), spr.textureY, spr.w, spr.h, spr.frames, spr.speed, true);
			
			// deactivate all sprites
			fbl_set_sprite_active(spr.id[i], false);
			
		}

		// turn on the one facing the current direction
		fbl_set_sprite_active(spr.id[spr.dir], true);

	}

	std::cout << "Sprite system init!" << std::endl;

}

void SpriteSystem::Update(Coordinator& ecs) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& spr = ecs.GetComponent<Sprite>(entity);

		// update sprite direction only if different from last frame
		if (spr.dir != spr.dirLast) {

			// first set all sprites to inactive
			for(int i = 0; i < spr.num; i++)
				fbl_set_sprite_active(spr.id[i], false);

			// then turn on the sprite facing the current dir
			fbl_set_sprite_active(spr.id[spr.dir], true);

			spr.dirLast = spr.dir;	// update the last frame dir to current dir

		}

		fbl_set_sprite_xy(spr.id[spr.dir], pos.x, pos.y);

	}

}
