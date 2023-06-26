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
	void handleFlags(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	int  hasFlag(Entity e);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void handleBases(Entity e, Position& pos, Sprite& spr, Path & path, PathLogic& plog);

};
