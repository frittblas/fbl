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
    uint8_t dir, dirLast;  // facing direction, and last frame dir
    uint8_t layer;         // which layer (for draw order)
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

    int16_t access;   // so the fbl_mouse function doesn't trigger too often.

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
// Addon components /////
/////////////////////////

// Addons have this component
struct Addon {

    uint8_t type;        // examples: Laser, AutoAim etc
    std::string name;    // name
    uint16_t uiId;       // id for the button
    uint16_t tx, ty;     // where on the ui texture is the button?
    uint8_t level;       // Addons can level up (max 3?)
    uint8_t rarity;      // rarity of the addon (common, rare, etc.)
    bool passive;        // passive addon or active?
    int16_t equippedBy;  // who is this equipped by (robots) -1 = notSet
    uint8_t price;       // how much to buy (sells for half)

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
    float   hp;             // current hp
    uint8_t speed;          // speed
    bool diag;              // can go diagonal?
    uint8_t maxEnergy;      // max energy
    uint8_t energy;         // current energy
    uint8_t weight;         // weight
    int16_t slot[6];        // passive skills (like magnetic, diagonal etc.) as Entity (Addon) first 2!
                            // AND active skills (like weapons, for the buttons to the left and right) as Entity (Addon) last 4!
};

// PathLogic (handles when new paths are assigned and stuff)
struct PathLogic
{

    uint16_t baseX, baseY;  // the robot's base coordinates
    uint8_t  flags;         // keep track of how many flags collected
    uint8_t  coins;         // keep track of how many small coins collected
    bool     isAlive;       // is the robot alive?

};

// AutoAim (consists of a invisible ray (line) and dir)
struct AutoAim
{

    uint16_t rayId;         // id for the ray prim
    uint16_t length;        // length of the ray
    uint8_t  dir;           // which direction is beng checked
    uint8_t  intervalMax;   // checking for this many frames in each dir
    uint8_t  intervalCur;   // keep track of interval ticking
    bool     hasTarget;     // do you have somebody in the crosshairs?
    bool     active;        // is auto aim active? Cpu robots have this on, player can choose

};

// Laser (consists of a ray (line) and particle system)
struct Laser
{

    uint16_t rayId;         // id for the ray prim
    uint16_t particleId;    // id for the particle emitter
    uint16_t length;        // length of the beam
    uint8_t  dir;           // in what direction are you firing? can be 0 = up, 1 = right, 2 = left, 3 = down
    uint8_t  damage;        // damage dealt per frame / 10
    uint8_t  level;         // you can level up the laser.
    bool     isFiring;      // are you firing atm?

};

// Magnet (attracts coins)
struct Magnet
{

    uint16_t spriteId;      // id for the magnetic effect sprite
    uint16_t strength;      // strength of the magnet (distance to coin pulling effectish)
    uint8_t  level;         // you can level up the magnet.
    bool     isPulling;     // is the magnet pulling in coins atm?

};