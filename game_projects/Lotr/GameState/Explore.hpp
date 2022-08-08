/*
*
*	Lotr
*
*	Explore.hpp
*
*	Explore class header, takes care of the Explore game state.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IState.hpp"

class Explore : public IState{

public:
    Explore();
    ~Explore();

    virtual void tick();

};
