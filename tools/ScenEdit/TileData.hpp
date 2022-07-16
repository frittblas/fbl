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

#include <iostream>

struct TileData {

    uint32_t id;
    uint32_t x, y;
    uint32_t texture_x, texture_y;
    uint32_t layer;
    bool kinematic, animated;
    uint32_t animFrames, animSpeed;

};
