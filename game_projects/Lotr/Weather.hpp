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

    Weather();
    ~Weather();

    void tick(Game& g);
    void initClouds(int num);
    void initRain();

};
