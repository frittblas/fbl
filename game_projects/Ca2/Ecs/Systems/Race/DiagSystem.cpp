/*
*
*	Ecs
*
*	DiagSystem.cpp
*
*	The robot controller system for diagonals.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../../src/fbl.hpp"

#include "../../Ecs.hpp"
#include "../../Components.hpp"

#include "../../../Game.hpp"

#include "DiagSystem.hpp"


void DiagSystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		auto& diag = ecs.GetComponent<Diag>(entity);


	}

	std::cout << "Diagonal component system initialized." << std::endl;


}

void DiagSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{

		auto& path = ecs.GetComponent<Path>(entity);
		auto& diag = ecs.GetComponent<Diag>(entity);

		if (diag.active) {

			path.diag = true;

			if (diag.justSwitched) {
				path.newPath = true;
				std::cout << "Just switched diagonal!" << std::endl;
				diag.justSwitched = false;
			}

		}
		else {

			path.diag = false;

			if (diag.justSwitched) {
				path.newPath = true;
				std::cout << "Just switched diagonal!" << std::endl;
				diag.justSwitched = false;
			}

		}

	}

}
