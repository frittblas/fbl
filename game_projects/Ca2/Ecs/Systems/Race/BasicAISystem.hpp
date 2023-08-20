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

    enum Actions {
        None,
        Healing,
        Shielding,
        Turbo
    };

	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
