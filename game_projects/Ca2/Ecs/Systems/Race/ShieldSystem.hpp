/*
*
*	Ecs
*
*	ShieldSystem.hpp
*
*	The shield system header.
*
*	Hans Str�mquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;

class ShieldSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

};
