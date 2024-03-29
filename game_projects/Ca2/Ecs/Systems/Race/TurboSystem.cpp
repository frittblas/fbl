/*
*
*	Ecs
*
*	TurboSystem.cpp
*
*	The turbo system.
*
*
*	Hans Str�mquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"

#include "TurboSystem.hpp"

void TurboSystem::Init(Coordinator& ecs) {

	std::cout << "Turbo component system initialized." << std::endl;

}

void TurboSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& path = ecs.GetComponent<Path>(entity);
		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& turbo = ecs.GetComponent<Turbo>(entity);

		if (turbo.activated && sta.energy > 0) {

			float oldSpeed = (float)sta.speed / 10;
			path.speed = oldSpeed * turbo.amount;

			sta.energy -= static_cast<double>(turbo.energyCost) / 15;

		}
		else path.speed = (float)sta.speed / 10;

	}

}
