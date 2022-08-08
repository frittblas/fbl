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

class Game;

class IState {

public:
    virtual ~IState() = default; // virtual destructor, so the destructor gets called of the derived classes
    virtual void tick(Game& g) = 0;

};
