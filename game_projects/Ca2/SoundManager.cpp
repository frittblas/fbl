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
	//fbl_play_sound(0, -1);
	//loadAndPlayMusic("music/harp.ogg");

	std::cout << "Initialized SoundManager subsystem." << std::endl;

}

SoundManager::~SoundManager() {

	std::cout << "Destroyed SoundManager subsystem." << std::endl;

}

void SoundManager::loadSfx() {

	// load all the sound effects in the game and assign id's

	fbl_load_sound("sfx/summer.ogg");
	fbl_load_sound("sfx/rain.ogg");

}

void SoundManager::loadAndPlayMusic(std::string filename) {

	fbl_destroy_music();
	fbl_load_music(filename.c_str());
	fbl_play_music(-1);

}
