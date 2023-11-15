/*
*
*
*	DeathMatch.hpp
*
*	The death match game mode header.
*
*	Hans Strömquist 2023
*
*/

#pragma once

#include "../../../Ecs/System.hpp"

class Coordinator;
class Game;

struct Position;
struct Sprite;
struct Path;
struct PathLogic;

class DeathMatch
{
public:

	void handleTargets(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	int  hasTarget(Entity e);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	void findClosestTarget(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void checkWinCondition(Game& g);
	void switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);

};
