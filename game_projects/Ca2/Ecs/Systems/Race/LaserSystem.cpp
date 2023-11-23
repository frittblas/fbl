/*
*
*	Ecs
*
*	LaserSystem.cpp
*
*	The laser system. It's raycasting, line primitives and particle efx at the end.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../GameState/Race/Maze.hpp"

#include "../../../Efx.hpp"
#include "../../../Game.hpp"
#include "../../../Robots.hpp"
#include "../../../Progress.hpp"
#include "../../../Addons.hpp"

#include "LaserSystem.hpp"

void LaserSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& las = ecs.GetComponent<Laser>(entity);
		auto& aim = ecs.GetComponent<AutoAim>(entity);

		// create crosshair
		las.crossHairId = fbl_create_prim(FBL_NORMAL_RECT, 0, 0, 2, 2, 0, 0, true);
		fbl_set_prim_color(las.crossHairId, 255, 255, 255, 255);
		fbl_set_prim_active(las.crossHairId, false);

		// create the ray with 0 length
		las.rayId = fbl_create_prim(FBL_RAY, 0, 0, 0, 0, 0, true, false);
		// set color
		switch (las.level) {
		case 1:
			fbl_set_prim_color(las.rayId, 255, 70, 70, 255);
			break;
		case 2:
			fbl_set_prim_color(las.rayId, 70, 255, 70, 255);
			break;
		case 3:
			fbl_set_prim_color(las.rayId, 70, 70, 255, 255);
			break;
		}
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

		// only show the crosshair if the robots are showing
		if (Maze::sPickDone && aim.active) fbl_set_prim_active(las.crossHairId, true);

		// get info from autoAim component if there is a target (and if auto-aim is active)
		if (aim.active) {

			if (aim.hasTarget) las.isFiring = true;
			else las.isFiring = false;

			las.dir = aim.dir;

		}

		// set crosshair
		switch (las.dir) {
			case Addons::Dir::Up :
				fbl_set_prim_xy(las.crossHairId, pos.x + 16, pos.y - 2);
				break;
			case Addons::Dir::Right:
				fbl_set_prim_xy(las.crossHairId, pos.x + 32, pos.y + 16);
				break;
			case Addons::Dir::Down:
				fbl_set_prim_xy(las.crossHairId, pos.x + 16, pos.y + 32);
				break;
			case Addons::Dir::Left:
				fbl_set_prim_xy(las.crossHairId, pos.x - 2, pos.y + 16);
				break;
		}

		// skip if dead or out of energy
		if (sta.hp < 0.1 || sta.energy < 0.1) {
			fbl_set_prim_active(las.rayId, false);
			fbl_set_emitter_active(las.particleId, false);
			fbl_set_prim_active(las.crossHairId, false);
			continue;
		}
		else if(sta.hp > 0.1 && sta.energy > 0.1 && Maze::sPickDone)
			fbl_set_prim_active(las.crossHairId, true);

		// only fire if (bool)isFiring is true
		if (!las.isFiring) {
			// turn these off if !firing, it's ok to call every frame
			fbl_set_prim_active(las.rayId, false);
			fbl_set_emitter_active(las.particleId, false);
			continue;
		}


		firedLaser();

		fbl_set_prim_active(las.rayId, true);	// show the ray

		setDirection(pos, las);

		sta.energy -= static_cast<double>(las.energyCost) / 20;	// energy cost

		// some ray hit detection
		int id, x, y;
		fbl_get_ray_hit_sprite(las.rayId, &id, &x, &y);
		fbl_set_emitter_xy(las.particleId, x, y);	// position the particles where the ray hit

		if (id != -1) {	// do the following if the laser didn't miss completely (didn't even hit a rock)

			if (x > 25 && y > 25)	// avoid having the particles spawn at x == 0 and y == 0
				fbl_set_emitter_active(las.particleId, true);	// only turn the particles on if the ray hit something

			std::cout << "id hit = " << id << std::endl;

			//printf("Ray 0 hit sprite: %d at x: %d, y: %d\n", id, x, y);

			dealDamage(g, entity, g.mRobots->mSpriteIdToEntityMap[id]);

		}
		else fbl_set_emitter_active(las.particleId, false);	// turn off particles if ray didn't hit anything

		// only show the crosshair if the robots are showing
		if(Maze::sPickDone) fbl_set_prim_active(las.crossHairId, true);

		// don't show the crosshair if robot's dead
		if(sta.hp < 0.1) fbl_set_prim_active(las.crossHairId, false);

	}

}

void LaserSystem::dealDamage(Game &g, Entity attacker, Entity target) {

	// check if a laser has hit another robot
	for (int i = 0; i < g.mRobots->mNumRacers; i++) {

		if (target != g.mRobots->mRacingRobots[i]) continue;	// skip if target is not a robot

		auto& attackLas = g.mEcs->GetComponent<Laser>(attacker);
		auto& targetSta = g.mEcs->GetComponent<Stats>(target);
		auto& light = g.mEcs->GetComponent<Light>(target);

		// avoid shooting yourself
		if (attacker == target) continue;

		// if target has shield
		Shield* targetShield = nullptr;
		if (g.mEcs->HasComponent<Shield>(target))
			targetShield = &g.mEcs->GetComponent<Shield>(target);

		if (targetShield) {

			if (targetShield->isShielding && targetSta.energy > 0 && targetSta.hp > 0) {
				
				// nothing (don't take damage) NOTE: match the different shields and lasers here
				// red lasers can be blocked by red shield etc.

				if(attackLas.level == targetShield->level + 1)
					targetSta.hp -= static_cast<double>(attackLas.damage / 3) / 10;
				if (attackLas.level == targetShield->level + 2)
					targetSta.hp -= static_cast<double>(attackLas.damage / 2) / 10;
				
			}
			else targetSta.hp -= static_cast<double>(attackLas.damage) / 10;

		}
		else targetSta.hp -= static_cast<double>(attackLas.damage) / 10;

		// if target dead
		if (targetSta.hp <= 0) {
			//auto& targetPlog = g.mEcs->GetComponent<PathLogic>(target);
			auto& attackSta = g.mEcs->GetComponent<Stats>(attacker);
			auto& targetSpr = g.mEcs->GetComponent<Sprite>(target);
			auto& targetAim = g.mEcs->GetComponent<AutoAim>(target);
			auto& targetPos = g.mEcs->GetComponent<Position>(target);
			Laser* targetLas = nullptr;
			if (g.mEcs->HasComponent<Laser>(target))
				targetLas = &g.mEcs->GetComponent<Laser>(target);

			targetSta.hp = 0;	// keep hp at 0
			fbl_set_sprite_active(targetSpr.id[0], false);				// turn off sprite (dead)
			fbl_set_sprite_phys(targetSpr.id[0], false, 0, 0, false);	// turn off ray colission
			fbl_set_sprite_active(light.id, false);						// turn off the light sprite (dead)
			fbl_set_prim_active(targetAim.rayId, false);				// turn off the aim prim
			if (targetLas) fbl_set_prim_active(targetLas->crossHairId, false);	// turn off the crosshair for targetLaser
			targetAim.active = false;									// can't target people when dead
			if (targetLas) targetLas->isFiring = false;
			//Efx::getInstance().shakeCamera(20, 40);					// shake camera
			fbl_set_emitter_active(attackLas.particleId, false);		// turn off emitter, making a cloud

			// explosion
			Efx::getInstance().startExplosion(targetPos.x, targetPos.y);

			robotDied();

			// drop the flag when dead (doesn't do anything in other game modes)
			for (int j = 0; j < Maze::cMaxFlags; j++) {
				if (Maze::sFlag[j].state == target) {
					Maze::sFlag[j].state = Maze::FlagState::Dropped;
					Maze::sUpdatePaths = true;
					break;
				}
			}

			if(attacker == g.mProgress->mFavRobot)
				attackSta.xp += 2;	// 2 XP for killing a robot, only for player

		}
		//std::cout << sta.name << " killed " << targetSta.name << std::endl;
		break;	// no need to check the other robots after a hit

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

void LaserSystem::attachObserver(IObserver* observer) {

	observers.push_back(observer);

}

void LaserSystem::detachObserver(IObserver* observer) {

	// Find the observer in the vector and remove it
	auto obs = std::find(observers.begin(), observers.end(), observer);
	if (obs != observers.end()) {
		observers.erase(obs);
	}

}

void LaserSystem::freeObserverList() {

	observers.clear();

}

void LaserSystem::robotDied() {

	// Notify all attached observers
	for (IObserver* observer : observers)
		observer->onRobotDeath();

}

void LaserSystem::firedLaser() {

	for (IObserver* observer : observers)
		observer->onFireLaser();

}
