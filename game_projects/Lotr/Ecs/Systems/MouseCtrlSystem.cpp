/*
*
*	Ecs
*
*	MouseCtrlSystem.cpp
*
*	The mouse controller system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "MouseCtrlSystem.hpp"

void MouseCtrlSystem::Init(Coordinator& ecs) {


	for (auto const& entity : mEntities)
	{

		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);


	}

	std::cout << "MouseCtrl component system initialized." << std::endl;


}

void MouseCtrlSystem::Update(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& path = ecs.GetComponent<Path>(entity);


	}

}
