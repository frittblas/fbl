/*
*
*	FBL ENGINE
*
*	TileData.hpp
*
*	The tile data struct.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

struct TileData {

    uint32_t id;
    uint32_t x, y;
    uint32_t textureX, textureY;
    uint32_t layer, terrain;
    bool kinematic, animated;   // NOTE: change kinematic to terrain and have 0,1,2 as walkable, solid, water
    uint32_t animFrames, animSpeed;

};
