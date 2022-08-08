/*
*
*	Ecs
*
*	Components.hpp
*
*	Here goes all the components used by the entities.
*
*	I got this from https://austinmorlan.com/posts/entity_component_system/
*	Rearanged it a bit for Lotr.
*
*/

#pragma once

#include <iostream>

// the position of an entity
struct Position
{

    int x, y;   // position in the world

};

// the sprite render component
struct Sprite
{
    // try to use smaller datatypes to be more cache friendly

    uint16_t id[4]; // 4 sprites, one for each direction (up, right, left, down)
    uint8_t num;    // number of actual sprites (max 4)
    uint16_t textureX, textureY; // where on the sprite sheet is the sprite located?
    uint8_t w, h;   // width and height of sprite
    bool animated;  // is animated or not
    uint8_t frames, speed; // how many animation frames, and anim speed
    uint8_t dir, dirLast;    // facing direction, and last frame dir

};
