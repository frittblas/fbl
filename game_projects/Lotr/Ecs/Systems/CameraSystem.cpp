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

		// lerped using the above function
		//cam.x = _lerp(pos.x - Game::LogicalResW / 2, cam.x, 0.05);
		//cam.y = _lerp(pos.y - Game::LogicalResH / 2, cam.y, 0.05);

		//vActual = vDesired * factor + vActual * (1.0 - factor);

		// camera trap, or deadzone
		//if(pos.x < ((Game::LogicalResW / 2) - 100) - fbl_get_camera_x() ||

		// "inline" lerp
		cam.x = (pos.x - Game::LogicalResW / 2) * 0.05 + cam.x * (1.0 - 0.05);
		cam.y = (pos.y - Game::LogicalResH / 2) * 0.05 + cam.y * (1.0 - 0.05);


		// limit camera xy
		if (cam.x < 0) cam.x = 0;
		if (cam.y < 0) cam.y = 0;
		if (cam.x > Game::LogicalResW) cam.x = Game::LogicalResW;
		if (cam.y > Game::LogicalResH) cam.y = Game::LogicalResH;

		fbl_set_camera_xy(cam.x, cam.y);

	}

}
