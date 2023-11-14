/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Maintenance.hpp
*
*	Maintenance class header, takes care of the Maintenance game state.
*
*	Hans Strömquist 2023
*
*/

#pragma once

#include "IState.hpp"

#include "Race/PostRace.hpp"

class Game;
struct Addon;

class Maintenance : public IState{

public:
    Maintenance();
    ~Maintenance();

    enum RaceState {
        Undecided,
        First,
        Second,
        Third,
        Fourth,
        Dead,
        GameOver
    };

    void setupMaintenance(Game& g);
    void getInput(Game& g);


    virtual void tick(Game& g) override;

    //static int sRaceState;

private:

    PostRace* mPostRace;
    int mPostRaceDelay;

};
