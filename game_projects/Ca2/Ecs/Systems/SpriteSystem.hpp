/*
*
*	Ecs
*
*	SpriteSystem.hpp
*
*	The sprite system header.
*
*	Hans Str�mquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class SpriteSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
