/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Dialogue.hpp
*
*	Dialogue class header, takes care of the Dialogue game state.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IState.hpp"

class Game;

class Dialogue : public IState{

public:
    Dialogue();
    ~Dialogue();

    virtual void tick(Game& g) override;

};
