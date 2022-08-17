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

		auto& mCtrl = ecs.GetComponent<MouseCtrl>(entity);
		mCtrl.clicked = false;

	}

	std::cout << "MouseCtrl component system initialized." << std::endl;


}

void MouseCtrlSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& path = ecs.GetComponent<Path>(entity);
		auto& mCtrl = ecs.GetComponent<MouseCtrl>(entity);

		if (fbl_get_mouse_click(FBLMB_LEFT)) {

			mCtrl.clicked = true;

		}

		if (fbl_get_mouse_release(FBLMB_LEFT) && mCtrl.clicked) {

			path.goalX = fbl_get_mouse_logical_x() + fbl_get_camera_x();
			path.goalY = fbl_get_mouse_logical_y() + fbl_get_camera_y();

			// snap to grid
			while (path.goalX % Game::TileSize != 0) path.goalX--;
			while (path.goalY % Game::TileSize != 0) path.goalY--;

			path.newPath = true;

			mCtrl.clicked = false;

		}


	}

}
