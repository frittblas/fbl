/*
*
*	Ecs
*
*	Components.hpp
*
*	Here goes all the components used by the entities.
*
*	I got this from https://austinmorlan.com/posts/entity_component_system/
*	Rearranged it a bit for Ca2.
*
*/

#pragma once

#include <iostream>

// try to use smaller datatypes to be more cache friendly

// the position of an entity
struct Position
{

    float x, y;   // position in the world

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
    uint8_t layer;  // which layer (for draw order)

};

// the path finding component
struct Path
{

    uint8_t id; // the path id from the a_star module
    uint16_t goalX, goalY;  // the coords of the current goal to reach
    bool newPath;   // is this is set to true, find a new path
    float speed;    // how fast is the entity following the path
    uint8_t diag;   // using diagonals or not
    uint8_t pixelsPerFrame;     // check out the astar api

};

// the mouse controller component (control a character in explore mode)
struct MouseCtrl
{

    bool clicked;   // so the fbl_mouse function doesn't fire several times.

};

// the camera component (the camera will follow the entity with this)
struct Camera
{

    float x, y;    // camera position
    float damp;     // camera smoothing amount
    uint8_t deadZoneW, deadZoneH;    // deadzone size in tiles

};

// Dialogue trigger component, player starts dialogue with entities with this.
struct DialogueTrigger
{

    uint8_t dialogueId; // id of which dialogue to use in the Lua system

};

// light, a 2D-light surrounding an entity.
struct Light
{

    uint16_t id;    // the light-sprite id
    uint16_t textureX, textureY; // where on the sprite sheet is the light located?
    uint8_t w, h;   // width and height of light
    float scale;    // light size (scale)

};


/////////////////////////
// Robot components /////
/////////////////////////

// Robot stats
struct Stats
{

    std::string name;       // name
    uint8_t level;          // the current level
    uint16_t xp;            // experience points
    uint16_t nextLv;        // limit for next level
    uint8_t maxHp;          // max hp
    uint8_t hp;             // current hp
    uint8_t speed;          // speed
    bool diag;              // can go diagonal?
    uint8_t maxEnergy;      // max energy
    uint8_t energy;         // current energy
    uint8_t weight;         // weight
    uint8_t activeSlot[4];  // active skills (like weapons, for the buttons to the left and right)
    uint8_t passiveSlot[2]; // passive skills (like magnetic, diagonal etc.)

};

// Addons have this component
struct Addon {

    uint8_t type;   // might be unnecessary
    uint8_t rarity; // rariity of the addon (common, rare, etc.)
    bool passive;   // passive addon or active?

    // hmm how to solve this?
    // should the addons have 2 components or one
    // should the robots use the actual

    // I think addons should have: addon component, sprite component aand laser component?? think about that


};

// laser (consists of a ray (line) and particle system)
struct Laser
{

    uint16_t rayId;         // id for the ray prim
    uint16_t particleId;    // id for the particle emitter
    uint16_t type;          // laser type (red, white, green, blue)
    uint16_t length;        // length of the beam
    uint8_t dir;            // in what direction are you firing? can be 0 = front, 1 = back, 2 = left, 3 = right
    uint8_t damage;         // damage dealt per frame

};
