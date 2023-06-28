/*
*
*	Ecs
*
*	MagnetSystem.hpp
*
*	The magnet system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;

class MagnetSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);
	int  distance(int px, int py, int cx, int cy);

};
