/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	SoundManager.cpp
*
*	SoundManager class implemetation, takes care of all sound in the game, sfx, ambience, music.
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"

#include "SoundManager.hpp"

// SoundManager-class implementation

SoundManager::SoundManager() {

	//loadSfx();
	//fbl_play_sound(1, -1);
	//loadAndPlayMusic("music/harp.ogg");

	std::cout << "Initialized SoundManager subsystem." << std::endl;

}

SoundManager::~SoundManager() {

	std::cout << "Destroyed SoundManager subsystem." << std::endl;

}

SoundManager SoundManager::instance;

SoundManager& SoundManager::getInstance() {

	return instance;

}

void SoundManager::loadSfx() {

	// load all the sound effects in the game and assign id's
	mSfxLaser1 = fbl_load_sound("sfx/laser1.ogg");
	mSfxExplosion = fbl_load_sound("sfx/explosion1.ogg");
	fbl_set_sound_volume(mSfxExplosion, 40); // max is 128

	mSfxRainstorm = fbl_load_sound("sfx/rain.ogg");
	mSfxSummer = fbl_load_sound("sfx/summer.ogg");

	fbl_play_sound(mSfxSummer, 0, -1);
	loadAndPlayMusic("music/title.ogg");

}

void SoundManager::loadAndPlayMusic(std::string filename) {

	fbl_destroy_music();
	fbl_load_music(filename.c_str());
	fbl_play_music(-1);

}

void SoundManager::onRobotDeath() {

	fbl_play_sound(mSfxExplosion, 1, 0);

	//std::cout << "EXPLOSION SOUND!!!!!!!!" << std::endl;

}

void SoundManager::onFireLaser() {

	fbl_play_sound(mSfxLaser1, 2, 0);

}
