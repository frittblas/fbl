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


class Maze {

public:
    Maze();
    ~Maze();

    static const int MazeSizeX = 30;
    static const int MazeSizeY = 17;

    void setupPickStart();
    void pickStartPosition();

    // void find_paths()
    void resetMaze();
    void randomizeMaze(int density);
    void populateMaze();
    void addBorder();
    // void bool maze_has_all_paths()
    // assign_paths()


private:
    uint8_t mMazeMap[MazeSizeX][MazeSizeY];

};
