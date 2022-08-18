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

		int id = fbl_create_prim(FBL_RAY + 1, cam.trapX, cam.trapY, cam.trapW, cam.trapH, 0, false, false);
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

		// camera trap (or deadzone) + lerp
		if (pos.x < ((Game::LogicalResW / 2) - Game::TileSize * 3) + fbl_get_camera_x()) {
			cam.x = ((pos.x - Game::LogicalResW / 2) + Game::TileSize * 3) * cam.damp + cam.x * (1.0 - cam.damp);
			std::cout << "Pushing camera bounds left!" << std::endl;
		}
		
		if (pos.x > ((Game::LogicalResW / 2) + Game::TileSize * 3) + fbl_get_camera_x()) {
			cam.x = ((pos.x - Game::LogicalResW / 2) - Game::TileSize * 3) * cam.damp + cam.x * (1.0 - cam.damp);
			std::cout << "Pushing camera bounds right!" << std::endl;
		}
			
		if (pos.y < ((Game::LogicalResH / 2) - Game::TileSize * 3) + fbl_get_camera_y()) {
			cam.y = ((pos.y - Game::LogicalResH / 2) + Game::TileSize * 3) * cam.damp + cam.y * (1.0 - cam.damp);
			std::cout << "Pushing camera bounds up!" << std::endl;
		}

		if (pos.y > ((Game::LogicalResH / 2) + Game::TileSize * 3) + fbl_get_camera_y()) {
			cam.y = ((pos.y - Game::LogicalResH / 2) - Game::TileSize * 3) * cam.damp + cam.y * (1.0 - cam.damp);
			std::cout << "Pushing camera bounds down!" << std::endl;
		}
			
		//std::cout << "Pushing camera bounds!" << std::endl;


		// limit camera xy
		if (cam.x < 0) cam.x = 0;
		if (cam.y < 0) cam.y = 0;
		if (cam.x > Game::LogicalResW) cam.x = Game::LogicalResW;
		if (cam.y > Game::LogicalResH) cam.y = Game::LogicalResH;

		// finally set the fbl camera
		fbl_set_camera_xy(cam.x, cam.y);

	}

}
