/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	PostRace.hpp
*
*	PostRace class header, takes care of what happens after the race (and after maintenance).
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
    PostRace();
    ~PostRace();

    void tick(Game& g);
    void gameOver();
    void updateContextHelp(std::string msg);
    void updateAddonInfo(Game& g, bool empty);
    void selectAddon(Game& g);
    void prepShop(Game& g);
    void buySelectedItem(Game& g);
    void initPostRaceMenu(Game& g);

private:
    // id's for the robot collection-menu
    uint16_t mRobotLevel, mRobotHp, mRobotSpeed, mRobotDiag, mRobotEnergy;
    uint16_t mRobotLevelBonus, mRobotHpBonus, mRobotSpeedBonus, mRobotDiagBonus, mRobotEnergyBonus;
    uint16_t mRobotLevelUp;
    uint16_t mAddonName, mAddonLevel, mAddonRarity, mAddonPassive, mAddonEquipped, mAddonPrice;
    uint16_t mContextHelp, mContinueButton, mBuyButton, mFundsText;

    const int notSet = -1;
    int mSelectedAddon;     // currently selected addon in the shop
    int mKeyAccess;

};
