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

    std::vector<Cloud*> mCloud; // list of clouds
    uint32_t rainLayerId[3];
    bool rainOn;
    uint8_t tint_r, tint_g, tint_b; // for time of day
    bool lightningOn, lightningTrigger;
    uint8_t lightningTimer;

    Weather();
    ~Weather();

    void tick(Game& g);
    void setWeather(TimeOfDay time, uint8_t rainLevel, uint8_t numClouds, bool lightning);
    void initClouds(uint8_t num);
    void initRain(uint8_t amount, uint8_t alpha);   // amount of 1 = max

};
