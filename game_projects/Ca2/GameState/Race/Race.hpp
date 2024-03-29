/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Race.hpp
*
*	Race class header, takes care of the Race game state.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "../IState.hpp"
#include "Maze.hpp"
#include "PostRace.hpp"

class Game;
struct Addon;

class Race : public IState{

public:
    Race();
    ~Race();

    enum GameMode {
        GM_CaptureFlags,
        GM_DeathMatch
    };

    enum RaceState {
        Undecided,
        First,
        Second,
        Third,
        Fourth,
        Dead,
        GameOver
    };

    void assignRobots(Game& g);
    void unassignRobots(Game& g);
    void getInput(Game& g);
    void handleAddons(Game& g, Addon& add, Entity playingRobot, bool onOff);

    virtual void tick(Game& g) override;

    static int sRaceState;

private:
    Maze *mMaze;
    PostRace* mPostRace;
    int mPostRaceDelay;
    uint8_t mNumRacers;

};
