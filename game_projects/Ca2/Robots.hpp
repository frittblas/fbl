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

//#include <vector>
#include "Ecs/Types.hpp"


class Coordinator;

class Robots {

public:

    enum Name {
        Charmy,
        Alarmy,
		Boingy,
        Chompy,
        NumRobots
    };

    const uint32_t Unassigned = 9999; // used for empty slots in the robot-arrays.

    Entity mAllRobots[NumRobots];     // all the robots
    Entity mOwnedRobots[NumRobots];   // all the robots in your collection
    Entity mRacingRobots[NumRobots];  // all the robots in a race at once

    Entity mTeam[3];        // the robots you're currently using

    int mNumRacers;         // his many robots in a race at once (this gets the value from the sam variable in Maze.hpp)

    std::unordered_map<uint16_t, Entity> mSpriteIdToEntityMap; // map spriteId to an entity, used after fbl_ray_hit_sprite()

    Robots();
    ~Robots();

    void setupRobots(Coordinator* mEcs);
    void mapSpriteIdToEntity(Coordinator* mEcs);    // used by the laser hitdetection (raycasting)
    void removeRobots(Coordinator* mEcs);
    void hideRobots(Coordinator* mEcs);
    void showRobotInMenu(Coordinator* mEcs, int nameIndex);
    void showRobotInRace(Coordinator* mEcs, Entity robot, int position);
    void claimRobot(int nameIndex);    // call this to "own" a robot

};
