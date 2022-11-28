/*
*
*	Ecs
*
*	LaserSystem.cpp
*
*	The laser system. It's raycasting and possibly particle efx at the end.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "LaserSystem.hpp"

void LaserSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);


			
	}

	std::cout << "Laser component system initialized!" << std::endl;
	
}

void LaserSystem::Update(Coordinator& ecs) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);


	}

}
