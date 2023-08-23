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

		auto& sta = ecs.GetComponent<Stats>(entity);
		auto& ai  = ecs.GetComponent<BasicAI>(entity);

		ai.action = Actions::None;
		ai.durationLeft = 0;
		ai.hpLastFrame = sta.hp;

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
						if (ai.durationLeft > 1) heal.activated = true;
						else heal.activated = false;
					}
					break;
				case Actions::Shielding:
					{
						auto& shield = ecs.GetComponent<Shield>(entity);
						if (ai.durationLeft > 1) shield.isShielding = true;
						else shield.isShielding = false;
					}
					break;
				case Actions::Turboing:
					{
						auto& turbo = ecs.GetComponent<Turbo>(entity);
						if (ai.durationLeft > 1) turbo.activated = true;
						else turbo.activated = false;
					}
					break;


			}

			ai.durationLeft--;
			if (ai.durationLeft == 0) ai.action = Actions::None;

		}


		// assign new actions based on some basic conditions
		if (ai.durationLeft == 0) {

			// check if the entity has any of the following components then do stuff

			// if hp is < 50%
			if (sta.hp < (sta.maxHp / 2)) {

				if (ecs.HasComponent<Heal>(entity)) {
					ai.action = Actions::Healing;
					ai.durationLeft = rand() % 10 + 15;
				}

			}


			// see if the robot has take damage since last frame
			if (sta.hp < ai.hpLastFrame) {

				std::cout << "Robot shot!!!" << std::endl;

				if (ecs.HasComponent<Shield>(entity)) {
				
					ai.action = Actions::Shielding;
					ai.durationLeft = rand() % 10 + 10;
				
				}

			}

			// randomly use turbo if robot has > 50% charge
			if (rand() % 150 == 0) {

				if (sta.energy > (sta.maxEnergy / 2)) {

					std::cout << "Over 50% energy!" << std::endl;

					if (ecs.HasComponent<Turbo>(entity)) {

						ai.action = Actions::Turboing;
						ai.durationLeft = rand() % 20 + 40;	// boost for a second ish

					}

				}

			}

		}

		/*
		if (rand() % 100 == 0) {
			std::cout << "hp : " << sta.hp << " lastF: " << ai.hpLastFrame << " Entity: " << entity << std::endl;
			std::cout << "durLeft : " << (int)ai.durationLeft << std::endl;
		}
		*/

		ai.hpLastFrame = sta.hp;

	}

}
