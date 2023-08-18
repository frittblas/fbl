/*
*
*	Ecs
*
*	DeathMatchSystem.hpp
*
*	The death match system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../../System.hpp"

class Coordinator;
class Game;

class DeathMatchSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);
	void handleCoins(Entity e, Sprite& spr, DeathMatch& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path & path, DeathMatch& plog);
	void updatePaths(Game& g);
	void switchCtrl(Game& g);

};
