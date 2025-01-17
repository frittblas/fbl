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

	uint16_t mMaxHp;            // max hp for the player
	uint16_t mHp;               // current hp for the player
    uint16_t mFunds;            // total amount of coins
    uint16_t mFavRobot;         // the selected robot in the menu (racing robot)
    uint8_t  mCurrentLocation;  // current location
    uint16_t mCompletedRaces;   // how many races have been completed in total
    uint16_t mCompletedMaint;   // how many maintenance runs have been completed in total
	uint16_t mCompletedDungeons;// how many dungeons have been completed in total

    void resetProgress();       // reset funds and completed races etc.

};
