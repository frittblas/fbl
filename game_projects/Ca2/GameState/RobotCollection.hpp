/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	RobotCollection.hpp
*
*	RobotCollection class header, takes care of upgrading robots etc..
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IState.hpp"

class Game;

void initCollectionMenu();
void showCollectionMenu();
void hideCollectionMenu();

class RobotCollection : public IState {

public:
    RobotCollection();
    ~RobotCollection();

    void cyclePages(Game& g, int dir);
    void updateAddonInfo(Game& g, bool empty);
    void setFreeSlotsArrows(Game& g, bool empty);
    void selectAddon(Game& g);
    void equipAddon(Game& g);
    void processInput(Game& g);

    virtual void tick(Game& g) override;

private:
    const int notSet = -1;
    int mCurrentRobotPage;  // what robot-page are you at? It's an index to the owned robot array
    int mSelectedAddon;     // currently selected addon on the grid (as entity id)
    int mKeyAccess;
    bool mMouseDown;

};
