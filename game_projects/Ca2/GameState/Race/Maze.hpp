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

    static const int MazeSizeX = 30;
    static const int MazeSizeY = 17;

    void initMaze(Game& g, int density, int numRacers);

    //void setupPickStart();
    //void pickStartPosition();

    void resetMaze();
    void randomizeMaze(int density);
    void populateMaze();
    void addBorder();
    bool mazeHasAllPaths(int numRacers);

    // assign_paths()


private:
    uint8_t mMazeMap[MazeSizeX][MazeSizeY];

    int pathId[Robots::NumRobots] = {};
    int startPos[Robots::NumRobots][2] = {};

};
