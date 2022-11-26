/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Robots.hpp
*
*	Robots class header, handles the robots in the game, storing entity id's etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include "Ecs/Types.hpp"

class Game;
class Coordinator;

class Robots {

public:

    // Now we have to use static_cast, see below
    enum class RobotName {
        Charming,
        Chompy,
		Dancy,
        NumRobots
    };

    const int Unassigned = 1000000;     // used for empty slots in the mAllRobots vector.

    std::vector<Entity> mAllRobots;     // list of all the robots
    std::vector<Entity> mOwnedRobots;   // list of all the robots in your collection

    Robots();
    ~Robots();

    void setupRobots(Coordinator* mEcs);
    void removeRobots(Coordinator* mEcs);
    void hideRobots(Coordinator* mEcs);
    void claimRobot(RobotName name);    // call this to "own" a robot

};
