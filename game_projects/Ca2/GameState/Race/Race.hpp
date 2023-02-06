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

class Race : public IState{

public:
    Race();
    ~Race();

    void assignRobots(Game& g);
    void unassignRobots(Game& g);
    void shakeScreen(int intensity, int duration);
    void tickShake();

    virtual void tick(Game& g) override;

private:
    Maze *mMaze;
    uint8_t mNumRacers;

    uint16_t shakeDuration = 0;
    uint16_t shakeIntensity = 0;

};
