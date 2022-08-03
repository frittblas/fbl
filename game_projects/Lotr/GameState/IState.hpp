/*
*
*	Lotr
*
*	IState.hpp
*
*	State interface, all the game states inherit from this.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

class IState {

public:
    virtual ~IState() {} // virtual destructor, calls the destructor of the derived classes
    virtual void tick() = 0;

};
