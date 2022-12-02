/*
*
*	Ecs
*
*	PathSystem.cpp
*
*	The path system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "../../Game.hpp"
#include "../../Location.hpp"

#include "PathSystem.hpp"

void PathSystem::Init(Coordinator& ecs) {

	int nextAvailableId = 0;

	// assign the path id, set params
	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);

		path.id = nextAvailableId++;	// set id, then increment

		fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);

		std::cout << "Path id: " << (int)path.id << std::endl;

	}

	std::cout << "Path component system initialized." << std::endl;


}

void PathSystem::Update(Game& g) {


	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& path = g.mEcs->GetComponent<Path>(entity);
		auto& spr = g.mEcs->GetComponent<Sprite>(entity);

		// find path if newPath is true (meaning a new path has been requested)
		if (path.newPath) {

			fbl_pathf_set_path_status(path.id, fbl_pathf_find_path(path.id, pos.x, pos.y, path.goalX, path.goalY, path.diag));
			path.newPath = false;

			if(fbl_pathf_get_path_status(path.id) == FBL_PATHF_FOUND)
				std::cout << "New path for id: " << (int)path.id << std::endl;	// need to cast path.id bc of auto& (Path&)

		}

		// do the pathfinding only if we haven't reached the goal
		if (fbl_pathf_get_path_status(path.id) == FBL_PATHF_FOUND) {


			fbl_pathf_read_path(path.id, pos.x, pos.y, path.pixelsPerFrame);


			if (pos.x > fbl_pathf_get_x_path(path.id)) {
				pos.x -= path.speed;
				spr.dir = Dir::Left;
			}
			if (pos.x < fbl_pathf_get_x_path(path.id)) {
				pos.x += path.speed;
				spr.dir = Dir::Right;
			}
			if (pos.y > fbl_pathf_get_y_path(path.id)) {
				pos.y -= path.speed;
				spr.dir = Dir::Up;
			}
			if (pos.y < fbl_pathf_get_y_path(path.id)) {
				pos.y += path.speed;
				spr.dir = Dir::Down;
			}

			//std::cout << "pathing!" << std::endl;


		}

	}

}
