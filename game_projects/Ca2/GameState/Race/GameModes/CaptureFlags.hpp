/*
*
*
*	CaptureFlags.hpp
*
*	The capture flags game mode header.
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

class CaptureFlags
{
public:

	void handleFlags(Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	int  hasFlag(Entity e);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	void findClosestFlag(Position& pos, Path& path, PathLogic& plog);
	void checkWinCondition(Game& g);
	void switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);

};
