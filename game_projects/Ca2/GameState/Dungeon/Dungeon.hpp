/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Dungeon.hpp
*
*	Dungeon class header, takes care of the Dungeon game state.
*
*	Hans Strömquist 2024
*
*/

#pragma once

#include "../IState.hpp"

class Game;

class Dungeon : public IState{

public:
    Dungeon();
    ~Dungeon();

    void processInput(Game& g);

    virtual void tick(Game& g) override;

};
