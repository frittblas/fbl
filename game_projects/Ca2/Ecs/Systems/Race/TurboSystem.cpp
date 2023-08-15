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

		auto& rCtrl = ecs.GetComponent<Turbo>(entity);

	}

	std::cout << "Turbo component system initialized." << std::endl;


}

void TurboSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);
		auto& rCtrl = ecs.GetComponent<Turbo>(entity);


	}

}
