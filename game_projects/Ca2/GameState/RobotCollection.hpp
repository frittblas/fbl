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

    void processInput(Game& g);
    void cyclePages(Game& g, int dir);
    void updateAddonInfo(Game& g);

    virtual void tick(Game& g) override;

private:
    int mCurrentRobotPage;

};
