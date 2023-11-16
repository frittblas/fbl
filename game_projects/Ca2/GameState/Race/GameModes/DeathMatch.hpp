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
	DeathMatch();
	~DeathMatch();

	enum DMStates {

		GoingToBase		 = -2,
		CheckPointBase	 = -1

	};

	void handleTargets(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void goToBase(Game& g, Entity e, Path& path, PathLogic& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	void findClosestTarget(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void checkWinCondition(Game& g);
	void switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void printPlayerMsg(std::string msg, Entity e);
	void snapToGrid(uint16_t& x, uint16_t& y);
	int  distance(int px, int py, int cx, int cy);

};
