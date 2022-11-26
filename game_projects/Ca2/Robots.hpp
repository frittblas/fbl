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
    enum class Name {
        Charming,
        Chompy,
		Dancy,
        NumRobots
    };


    // array of all the playable robots 
    Entity mAllRobots[static_cast<int>(Name::NumRobots)]; // static cast so we can use Name as int array index.

    std::vector<Entity> mOwnedRobots;       // list of all the robots in your collection

    Robots();
    ~Robots();

    void setupRobots(Coordinator* mEcs);
    void removeRobots(Coordinator* mEcs);


};
