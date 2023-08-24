/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Progress.hpp
*
*	Progress class header, takes care of all game progress, how far the player has come,
*   current location, quests, coins etc. Also save and load game.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

class Progress {

public:
    Progress();
    ~Progress();

    uint16_t mFunds;            // total amount of coins
    uint8_t  mCurrentLocation;  // current location

};
