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

    const uint32_t Unassigned = 9999; // used for empty slots in the addon-arrays.

    Entity mAllAddons[NumAddons];     // all the addons
    Entity mOwnedAddons[NumAddons];   // all the addons you own
    Entity mRacingAddons[6];          // 2 passive, 4 active

    Addons();
    ~Addons();

    void setupAddons(Coordinator* mEcs);
    void removeAddons(Coordinator* mEcs);
    void hideAddons(Coordinator* mEcs);
    void showAddonsInMenu(Coordinator* mEcs);
    void showAddonInRace(Coordinator* mEcs, Entity addon, int position);
    void claimAddon(int nameIndex);    // call this to "own" an Addon

};
