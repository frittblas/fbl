/*
*
*	Ecs
*
*	LightSystem.hpp
*
*	The light system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class LightSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
