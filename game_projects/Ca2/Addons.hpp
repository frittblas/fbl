/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Addons.hpp
*
*	Addons class header, handles the addons in the game, both in race and explore mode.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include "Ecs/Types.hpp"


class Coordinator;

class Addons {

public:

    enum Type {

        AutoAim,
        Laser,
        NumAddons

    };

    enum Dir {
        Up,
        Right,
        Left,
        Down
    };

    Entity mAllAddons[NumAddons];         // all the addons
    std::vector<Entity> mOwnedAddons;     // all the addons you own

    Addons();
    ~Addons();



};
