/*
*
*	Ecs
*
*	BasicAISystem.hpp
*
*	Basic ai system system for the non player robots.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class BasicAISystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
