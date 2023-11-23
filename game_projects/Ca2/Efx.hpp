/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Efx.hpp
*
*	Efx class header, deals with different effects such as cameraShake, tweening, transitions etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IObserver.hpp"

class Efx : public IObserver{

    public:

        enum Tweens {
            EaseOut,
            ElasticOut
        };

        ~Efx();

        static Efx &getInstance();

        static const int MaxTweens = 10;

        // tweening
        int   setupTween(int startVal, int endVal, int duration, int type);   // returns id (array position)
        float getCurValue(int id);
        void  tickTweens();

        // camera shake
        void shakeCamera(int intensity, int duration);
        void tickCameraShake();

        // explosion
        void initExplosion();
        void startExplosion(int x, int y);
        void tickExplosion();

        // coin effect
        void initCoinEfx();
        void startCoinEfx(int x, int y);
        void tickCoinEfx();

        // black border transition, film style
        void setupFilmTransition(bool in);
        void tickFilmTransition();

        void onRobotDeath() override;

    private:

        struct Tween {
            float startValue, endValue, curValue;
            int tweenStartTime, tweenDuration, tweenType;
        };

        static Efx instance;
        Efx();                              // private constructor, so we can't instantiate

        Tween tweenList[MaxTweens] = {};    // can have 10 tweens going on at the same time
        uint32_t nextTweenId;               // next free id

        uint16_t shakeIntensity, shakeDuration;  // camera shake stuff :)

        uint16_t explosionId, explodeDuration;  // explosion things
        uint16_t coinEfxId, coinEfxDuration;  // coin things

        int viewPortX, viewPortY;

        // add more of these
        float easeOut(float t);
        float elasticOut(float t);

};
