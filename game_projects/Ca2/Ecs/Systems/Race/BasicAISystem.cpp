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

		auto& ai = ecs.GetComponent<BasicAI>(entity);

		ai.action = Actions::None;
		ai.duration = 20;
		ai.durationLeft = 0;

	}

	std::cout << "BasicAI component system initialized." << std::endl;


}

void BasicAISystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& ai  = ecs.GetComponent<BasicAI>(entity);

		// first check if the robot got duration on any action left and carry that action out

		if (ai.durationLeft > 0) {

			switch (ai.action) {

				case Actions::Healing:
					{
						auto& heal = ecs.GetComponent<Heal>(entity);
						heal.activated = true;
					}
					break;
				case Actions::Shielding:
					break;
				case Actions::Turbo:
					break;


			}

			ai.durationLeft--;

		}


		// assign new actions based on some basic conditions
		if (ai.durationLeft == 0) {

			// check if the entity has any of the following components then do stuff

			if (ecs.HasComponent<Heal>(entity)) {

				auto& heal = ecs.GetComponent<Heal>(entity);

				// if hp is < 50%
				if (sta.hp < (sta.maxHp / 2)) {
					ai.action = Actions::Healing;
					ai.duration = rand() % 10 + 15;
					ai.durationLeft = ai.duration;
				}

			}

		}

	}

}
