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

#include "../IState.hpp"

class Game;

class Maze : public IState{

public:
    Maze();
    ~Maze();

};
