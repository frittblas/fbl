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

    static const int cMazeSizeX = 30;
    static const int cMazeSizeY = 17;

    void tick(Game& g);

    void initMaze(Game& g, int density, int numRacers);
    void exitMaze();

    void setupPickStart();
    void pickStartPosition();

    void resetMaze();
    void randomizeMaze(int density);
    void populateMaze();
    void addBorder();
    bool mazeHasAllPaths(int numRacers);

    void assignPaths();


private:

    int mPathId[Robots::NumRobots] = {};        // keep robots path id's
    int mStartPos[Robots::NumRobots][2] = {};   // keep robots starting positions
    uint8_t mUseDiag[Robots::NumRobots] = {};   // keep info on wether robot can go diagonally
    int mNumRacers;

    int mGetReadyTextId = 0;    // get ready text id

    int mGetReadyTimer = 1;	    // seconds of GET READY text with black bg
    int mTimeToPick = 5;        // seconds of actual time player has to pick corner

    int mPickTimer = 0;	        // timer for picking state(in seconds * 60)
    int mPickedPosition = 0;	// stores the picked corner(1 = up left, 2 = up right, 3 down left, 4 down right)

    int mBlackBgId = 0;	        // id for black background id
    int mBlackBgFade = 255;     // bg fadout

    int mTimeBarId = 0;         // green time bar id
    int mTimeBarRed = 0;        // how much red in bar

    int mCircleSize[3];         // for the circle effect

};
