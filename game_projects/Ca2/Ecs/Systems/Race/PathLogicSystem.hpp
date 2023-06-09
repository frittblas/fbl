/*
*
*	Ecs
*
*	PathLogicSystem.hpp
*
*	The path logic system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;
class Game;

class PathLogicSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);

};
