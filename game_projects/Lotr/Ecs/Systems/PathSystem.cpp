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

#include "PathSystem.hpp"

void PathSystem::Init(Coordinator& ecs) {

	int nextAvailableId = 0;

	// assign the path id, set params
	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);

		path.id = nextAvailableId++;	// set id, then increment


		// set the goal to be the current position
		//path.goalX = pos.x;
		//path.goalY = pos.y;

		//path.newPath = false;

		std::cout << "Path id: " << (int)path.id << std::endl;

	}

	std::cout << "Path component system initialized." << std::endl;


}

void PathSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);
		auto& spr = ecs.GetComponent<Sprite>(entity);

		// find path if newPath is true (meaning a new path has been requested)
		if (path.newPath) {

			fbl_pathf_set_path_status(path.id, fbl_pathf_find_path(path.id, pos.x, pos.y, path.goalX, path.goalY, FBL_PATHF_USE_DIAG));
			path.newPath = false;

			if(fbl_pathf_get_path_status(path.id) == FBL_PATHF_FOUND)
				std::cout << "New path for id: " << (int)path.id << std::endl;	// need to cast path.id bc of auto& (Path&)

		}

		// do the pathfinding only if we haven't reached the goal
		if (fbl_pathf_get_path_status(path.id) == FBL_PATHF_FOUND) {

			fbl_pathf_read_path(path.id, pos.x, pos.y, 1);

			if (pos.x > fbl_pathf_get_x_path(path.id)) {
				pos.x -= 2;
				spr.dir = Dir::Left;
			}
			if (pos.x < fbl_pathf_get_x_path(path.id)) {
				pos.x += 2;
				spr.dir = Dir::Right;
			}
			if (pos.y > fbl_pathf_get_y_path(path.id)) {
				pos.y -= 2;
				spr.dir = Dir::Up;
			}
			if (pos.y < fbl_pathf_get_y_path(path.id)) {
				pos.y += 2;
				spr.dir = Dir::Down;
			}

			//std::cout << "pathing!" << std::endl;

		}

	}

}
