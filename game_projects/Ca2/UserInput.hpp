/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	UserInput.hpp
*
*	Input class header, takes care of all the user input.
*
*	Hans Strömquist 2022
*
*/

#pragma once

class Game;

class UserInput {

public:

    const int buttonDelay = 30;
    int access;

    UserInput();
    ~UserInput();

    void tick(Game& g);

private:
    bool isFullscreen;

};
