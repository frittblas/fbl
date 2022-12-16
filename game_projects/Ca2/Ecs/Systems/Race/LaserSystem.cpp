/*
*
*	Ecs
*
*	LaserSystem.cpp
*
*	The laser system. It's raycasting and possibly particle efx at the end.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../Robots.hpp"

#include "LaserSystem.hpp"

void LaserSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& las = ecs.GetComponent<Laser>(entity);

		// create the ray
		las.rayId = fbl_create_prim(FBL_RAY, 0, 0, 100, 100, 0, true, false);

		// create the particles
		las.particleId = fbl_create_emitter(30);
		fbl_set_emitter_params(las.particleId, FBL_EMITTER_FLOWER, 2, 2, 60, 1, 3, 0.2, 1.0);
		fbl_set_emitter_particle_shape(las.particleId, FBL_NO_PRIM, 448, 128, 64, 64);	// use particle image instead of prim
		// only activate when you use the laser (create smoke where the laser hits)
		//fbl_set_emitter_active(las.particleId, false);
	}

	std::cout << "Laser component system initialized!" << std::endl;
	
}

void LaserSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& sta = g.mEcs->GetComponent<Stats>(entity);
		auto& las = g.mEcs->GetComponent<Laser>(entity);

		fbl_set_emitter_xy(las.particleId, pos.x + Game::TileSize, pos.y + Game::TileSize / 2);

		fbl_set_prim_xy(las.rayId, pos.x - 48, pos.y - 48);
		fbl_set_prim_size(las.rayId, pos.x - 200, pos.y - 200, 0);

		int id, x, y;
		fbl_get_ray_hit_sprite(las.rayId, &id, &x, &y);
		if (id != -1) {

			//printf("Ray 0 hit sprite: %d at x: %d, y: %d\n", id, x, y);
			
			// check if a laser has hit another robot
			for(int i = 0; i < g.mRobots->mNumRacers; i++)
				if (g.mRobots->mSpriteIdToEntityMap[id] == g.mRobots->mRacingRobots[i]) {
					auto& targetSta = g.mEcs->GetComponent<Stats>(g.mRobots->mSpriteIdToEntityMap[id]);
					std::cout << sta.name << " killed " << targetSta.name << std::endl;
				}
			
		}


	}

}
