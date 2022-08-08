/*
*
*	Ecs
*
*	PhysicsSystem.hpp
*
*	The physics system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class PhysicsSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
