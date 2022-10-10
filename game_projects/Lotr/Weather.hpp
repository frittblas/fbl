/*
*
*	Lotr
*
*	Weather.hpp
*
*	Weather class header, handles the weather system.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include <iostream>

class Game;

struct Cloud {

    uint32_t id;
    float x, y;
    float speed, scale;
    uint8_t alpha;

};

class Weather {

public:

    enum class TimeOfDay {

        Morning,
        Day,
        Evening,
        Late,
        Night

    };

    Weather();
    ~Weather();

    void tick(Game& g);
    void setWeather(TimeOfDay time, uint8_t rainLevel, uint8_t snowLevel, uint8_t numClouds, bool doLightning);

private:

    void initClouds(uint8_t num);
    void initRain(uint8_t amount, uint8_t alpha);   // amount of 1 = max
    void initSnow(uint8_t amount, uint8_t alpha);

    std::vector<Cloud*> mCloud; // list of clouds
    uint32_t mRainLayerId[3], mSnowLayerId[3];
    bool mRainOn, mSnowOn;
    uint8_t mTint_r, mTint_g, mTint_b; // for time of day
    bool mLightningOn, mLightningTrigger;
    uint8_t mLightningTimer;

};
