/*
*
*	Ecs
*
*	LaserSystem.cpp
*
*	The laser system. It's raycasting and possibly particle efx at the end.
*
*
*	Hans Str�mquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../Robots.hpp"
#include "../../../Addons.hpp"

#include "LaserSystem.hpp"

void LaserSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& las = ecs.GetComponent<Laser>(entity);

		// create the ray with 0 length
		las.rayId = fbl_create_prim(FBL_RAY, 0, 0, 0, 0, 0, true, false);
		fbl_set_prim_active(las.rayId, false);

		// create the particles
		las.particleId = fbl_create_emitter(200);
		fbl_set_emitter_params(las.particleId, FBL_EMITTER_FLOWER, 2, 2, 55, 1, 1, 0.2, 0.8);
		fbl_set_emitter_particle_shape(las.particleId, FBL_NO_PRIM, 448, 128, 64, 64);	// use particle image instead of prim
		fbl_set_emitter_color(las.particleId, 120, 120, 120, 200, true);
		fbl_set_emitter_color(las.particleId, 0, 0, 0, 0, false);
		// only activate when you use the laser (create smoke where the laser hits)
		fbl_set_emitter_active(las.particleId, false);
	}

	std::cout << "Laser component system initialized!" << std::endl;
	
}

void LaserSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& sta = g.mEcs->GetComponent<Stats>(entity);
		auto& las = g.mEcs->GetComponent<Laser>(entity);

		if(fbl_get_key_down(FBLK_Z)) las.firing = true;
		if (fbl_get_key_up(FBLK_Z)) las.firing = false;

		// only fire if (bool)firing is true

		if (las.firing) {

			fbl_set_prim_active(las.rayId, true);	// show the ray


			// set laser direction (the robots have slightly smaller physics hitbox, makes things easier to manage (ray colissions))

			switch (las.dir) {

				case Addons::Dir::Up :
					fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize / 2, pos.y + 1);

					// can't draw ray with minus coordinates
					if(pos.y - las.length < 0)
						fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, 0, 0);
					else
						fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, pos.y - las.length, 0);
					break;

				case Addons::Dir::Right :
					fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize - 1, pos.y + Game::TileSize / 2);
					fbl_set_prim_size(las.rayId, pos.x + las.length, pos.y + Game::TileSize / 2, 0);
					break;

				case Addons::Dir::Left :
					fbl_set_prim_xy(las.rayId, pos.x + 1, pos.y + Game::TileSize / 2);

					// can't draw ray with minus coordinates
					if (pos.x - las.length < 0)
						fbl_set_prim_size(las.rayId, 0, pos.y + Game::TileSize / 2, 0);
					else
						fbl_set_prim_size(las.rayId, pos.x - las.length, pos.y + Game::TileSize / 2, 0);
					break;

				case Addons::Dir::Down :
					fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize / 2, pos.y + Game::TileSize - 1);
					fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, pos.y + las.length, 0);
					break;

			}

			// some ray hit detection
			int id, x, y;
			fbl_get_ray_hit_sprite(las.rayId, &id, &x, &y);
			fbl_set_emitter_xy(las.particleId, x, y);	// position the particles where the ray hit
			if (id != -1) {

				fbl_set_emitter_active(las.particleId, true);	// only turn the particles on if the ray hit something
				std::cout << "id hit = " << id << std::endl;

				//printf("Ray 0 hit sprite: %d at x: %d, y: %d\n", id, x, y);

				// check if a laser has hit another robot
				for (int i = 0; i < g.mRobots->mNumRacers; i++)
					if (g.mRobots->mSpriteIdToEntityMap[id] == g.mRobots->mRacingRobots[i]) {
						auto& targetSta = g.mEcs->GetComponent<Stats>(g.mRobots->mSpriteIdToEntityMap[id]);
						std::cout << sta.name << " killed " << targetSta.name << std::endl;
					}

			}
			else fbl_set_emitter_active(las.particleId, false);	// turn off particles if ray didn't hit anything

		}
		else {
			// turn these of if !firing, it's ok to call every frame
			fbl_set_prim_active(las.rayId, false);
			fbl_set_emitter_active(las.particleId, false);
		}


	}

}
