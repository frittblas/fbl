/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Chars.hpp
*
*	Chars class header, handles the characters in the game, storing entity id's etc.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include "Ecs/Types.hpp"

class Game;
class Coordinator;

class Chars {

public:

    enum NpcName {
        StorySlime = 10,
        InfoSlime = 30,
        EventSlime = 50,
        ChestMan = 100
    };

    struct NpcRestorer {
        int x, y;
        NpcName name;
    };

    Entity mBrodo;  // the player!

    // the fadeout of the event slime
    int mFadingEventSlimeId;
    int mFadeCounter;

    // array of npc characters to talk to
    //Entity mAllChars[static_cast<int>(Name::NumChars)]; // static cast so we can use Name as int array index.

    std::vector<Entity> mNpc;       // list of all the npcs, (in one scene at a time)

    Chars();
    ~Chars();

    void setupPlayer(Coordinator* mEcs);
    void removePlayer(Coordinator* mEcs);
    void hidePlayer(Coordinator* mEcs);
    void stopPlayerPathing(Game& g);

    void setupNpc(Game& g);
    void removeNpc(Coordinator* mEcs);
    void checkNPC(Game& g, int npc);
    void removeAndStartFade(Game& g, Entity e, int x, int y);
    void resetFadeCounter();
    void removeUsedNpcTile(Game& g, int x, int y, int npc);
    void openChestMan(Game& g, int x, int y);
    void restoreNpcs(Game& g);
    void clearRestoreList();

private:

    std::vector<NpcRestorer> mRestoreList;  // list of the npcs that have been "used up" (open chests, old EventSlimes etc.)

};
