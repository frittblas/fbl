/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Objects.hpp
*
*	Objects class header, keeps references to the game objects, movable rocks, shops etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include "Ecs/Types.hpp"

class Objects {

public:
    std::vector<Entity> mMoveBlock; // list of all the movable blocks in a scene

    Objects();
    ~Objects();

};
