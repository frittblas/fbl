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
	/*
	mSfxCoin = fbl_load_sound("sfx/coin.ogg");
	mSfxDenied = fbl_load_sound("sfx/denied.ogg");
	mSfxExplosion = fbl_load_sound("sfx/explosion1.ogg");
	fbl_set_sound_volume(mSfxExplosion, 40); // max is 128
	mSfxHeal = fbl_load_sound("sfx/heal.ogg");
	mSfxLaser1 = fbl_load_sound("sfx/laser1.ogg");
	mSfxMagnet = fbl_load_sound("sfx/magnet.ogg");
	mSfxPass = fbl_load_sound("sfx/pass.ogg");
	mSfxPowerup = fbl_load_sound("sfx/powerup.ogg");
	mSfxQuit = fbl_load_sound("sfx/quit.ogg");
	fbl_set_sound_volume(mSfxQuit, 60); // max is 128
	mSfxShield = fbl_load_sound("sfx/shield.ogg");
	mSfxSnap = fbl_load_sound("sfx/snap.ogg");
	mSfxTick = fbl_load_sound("sfx/tick.ogg");
	fbl_set_sound_volume(mSfxTick, 20); // max is 128
	mSfxTurbo = fbl_load_sound("sfx/turbo.ogg");

	mSfxRainStorm = fbl_load_sound("sfx/rain.ogg");
	mSfxSummer = fbl_load_sound("sfx/summer.ogg");

	fbl_play_sound(mSfxSummer, Ambient, 1);
	loadAndPlayMusic("music/title.ogg", 40);
	*/
}

void SoundManager::loadAndPlayMusic(std::string filename, int volume) {

	//fbl_destroy_music();
	//fbl_load_music(filename.c_str());
	//fbl_play_music(volume, 0);	// max volume = 128

}

void SoundManager::onRobotDeath() {

	//fbl_play_sound(mSfxExplosion, 1, 0);

	//std::cout << "EXPLOSION SOUND!!!!!!!!" << std::endl;

}

void SoundManager::onFireLaser() {

	//fbl_play_sound(mSfxLaser1, 2, 0);

}
