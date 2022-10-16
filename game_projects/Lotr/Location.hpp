/*
*
*	Lotr
*
*	Location.hpp
*
*	Location class header, takes care of all locations (levels), loading, unloading, transitions etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

class ScenEdit;

class Location {

public:

    Location();
    ~Location();

    void change(ScenEdit* map, uint32_t posX, uint32_t posY);
    void transition(uint8_t type, bool out);
    void loadLocation(ScenEdit* map);
    void unLoadLocation(ScenEdit* map);

};
