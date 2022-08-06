/*
*
*	Ecs
*
*	PhysicsSystem.cpp
*
*	The physics system.
*
*
*	Hans Str�mquist 2022
*
*/

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "PhysicsSystem.hpp"

extern Coordinator gEcs;

void PhysicsSystem::Init() {
	std::cout << "lol Init" << std::endl;
}

void PhysicsSystem::Update() {
	for (auto const& entity : mEntities)
	{
		auto& pos = gEcs.GetComponent<Position>(entity);
		//auto& transform = gCoordinator.GetComponent<Transform>(entity);

		pos.x++;

	}
}