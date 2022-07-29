#pragma once

#include "../Ecs.hpp"
#include "../Components.hpp"
#include "../System.hpp"

extern Coordinator gEcs;

class PhysicsSystem : public System
{

public:
	void Init() {
	}

	void Update() {
		for (auto const& entity : mEntities)
		{
			auto& pos = gEcs.GetComponent<Position>(entity);
			//auto& transform = gCoordinator.GetComponent<Transform>(entity);

			pos.x++;

		}
	}

};
