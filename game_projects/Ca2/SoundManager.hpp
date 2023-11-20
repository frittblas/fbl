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

    enum Channel {

        Ambient,
        Ui,
        Expl,
        Heal,
        Laser,
        Magnet,
        Shield,
        Turbo

    };

    static SoundManager& getInstance();

    void loadSfx();
    void playSfx(int id, int channel, int loops);
    void loadAndPlayMusic(std::string filename, int volume);

    void onRobotDeath() override;
    void onFireLaser() override;

    uint16_t mSfxLaser1, mSfxLaser2, mSfxLaser3;
    uint16_t mSfxMagnet, mSfxHeal, mSfxTurbo, mSfxShield;
    uint16_t mSfxExplosion, mSfxCoin, mSfxPowerup;
    uint16_t mSfxPass, mSfxDenied, mSfxSnap, mSfxTick, mSfxQuit;
    uint16_t mSfxRainStorm, mSfxSummer;

private:

    static SoundManager instance;
    SoundManager();

};
