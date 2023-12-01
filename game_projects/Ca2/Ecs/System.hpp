/*
*
*	Ecs
*
*	System.hpp
*
*	System base class.
*
*	I got this from https://austinmorlan.com/posts/entity_component_system/
*	Rearranged it a bit for Ca2.
*
*/

#pragma once

#include <set>
#include "Types.hpp"

class System
{
public:
	std::set<Entity> mEntities;
};
