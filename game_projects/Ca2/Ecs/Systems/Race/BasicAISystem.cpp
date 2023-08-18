/*
*
*	Ecs
*
*	BasicAISystem.cpp
*
*	Basic ai system system for the non player robots.
*
*	Just simple stuff like healing when under 50% health etc.
*
*	Hans Strömquist 2023
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"

#include "BasicAISystem.hpp"

void BasicAISystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		auto& turbo = ecs.GetComponent<BasicAI>(entity);



	}

	std::cout << "BasicAI component system initialized." << std::endl;


}

void BasicAISystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{



	}

}
