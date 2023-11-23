/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Efx.cpp
*
*	Efx class implementation, deals with different effects such as cameraShake, tweening, transitions etc.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "../../src/fbl.hpp"
#include "Game.hpp"
#include "Efx.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Efx-class implementation

Efx::Efx() {

	shakeIntensity = 0;
	shakeDuration = 0;
	nextTweenId = 0;

	// get the original viewport so screenshake sets it correctly
	int w, h;
	fbl_get_viewport(&viewPortX, &viewPortY, &w, &h);

	std::cout << "Initialized Efx subsystem." << std::endl;
}

Efx::~Efx() {

	std::cout << "Destroyed Efx subsystem." << std::endl;

}

Efx Efx::instance;

Efx &Efx::getInstance() {

    return instance;

}

// easing functions
float Efx::easeOut(float t) {
	return 1 - pow(1 - t, 3);
}

float Efx::elasticOut(float t) {
	float p = 0.3;
	return pow(2, -10 * t) * sin((t - p / 4) * (2 * M_PI) / p) + 1;
}

int Efx::setupTween(int startVal, int endVal, int duration, int type) {


	tweenList[nextTweenId % MaxTweens].startValue = startVal;	 // animation start point
	tweenList[nextTweenId % MaxTweens].endValue = endVal;		 // animation end point

	tweenList[nextTweenId % MaxTweens].curValue = startVal;		 // curVal is the tweened value that you want (get with getCurValue(int id))

	tweenList[nextTweenId % MaxTweens].tweenStartTime = fbl_timer_get_ticks();
	tweenList[nextTweenId % MaxTweens].tweenDuration = duration; // ms

	tweenList[nextTweenId % MaxTweens].tweenType = type;		 // easeOut, elasticOut etc.

	nextTweenId++;

	return (nextTweenId - 1) % MaxTweens;

}

float Efx::getCurValue(int id) {

	return tweenList[id].curValue;

}

void Efx::tickTweens() {

	for (int i = 0; i < MaxTweens; i++) {

		int elapsedTime = fbl_timer_get_ticks() - tweenList[i].tweenStartTime;
		float t = (float)elapsedTime / tweenList[i].tweenDuration;
		t = fmin(t, 1);

		switch (tweenList[i].tweenType) {

		case Efx::Tweens::EaseOut:
			tweenList[i].curValue = tweenList[i].endValue * Efx::easeOut(t);
			break;
		case Efx::Tweens::ElasticOut:
			tweenList[i].curValue = tweenList[i].endValue * Efx::elasticOut(t);
			break;

		}

	}

}

void Efx::shakeCamera(int intensity, int duration) {

	if (shakeDuration == 0) {
		shakeIntensity = intensity;
		shakeDuration = duration;
	}

}

void Efx::tickCameraShake() {

	int x, y;

	if (shakeDuration > 1) {
		x = y = rand() % shakeIntensity; // -shakeIntensity / 2;
		fbl_set_viewport(viewPortX + x, viewPortY + y, Game::LogicalResW, Game::LogicalResH);
		shakeDuration--;

		// gradually decrease intensity
		if (shakeDuration % 2 == 0)
			shakeIntensity--;

	}
	else if (shakeDuration == 1) {
		fbl_set_viewport(viewPortX, viewPortY, Game::LogicalResW, Game::LogicalResH);
		shakeDuration = 0;
	}

}

void Efx::initExplosion() {
	// create the particles
	explosionId = fbl_create_emitter(150);	// create emitter with 150 particles, creating a mezmerizing nebulosa :)
	fbl_set_emitter_params(explosionId, FBL_EMITTER_FLOWER, 10, 10, 100, 1, 3, 0.3, 1.5);
	fbl_set_emitter_particle_shape(explosionId, FBL_NO_PRIM, 448, 128, 64, 64);	// use particle image instead of prim
	fbl_set_emitter_active(explosionId, false);
	explodeDuration = 50;
}

void Efx::startExplosion(int x, int y) {

	fbl_set_emitter_xy(explosionId, x, y);
	fbl_set_emitter_active(explosionId, true);
	explodeDuration = 49;

}

void Efx::tickExplosion() {

	if (explodeDuration < 50) {

		explodeDuration--;
		if (explodeDuration < 1) {
			fbl_set_emitter_active(explosionId, false);
			explodeDuration = 50;
		}
	}

}

void Efx::initCoinEfx() {

	// create the particles
	coinEfxId = fbl_create_emitter(50);	// create emitter with 50 particles
	fbl_set_emitter_color(coinEfxId, 255, 224, 0, 255, true);
	fbl_set_emitter_color(coinEfxId, 255, 253, 234, 0, false);
	fbl_set_emitter_params(coinEfxId, FBL_EMITTER_FLOWER, 5, 5, 30, 1, 3, 0.3, 1.0);
	fbl_set_emitter_active(coinEfxId, false);
	coinEfxDuration = 30;

}

void Efx::startCoinEfx(int x, int y) {

	fbl_set_emitter_xy(coinEfxId, x, y);
	fbl_set_emitter_active(coinEfxId, true);
	coinEfxDuration = 29;

}

void Efx::tickCoinEfx() {

	if (coinEfxDuration < 30) {

		coinEfxDuration--;
		if (coinEfxDuration < 1) {
			fbl_set_emitter_active(coinEfxId, false);
			coinEfxDuration = 30;
		}
	}

}

void Efx::setupFilmTransition(bool in) {


}

void Efx::tickFilmTransition() {

}

void Efx::onRobotDeath() {

	shakeCamera(30, 50);

}
