/*
*
*	Ecs
*
*	RobotCtrlSystem.cpp
*
*	The robot controller system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"
#include "../../../../Ca2/GameState/Race/Maze.hpp"

#include "RobotCtrlSystem.hpp"

void RobotCtrlSystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		auto& rCtrl = ecs.GetComponent<RobotCtrl>(entity);
		rCtrl.access = 120;
		rCtrl.justSwitched = false;

	}

	std::cout << "RobotCtrl component system initialized." << std::endl;


}

void RobotCtrlSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);
		auto& rCtrl = ecs.GetComponent<RobotCtrl>(entity);

		if (rCtrl.active) {

			if (fbl_get_mouse_click(FBLMB_LEFT) && rCtrl.access == 0 && !Maze::sStartingOut) {

				path.goalX = fbl_get_mouse_logical_x() + fbl_get_camera_x();
				path.goalY = fbl_get_mouse_logical_y() + fbl_get_camera_y();

				// snap to grid
				while (path.goalX % Game::TileSize != 0) path.goalX--;
				while (path.goalY % Game::TileSize != 0) path.goalY--;

				// restrict clicking area to only the actual racing area (and not where the addons are)

				if (path.goalX > (Game::TileSize * 3 - 2) && path.goalX < Game::LogicalResW - (Game::TileSize * 3)) {

					if (!(pos.x == path.goalX && pos.y == path.goalY))	// can't click on yourself, doesn't count
						path.newPath = true;

					rCtrl.access = 20;

				}

			}

		}

		rCtrl.access--;
		if (rCtrl.access < 0) rCtrl.access = 0;


	}

}
