/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Maze.hpp
*
*	Maze class header, takes care of the maze generation etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "../../Robots.hpp"

class Game;

class Maze {

public:
    Maze();
    ~Maze();

    // if the flag state is positive, the state corresponds to the carrying robots entity nr.
    enum FlagState {
        Base   = -3,
        Center = -2,
        Dropped = -1
    };

    static const int cMazeSizeX = 30;
    static const int cMazeSizeY = 17;

    static const int cMaxRacers = 4;

    static const int cTargetX = 15 * 32; //Game::TileSize;
    static const int cTargetY = 8 * 32;  //Game::TileSize;

    static const int cMaxFlags = 10;     // max nmber of flags in a level
    static const int cMaxCoins = 20;     // max nmber of coins in a level

    struct aFlag {
        int16_t  id;      // sprite id
        int16_t  state;   // -2 = center, -1 = dropped, > 0 held by that robot entity
    };

    struct aCoin {
        int16_t  id;      // sprite id
        uint8_t  anim;    // play som e nice anim when taken
    };

    struct guiItems {
        uint16_t levelTextId, flagTextId, coinTextId;
        uint8_t  level, flags, coins;
        uint16_t hpRectId, powRectId;
        uint8_t  hp, pow;
    };

    void tick(Game& g);
    void setupPickStart();
    void pickStartPosition(Game& g);
    void initMaze(Game& g, int density, int numRacers);
    void stopPathing();
    void resetMaze();
    void randomizeMaze(int density);
    void populateMaze();
    void addItems();
    void addBorder();
    void createGUI();
    void updateGUI(Game& g);
    void setOneUIbox(Stats stat, PathLogic plog, int base, int entity);
    bool mazeHasAllPaths();
    void assignPaths(Game& g);


private:

    int mPathId[Robots::NumRobots] = {};        // keep robots path id's
    int mStartPos[Robots::NumRobots][2] = {};   // keep robots starting positions
    uint8_t mUseDiag[Robots::NumRobots] = {};   // keep info on wether robot can go diagonally

    guiItems gui[Robots::NumRobots] = {};

    int mNumRacers;     // his many robots in a race at once

    int mGetReadyTextId = 0;    // get ready text id

    int mGetReadyTimer = 1;	    // seconds of GET READY text with black bg
    int mTimeToPick = 6;        // seconds of actual time player has to pick corner

    int mPickTimer = 0;	        // timer for picking state(in seconds * 60)
    int mPickedPosition = -1;	// stores the picked corner(0 = up left, 1 = up right, 2 down left, 3 down right)

    int mBlackBgId = 0;	        // id for black background id
    int mBlackBgFade = 255;     // bg fadout

    int mTimeBarId = 0;         // green time bar id
    int mTimeBarRed = 0;        // how much red in bar

    int mCircleSize[3];         // for the circle effect
    int mFirstCircleId;         // first id of the 12 circles

};
