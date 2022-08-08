/*
*
*	Lotr
*
*	Progress.hpp
*
*	Progress class header, takes care of all game progress, how far the player has come, levels, quests.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

class Progress {

public:
    uint32_t room;
    uint32_t level;

    Progress();
    ~Progress();

};
