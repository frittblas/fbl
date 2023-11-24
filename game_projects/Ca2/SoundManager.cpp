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
	
	mSfxCoin = fbl_load_sound("sfx/coin.ogg");
	fbl_set_sound_volume(mSfxDenied, 50); // max is 128
	mSfxDenied = fbl_load_sound("sfx/denied.ogg");
	fbl_set_sound_volume(mSfxDenied, 40); // max is 128
	mSfxExplosion = fbl_load_sound("sfx/explosion1.ogg");
	fbl_set_sound_volume(mSfxExplosion, 40); // max is 128
	mSfxHeal = fbl_load_sound("sfx/heal.ogg");
	mSfxLaser1 = fbl_load_sound("sfx/laser1.ogg");
	mSfxMagnet = fbl_load_sound("sfx/magnet.ogg");
	mSfxPass = fbl_load_sound("sfx/pass.ogg");
	fbl_set_sound_volume(mSfxPass, 50); // max is 128
	mSfxPowerup = fbl_load_sound("sfx/powerup.ogg");
	fbl_set_sound_volume(mSfxPowerup, 50); // max is 128
	mSfxShutdown = fbl_load_sound("sfx/shutdown.ogg");
	fbl_set_sound_volume(mSfxShutdown, 50); // max is 128
	mSfxQuit = fbl_load_sound("sfx/quit.ogg");
	fbl_set_sound_volume(mSfxQuit, 60); // max is 128
	mSfxShield = fbl_load_sound("sfx/shield.ogg");
	fbl_set_sound_volume(mSfxQuit, 30); // max is 128
	mSfxSnap = fbl_load_sound("sfx/snap.ogg");
	mSfxTick = fbl_load_sound("sfx/tick.ogg");
	fbl_set_sound_volume(mSfxTick, 20); // max is 128
	mSfxTurbo = fbl_load_sound("sfx/turbo.ogg");
	
	mSfxRainStorm = fbl_load_sound("sfx/rain.ogg");
	mSfxSummer = fbl_load_sound("sfx/summer.ogg");
	
}

void SoundManager::playSfx(int id, int channel, int loops) {

	fbl_play_sound(id, channel, loops);

}

void SoundManager::loadAndPlayMusic(std::string filename, int volume, int loops) {

	fbl_destroy_music();
	fbl_load_music(filename.c_str());
	fbl_play_music(volume, loops);	// max volume = 128

}

void SoundManager::stopMusic() {

	fbl_stop_music();

}

void SoundManager::onRobotDeath() {

	playSfx(mSfxExplosion, 1, 0);

	//std::cout << "EXPLOSION SOUND!!!!!!!!" << std::endl;

}

void SoundManager::onFireLaser() {

	playSfx(mSfxLaser1, 2, 0);

}
