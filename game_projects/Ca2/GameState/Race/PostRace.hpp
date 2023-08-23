/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	PostRace.hpp
*
*	PostRace class header, takes care of what happens after the race.
*	Either GAME OVER (if all your robots are dead) or eventual level up
*	and opportunity to buy one of 3 items (2 addons and a robot.)
*
*	Hans Strömquist 2023
*
*/

#pragma once


class Game;

class PostRace {

public:
    PostRace(bool win);
    ~PostRace();

    void tick(Game& g);
    void gameOver();
    void PostRace::updateItemInfo(Game& g, bool empty);
    void initPostRaceMenu();

private:
    // id's for the robot collection-menu
    uint16_t fContextHelp, fContextLine;
    uint16_t fMenuBgSquareId, fMenuBgOutlineId, fMenuRobotBgSquareId;
    uint16_t fMenuDividerLine, fSmallMenuDividerLine;
    uint16_t fMenuRobotDescr, fMenuAddonsDescr;
    uint16_t fMenuName, fMenuLevel, fMenuXp, fMenuHp, fMenuSpeed;
    uint16_t fMenuDiag, fMenuEnergy, fMenuWeight;
    uint16_t fMenuAddonInfoLine;
    uint16_t fAddonName, fAddonLevel, fAddonRarity, fAddonPassive, fAddonEquipped, fAddonPrice;
    uint16_t fContinue, fContinueText;

    const int notSet = -1;
    int mSelectedAddon;     // currently selected addon in the shop
    int mKeyAccess;

};
