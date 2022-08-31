/*
*
*	Ecs
*
*	CameraSystem.cpp
*
*	The camera system.
*
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

#include "../../../../src/fbl.hpp"
#include "../Ecs.hpp"
#include "../Components.hpp"
#include "../../Game.hpp"
#include "CameraSystem.hpp"

/*
float _lerp(float a, float b, float amount)
{
	return (a * amount) + (b * (1 - amount));
}
*/

void CameraSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& cam = ecs.GetComponent<Camera>(entity);

		// Debug Rect to visualize the deadzone
		// find x,y,w,h
		int x = (Game::LogicalResW / 2) - Game::TileSize * cam.deadZoneW;
		int y = (Game::LogicalResH / 2) - Game::TileSize * cam.deadZoneH;
		int w = (cam.deadZoneW * Game::TileSize * 2) + (1 * Game::TileSize);	// width is 2 times deadZoneW + 1 * tilesize (for the player in the middle)
		int h = (cam.deadZoneH * Game::TileSize * 2) + (1 * Game::TileSize);	// same

		int id = fbl_create_prim(FBL_RAY + 1, x, y, w, h, 0, false, false); // create debug rect
		fbl_set_prim_color(id, 255, 255, 255, 50);
		fbl_fix_prim_to_screen(id, true);

	}

	std::cout << "Camera component system initialized!" << std::endl;

}

void CameraSystem::Update(Coordinator& ecs) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& cam = ecs.GetComponent<Camera>(entity);

		// instant but stiff camera
		//cam.x = pos.x - Game::LogicalResW / 2;
		//cam.y = pos.y - Game::LogicalResH / 2;

		// lerped camera using the above _lerp function
		//cam.x = _lerp(pos.x - Game::LogicalResW / 2, cam.x, 0.05);
		//cam.y = _lerp(pos.y - Game::LogicalResH / 2, cam.y, 0.05);

		// lerp formula
		//vActual = vDesired * factor + vActual * (1.0 - factor);

		// "inline" lerp
		//cam.x = (pos.x - Game::LogicalResW / 2) * 0.05 + cam.x * (1.0 - 0.05);
		//cam.y = (pos.y - Game::LogicalResH / 2) * 0.05 + cam.y * (1.0 - 0.05);

		// camera trap (or deadzone) + lerp (subtract and add 1 pixel att the end bc otherwise always true)
		if (pos.x < ((Game::LogicalResW / 2) - Game::TileSize * cam.deadZoneW) + fbl_get_camera_x() - 1) {
			cam.x = ((pos.x - Game::LogicalResW / 2) + Game::TileSize * cam.deadZoneW) * cam.damp + cam.x * (1.0 - cam.damp);
			//std::cout << "Pushing camera bounds left!" << std::endl;
		}
		
		if (pos.x > ((Game::LogicalResW / 2) + Game::TileSize * cam.deadZoneW) + fbl_get_camera_x() + 1) {
			cam.x = ((pos.x - Game::LogicalResW / 2) - Game::TileSize * cam.deadZoneW) * cam.damp + cam.x * (1.0 - cam.damp);
			//std::cout << "Pushing camera bounds right!" << std::endl;
		}
			
		if (pos.y < ((Game::LogicalResH / 2) - Game::TileSize * cam.deadZoneH) + fbl_get_camera_y() - 1) {
			cam.y = ((pos.y - Game::LogicalResH / 2) + Game::TileSize * cam.deadZoneH) * cam.damp + cam.y * (1.0 - cam.damp);
			//std::cout << "Pushing camera bounds up!" << std::endl;
		}

		if (pos.y > ((Game::LogicalResH / 2) + Game::TileSize * cam.deadZoneH) + fbl_get_camera_y() + 1) {
			cam.y = ((pos.y - Game::LogicalResH / 2) - Game::TileSize * cam.deadZoneH) * cam.damp + cam.y * (1.0 - cam.damp);
			//std::cout << "Pushing camera bounds down!" << std::endl;
		}
			
		// limit camera xy
		if (cam.x < 0) cam.x = 0;
		if (cam.y < 0) cam.y = 0;
		if (cam.x > Game::LogicalResW) cam.x = Game::LogicalResW;
		if (cam.y > Game::LogicalResH) cam.y = Game::LogicalResH;

		// finally set the fbl camera
		fbl_set_camera_xy(cam.x, cam.y);

	}

}
