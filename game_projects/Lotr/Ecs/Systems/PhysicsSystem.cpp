/*
*
*	Ecs
*
*	PhysicsSystem.cpp
*
*	The physics system.
*
*	I got this from https://austinmorlan.com/posts/entity_component_system/
*	Rearanged it a bit for Lotr.
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
