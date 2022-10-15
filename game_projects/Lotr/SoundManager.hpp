/*
*
*	Lotr
*
*	SoundManager.hpp
*
*	SoundManager class header, takes care of all sound in the game, sfx, ambience, music.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

class SoundManager {

public:

    SoundManager();
    ~SoundManager();

    void loadSfx();
    void loadAndPlayMusic(std::string filename);

};
