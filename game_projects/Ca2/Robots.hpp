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
class Game;

class Robots {

public:

    enum Name {
        Charmy,
        Alarmy,
		Boingy,
        Chompy,
        Dancy,
        Floppy,
        Levitaty,
        Necky,
        Partybot,
        Sprinty,
        NumRobots
    };

    const uint8_t  cMaxRobotLevel = 5; // this is the level cap
    const uint32_t Unassigned = 9999;  // used for empty slots in the robot-arrays.

    Entity mAllRobots[NumRobots];     // all the robots
    Entity mOwnedRobots[NumRobots];   // all the robots in your collection
    Entity mRacingRobots[NumRobots];  // all the robots in a race at once

    Entity mTeam[3];        // the robots you're currently using

    int mNumRacers;         // this many robots in a race at once (this gets the value from the same variable in Maze.hpp)

    std::unordered_map<uint16_t, Entity> mSpriteIdToEntityMap; // map spriteId to an entity, used after fbl_ray_hit_sprite()

    Robots();
    ~Robots();

    void setupRobots(Coordinator* mEcs);
    bool addAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType);
    void removeAddonComponent(Coordinator* mEcs, Entity robot, uint8_t addonType);
    void mapSpriteIdToEntity(Coordinator* mEcs);    // used by the laser hitdetection (raycasting)
    void removeRobots(Coordinator* mEcs);
    void hideRobots(Coordinator* mEcs);
    void showRobotInMenu(Coordinator* mEcs, int nameIndex, int entity);
    void showRobotInRace(Coordinator* mEcs, Entity robot, int position);
    void claimRobot(int nameIndex);    // call this to "own" a robot
    void assignAIrobots(Game& g);
    void equipAIaddon(Game& g, int probability, int type);
    int  ownedRobotsLeft(Game& g);
    int  getNameIndexFromEntity(Game& g, uint32_t entity, bool owned);
    int* levelUpRobot(Game& g, int nameIndex, bool owned);
    void levelUpAllFreeRobots(Game& g);
    bool assignRobotXP(Game& g, int nameIndex);
    void shuffleArray(int arr[], int size);
    void shuffleVector(std::vector<int>& vec);

};
