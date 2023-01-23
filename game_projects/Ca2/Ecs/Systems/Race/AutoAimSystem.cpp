/*
*
*	Ecs
*
*	AutoAim.cpp
*
*	The auto aim system. Raycasting an invisible ray in all directions looking for targets.
*
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../Robots.hpp"
#include "../../../Addons.hpp"

#include "AutoAimSystem.hpp"

void AutoAimSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& aim = ecs.GetComponent<AutoAim>(entity);

		// create the ray with 0 length
		aim.rayId = fbl_create_prim(FBL_RAY, 0, 0, 0, 0, 0, true, false);
		fbl_set_prim_color(aim.rayId, 0, 200, 0, 50);
	}

	std::cout << "AutoAim component system initialized!" << std::endl;
	
}

void AutoAimSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& sta = g.mEcs->GetComponent<Stats>(entity);
		auto& aim = g.mEcs->GetComponent<AutoAim>(entity);

		fbl_set_prim_active(aim.rayId, true);	// show the ray

		setDirection(pos, aim);

		// some ray hit detection
		int id, x, y;
		fbl_get_ray_hit_sprite(aim.rayId, &id, &x, &y);

		if (id != -1) {


			// check if a ray has hit a robot
			for (int i = 0; i < g.mRobots->mNumRacers; i++)
				if (g.mRobots->mSpriteIdToEntityMap[id] == g.mRobots->mRacingRobots[i]) {
					auto& targetSta = g.mEcs->GetComponent<Stats>(g.mRobots->mSpriteIdToEntityMap[id]);
					std::cout << sta.name << " killed " << targetSta.name << std::endl;
				}

		}


	}

}


void AutoAimSystem::setDirection(Position& pos, AutoAim& aim) {


	switch (aim.hasShotDir) {

		case Addons::Dir::Up:
			fbl_set_prim_xy(aim.rayId, pos.x + Game::TileSize / 2, pos.y + 1);

			// can't draw ray with minus coordinates
			if (pos.y - aim.length < 0)
				fbl_set_prim_size(aim.rayId, pos.x + Game::TileSize / 2, 0, 0);
			else
				fbl_set_prim_size(aim.rayId, pos.x + Game::TileSize / 2, pos.y - aim.length, 0);
			break;

		case Addons::Dir::Right:
			fbl_set_prim_xy(aim.rayId, pos.x + Game::TileSize - 1, pos.y + Game::TileSize / 2);
			fbl_set_prim_size(aim.rayId, pos.x + aim.length, pos.y + Game::TileSize / 2, 0);
			break;

		case Addons::Dir::Left:
			fbl_set_prim_xy(aim.rayId, pos.x + 1, pos.y + Game::TileSize / 2);

			// can't draw ray with minus coordinates
			if (pos.x - aim.length < 0)
				fbl_set_prim_size(aim.rayId, 0, pos.y + Game::TileSize / 2, 0);
			else
				fbl_set_prim_size(aim.rayId, pos.x - aim.length, pos.y + Game::TileSize / 2, 0);
			break;

		case Addons::Dir::Down:
			fbl_set_prim_xy(aim.rayId, pos.x + Game::TileSize / 2, pos.y + Game::TileSize - 1);
			fbl_set_prim_size(aim.rayId, pos.x + Game::TileSize / 2, pos.y + aim.length, 0);
			break;

		}

}
