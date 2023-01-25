/*
*
*	Ecs
*
*	AutoAim.hpp
*
*	The auto aim system header. Raycasting an invisible ray in all directions (one at a time) looking for targets.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;
struct Position;
struct AutoAim;

class AutoAimSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

	void setDirection(Position& pos, AutoAim& aim);
};
