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
#include "Types.hpp"	// for the "using" Entity typedef (this include wasn't needed before, because of include order?

class System
{
public:
	std::set<Entity> mEntities;
};
