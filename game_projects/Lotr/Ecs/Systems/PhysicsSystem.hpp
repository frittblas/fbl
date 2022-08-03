/*
*
*	Ecs
*
*	PhysicsSystem.hpp
*
*	The physics system header.
*
*	Hans Str�mquist 2022
*
*/
#pragma once

#include "../System.hpp"

class PhysicsSystem : public System
{
public:
	void Init();
	void Update();
};
