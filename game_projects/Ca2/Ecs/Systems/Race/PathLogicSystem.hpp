/*
*
*	Ecs
*
*	PathLogicSystem.hpp
*
*	The path logic system header. Deals with
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
	void handleFlags(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	int  hasFlag(Entity e);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path & path, PathLogic& plog);
	void findClosestFlag(Position& pos, Path& path, PathLogic& plog);
	void switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);

};
