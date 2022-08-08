/*
*
*	Lotr
*
*	UserInput.hpp
*
*	Input class header, takes care of all the user input.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "Game.hpp"
#include "GameState/GameState.hpp"

class UserInput {

public:
    UserInput();
    ~UserInput();

    void tick(Game& g, GameState& gs);
};
