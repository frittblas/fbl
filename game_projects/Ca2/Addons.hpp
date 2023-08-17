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

        AutoAim1,
        AutoAim2,
        AutoAim3,
        Laser1,
        Laser2,
        Laser3,
        Magnet1,
        Magnet2,
        Magnet3,
        Turbo1,
        Turbo2,
        Turbo3,
        Shield1,
        Shield2,
        Shield3,
        Heal1,
        Heal2,
        Heal3,
        Diag,
        RobotCtrl,
        NumAddons

    };

    enum Dir {
        Up,
        Right,
        Left,
        Down
    };

    const uint16_t Unassigned = 9999;      // used for empty slots in the addon-arrays.
    const int16_t  notSet = -1;            // used for equippedBy

    Entity mAllAddons[NumAddons];          // all the addons
    Entity mOwnedAddons[NumAddons];        // all the addons you own

    Addons();
    ~Addons();

    void setupAddons(Coordinator* mEcs);
    void initAddons(Coordinator* mEcs);
    void initRaceAddons(Coordinator* mEcs);
    void setAddonColor(int uiId, int level);
    void removeAddons(Coordinator* mEcs);
    void hideAddons(Coordinator* mEcs);
    void showAddonsInMenu(Coordinator* mEcs);
    void showAddonAsEquipped(Coordinator* mEcs, Entity addon, int position);
    void showAddonInRace(Coordinator* mEcs, Entity addon, int position);
    void claimAddon(int nameIndex);    // call this to "own" an Addon

};
