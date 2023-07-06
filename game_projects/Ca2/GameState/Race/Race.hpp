/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Race.hpp
*
*	Race class header, takes care of the Race game state.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "../IState.hpp"
#include "Maze.hpp"

class Game;
struct Addon;

class Race : public IState{

public:
    Race();
    ~Race();

    void assignRobots(Game& g);
    void unassignRobots(Game& g);
    void getInput(Game& g);
    void handleAddons(Game& g, Addon& add, Entity playingRobot, bool on);

    virtual void tick(Game& g) override;

private:
    Maze *mMaze;
    uint8_t mNumRacers;

};
