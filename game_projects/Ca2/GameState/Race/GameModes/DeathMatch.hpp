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

	void handleTargets(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	int  getTargetSpriteId(Entity e);
	void snapToGrid(uint16_t& x, uint16_t& y);
	void handleCoins(Entity e, Sprite& spr, PathLogic& plog);
	void handleBases(Game& g, Entity e, Position& pos, Sprite& spr, Path& path, PathLogic& plog);
	void findClosestTarget(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void checkWinCondition(Game& g);
	void switchCtrl(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);
	void updatePaths(Game& g, Entity e, Position& pos, Path& path, PathLogic& plog);

private:
	std::unordered_map<Entity, uint16_t> mEntityToSpriteIdMap; // map entity to spriteId

};
