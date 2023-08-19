/*
*
*	Ecs
*
*	PathLogicSystem.hpp
*
*	The path logic system header. Deals with game modes.
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
	void tickCF(Game& g);
	void tickDM(Game& g);

};
