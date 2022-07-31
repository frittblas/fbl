/*
*
*	Ecs
*
*	System.hpp
*
*	System base class.
*
*	I got this from https://austinmorlan.com/posts/entity_component_system/
*	Rearanged it a bit for Lotr.
*
*/

#pragma once

#include <set>

class System
{
public:
	std::set<Entity> mEntities;
};
