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

class Game;

class Race : public IState{

public:
    Race();
    ~Race();

    virtual void tick(Game& g) override;

};
