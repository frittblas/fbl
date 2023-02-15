/*
*
*	Ecs
*
*	MouseCtrlSystem.cpp
*
*	The mouse controller system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "../../Game.hpp"

#include "MouseCtrlSystem.hpp"

void MouseCtrlSystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		// We dont' need this (already initialized when adding component)
		
		//auto& mCtrl = ecs.GetComponent<MouseCtrl>(entity);
		//mCtrl.access = 30;

	}

	std::cout << "MouseCtrl component system initialized." << std::endl;


}

void MouseCtrlSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);
		auto& mCtrl = ecs.GetComponent<MouseCtrl>(entity);

		if (fbl_get_mouse_click(FBLMB_LEFT) && mCtrl.access == 0) {

			path.goalX = fbl_get_mouse_logical_x() + fbl_get_camera_x();
			path.goalY = fbl_get_mouse_logical_y() + fbl_get_camera_y();

			// snap to grid
			while (path.goalX % Game::TileSize != 0) path.goalX--;
			while (path.goalY % Game::TileSize != 0) path.goalY--;

			if(!(pos.x == path.goalX && pos.y == path.goalY))	// can't click on yourself, doesn't count
				path.newPath = true;

			mCtrl.access = 20;

		}

		mCtrl.access--;
		if (mCtrl.access < 0) mCtrl.access = 0;


	}

}
