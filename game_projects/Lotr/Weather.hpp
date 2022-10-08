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

    std::vector<Cloud*> mCloud; // list of clouds
    uint32_t rainLayerId[3];

    Weather();
    ~Weather();

    void tick(Game& g);
    void initClouds(int num);
    void initRain(uint8_t amount, uint8_t alpha);   // amount of 1 = max

};
