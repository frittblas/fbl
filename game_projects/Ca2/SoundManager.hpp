/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	SoundManager.hpp
*
*	SoundManager class header, takes care of all sound in the game, sfx, ambience, music.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IObserver.hpp"
#include <iostream>

class SoundManager : public IObserver {

public:

    ~SoundManager();

    static SoundManager& getInstance();

    void loadSfx();
    void loadAndPlayMusic(std::string filename, int volume);

    void onRobotDeath() override;
    void onFireLaser() override;

private:

    static SoundManager instance;
    SoundManager();

    uint16_t mSfxLaser1, mSfxLaser2, mSfxLaser3;
    uint16_t mSfxMagnet, mSfxHealing, mSfxTurbo, mShield;
    uint16_t mSfxExplosion;
    uint16_t mSfxRainstorm, mSfxSummer;

};
