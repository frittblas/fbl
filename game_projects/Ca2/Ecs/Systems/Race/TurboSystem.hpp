/*
*
*	Ecs
*
*	TurboSystem.hpp
*
*	The turbo system header.
*
*	Hans Str�mquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class TurboSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
