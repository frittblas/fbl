/*
*
*	Lotr
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

class Dialogue : public IState{

public:
    Dialogue();
    ~Dialogue();

    virtual void tick();

};
