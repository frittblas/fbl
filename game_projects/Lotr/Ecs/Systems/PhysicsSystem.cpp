/*
*
*	Ecs
*
*	PhysicsSystem.cpp
*
*	The physics system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "PhysicsSystem.hpp"

void PhysicsSystem::Init(Coordinator& ecs) {
	std::cout << "lol Init" << std::endl;
}

void PhysicsSystem::Update(Coordinator& ecs) {
	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		//auto& transform = gCoordinator.GetComponent<Transform>(entity);

		pos.x++;

	}
}
