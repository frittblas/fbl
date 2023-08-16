/*
*
*	Ecs
*
*	TurboSystem.cpp
*
*	The turbo system.
*
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"

#include "TurboSystem.hpp"

extern bool gStartingOut;

void TurboSystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		auto& turbo = ecs.GetComponent<Turbo>(entity);



	}

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

			//std::cout << "Speed: " << path.speed << std::endl;

			sta.energy -= static_cast<double>(turbo.energyCost) / 20;

		}
		else path.speed = (float)sta.speed / 10;

	}

}
