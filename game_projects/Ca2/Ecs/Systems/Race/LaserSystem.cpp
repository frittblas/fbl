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

#include "../../../GameState/Race/Maze.hpp"

#include "../../../Game.hpp"
#include "../../../Robots.hpp"
#include "../../../Addons.hpp"
#include "../../../Efx.hpp"	// remove this (observer pattern)

#include "LaserSystem.hpp"

extern Maze::aFlag gFlag[Maze::cMaxFlags];

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

		// not firing yet!
		las.isFiring = false;

	}

	std::cout << "Laser component system initialized!" << std::endl;
	
}

void LaserSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& sta = g.mEcs->GetComponent<Stats>(entity);
		auto& aim = g.mEcs->GetComponent<AutoAim>(entity);
		auto& las = g.mEcs->GetComponent<Laser>(entity);

		// get info from autoAim component if there is a target (and if auto-aim is active)
		if (aim.active) {

			if (aim.hasTarget) {
				las.isFiring = true;
				las.dir = aim.dir;
			}
			else las.isFiring = false;

		}

		// only fire if (bool)firing is true
		if (las.isFiring) {

			fbl_set_prim_active(las.rayId, true);	// show the ray

			setDirection(pos, las);

			// some ray hit detection
			int id, x, y;
			fbl_get_ray_hit_sprite(las.rayId, &id, &x, &y);
			fbl_set_emitter_xy(las.particleId, x, y);	// position the particles where the ray hit

			if (id != -1) {	// do the following if the laser didn't miss completely (didn't even hit a rock)

				if(x > 25 && y > 25)	// avoid having the particles spawn at x == 0 and y == 0
					fbl_set_emitter_active(las.particleId, true);	// only turn the particles on if the ray hit something

				std::cout << "id hit = " << id << std::endl;

				//printf("Ray 0 hit sprite: %d at x: %d, y: %d\n", id, x, y);

				// check if a laser has hit another robot
				for (int i = 0; i < g.mRobots->mNumRacers; i++)
					if (g.mRobots->mSpriteIdToEntityMap[id] == g.mRobots->mRacingRobots[i]) {
						auto& targetSta = g.mEcs->GetComponent<Stats>(g.mRobots->mSpriteIdToEntityMap[id]);
						targetSta.hp--;
						if (targetSta.hp <= 0) {
							auto& targetPlog = g.mEcs->GetComponent<PathLogic>(g.mRobots->mSpriteIdToEntityMap[id]);
							auto& targetSpr = g.mEcs->GetComponent<Sprite>(g.mRobots->mSpriteIdToEntityMap[id]);
							auto& targetAim = g.mEcs->GetComponent<AutoAim>(g.mRobots->mSpriteIdToEntityMap[id]);
							Laser* targetLas = nullptr;
							if(g.mEcs->HasComponent<Laser>(g.mRobots->mSpriteIdToEntityMap[id]))
								targetLas = &g.mEcs->GetComponent<Laser>(g.mRobots->mSpriteIdToEntityMap[id]);
							
							targetSta.hp = 0;	// keep hp at 0
							fbl_set_sprite_active(targetSpr.id[0], false);				// turn off sprite (dead)
							fbl_set_sprite_phys(targetSpr.id[0], false, 0, 0, false);	// turn off ray colission
							targetAim.active = false;									// can't target people when dead
							if (targetLas) targetLas->isFiring = false;
							Efx::getInstance().shakeCamera(20, 40);						// shake camera
							fbl_set_emitter_active(las.particleId, false);				// turn off emitter making a cloud

							// drop the flag
							for (int j = 0; j < Maze::cMaxFlags; j++) {
								if (gFlag[j].state == g.mRobots->mSpriteIdToEntityMap[id]) {
									gFlag[j].state = Maze::FlagState::Dropped;
									break;
								}
							}
							targetPlog.hasFlag = Maze::FlagState::Dropped;
							
						}
						//std::cout << sta.name << " killed " << targetSta.name << std::endl;
						break;	// no need to check the other robots after a hit
					}

			}
			else fbl_set_emitter_active(las.particleId, false);	// turn off particles if ray didn't hit anything

		}
		else {
			// turn these off if !firing, it's ok to call every frame
			fbl_set_prim_active(las.rayId, false);
			fbl_set_emitter_active(las.particleId, false);
		}


	}

}


void LaserSystem::setDirection(Position& pos, Laser& las) {

	// set laser direction (the robots have slightly smaller physics hitbox, makes things easier to manage (ray colissions))

	switch (las.dir) {

	case Addons::Dir::Up:
		fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize / 2, pos.y + 1);

		// can't draw ray with minus coordinates
		if (pos.y - las.length < 0)
			fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, 0, 0);
		else
			fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, pos.y - las.length, 0);
		break;

	case Addons::Dir::Right:
		fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize - 1, pos.y + Game::TileSize / 2);
		fbl_set_prim_size(las.rayId, pos.x + las.length, pos.y + Game::TileSize / 2, 0);
		break;

	case Addons::Dir::Left:
		fbl_set_prim_xy(las.rayId, pos.x + 1, pos.y + Game::TileSize / 2);

		// can't draw ray with minus coordinates
		if (pos.x - las.length < 0)
			fbl_set_prim_size(las.rayId, 0, pos.y + Game::TileSize / 2, 0);
		else
			fbl_set_prim_size(las.rayId, pos.x - las.length, pos.y + Game::TileSize / 2, 0);
		break;

	case Addons::Dir::Down:
		fbl_set_prim_xy(las.rayId, pos.x + Game::TileSize / 2, pos.y + Game::TileSize - 1);
		fbl_set_prim_size(las.rayId, pos.x + Game::TileSize / 2, pos.y + las.length, 0);
		break;

	}

}