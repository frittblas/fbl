/*
*
*	Ecs
*
*	LaserSystem.hpp
*
*	The laser system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;

class LaserSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

	void dealDamage(Game& g, Entity attacker, Entity target);
	void setDirection(Position& pos, Laser& las);
};
