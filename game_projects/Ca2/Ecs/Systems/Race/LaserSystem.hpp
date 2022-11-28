/*
*
*	Ecs
*
*	LaserSystem.hpp
*
*	The laser system header.
*
*	Hans Str�mquist 2022
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class LaserSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};