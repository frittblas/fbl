/*
*
*	Ecs
*
*	HealSystem.hpp
*
*	The heal system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;

class HealSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

};
