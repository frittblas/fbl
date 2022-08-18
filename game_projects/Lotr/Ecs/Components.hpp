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

// try to use smaller datatypes to be more cache friendly

// the position of an entity
struct Position
{

    uint16_t x, y;   // position in the world

};

// the sprite render component
struct Sprite
{

    uint16_t id[4]; // 4 sprites, one for each direction (up, right, left, down)
    uint8_t num;    // number of actual sprites (max 4)
    uint16_t textureX, textureY; // where on the sprite sheet is the sprite located?
    uint8_t w, h;   // width and height of sprite
    bool animated;  // is animated or not
    uint8_t frames, speed; // how many animation frames, and anim speed
    uint8_t dir, dirLast;    // facing direction, and last frame dir

};

// the path finding component
struct Path
{

    uint8_t id; // the path id from the a_star module
    uint16_t goalX, goalY;  // the coords of the current goal to reach
    bool newPath;   // is this is set to true, find a new path

};

// the mouse controller component (control a character in explore mode)
struct MouseCtrl
{

    bool clicked;   // so the fbl_mouse function doesn't fire several times.

};

// the camera component (the camera will follow the entity with this)
struct Camera
{

    float x, y;     // camera position
    float damp;     // camera smoothing amount
    uint16_t trapX, trapY, trapW, trapH;    // deadzone

};
