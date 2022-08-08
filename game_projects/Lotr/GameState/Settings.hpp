/*
*
*	Lotr
*
*	Settings.hpp
*
*	Settings class header, takes care of the settings screen.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IState.hpp"

class Game;

class Settings : public IState {

public:
    Settings();
    ~Settings();

    virtual void tick(Game& g) override;
};
