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

		//if (tweenList[i].curValue < tweenList[i].endValue - 0.02 || tweenList[i].curValue > tweenList[i].endValue + 0.02) { // don't update anymore if done

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

		//}

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
		fbl_set_viewport(x, y, Game::LogicalResW, Game::LogicalResH);
		shakeDuration--;

		// gradually decrease intensity
		if (shakeDuration % 2 == 0)
			shakeIntensity--;

	}
	else if (shakeDuration == 1) {
		fbl_set_viewport(0, 0, Game::LogicalResW, Game::LogicalResH);
		shakeDuration = 0;
	}

}


void Efx::setupFilmTransition(bool in) {


}

void Efx::tickFilmTransition() {

}

void Efx::onRobotDeath() {

	shakeCamera(30, 50);

}
