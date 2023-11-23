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

    // Could use normal enum here, so we can use the enum as array index directly.
    // Now we have to use static_cast, see below
    enum NpcName {
        StorySlime,
        InfoSlime,
        EventSlime,
        NumChars
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
    void removeEventSlime(Game& g);
    void removeAndStartFade(Game& g, Entity e, int x, int y);
    void resetFadeCounter();


};
