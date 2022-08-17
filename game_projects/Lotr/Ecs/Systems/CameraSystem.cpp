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

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "../../Game.hpp"

#include "CameraSystem.hpp"

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

		cam.x = pos.x - Game::LogicalResW / 2;
		cam.y = pos.y - Game::LogicalResH / 2;

		fbl_set_camera_xy(cam.x, cam.y);

	}

}
