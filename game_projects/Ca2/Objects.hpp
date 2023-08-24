/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Objects.hpp
*
*	Objects class header, keeps references to the game objects, movable rocks
*   and other interactable things in explore mode etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include "Ecs/Types.hpp"

class Objects {

public:
    Objects();
    ~Objects();

    std::vector<Entity> mMoveBlock; // list of all the movable blocks in a scene

};
