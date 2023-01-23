/*
*
*	Ecs
*
*	AutoAim.hpp
*
*	The auto aim system header.
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
