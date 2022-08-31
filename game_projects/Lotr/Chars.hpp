/*
*
*	Lotr
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
    enum class Name {
        Slime,
        Bilbo,
        Gandalf,
        Aragorn,
        Gimli,
        Legolas,
        Samwise,
        Faramir,
        Boromir,
        Treebreard,
        Merry,
        Pippin,
        Bombadill,
        Arwen,
        Galadriel,
        Gollum,
        Saruman,
        Sauron,
        NumChars
    };

    Entity mFrodo;  // the player!

    // array of classic characters from the books
    Entity mAllChars[static_cast<int>(Name::NumChars)]; // static cast so we can use Name as int array index.

    std::vector<Entity> mNpc;       // list of all the npcs, not fighting (in one scen at a time)
    std::vector<Entity> mAlly;      // list of all the allies (in one scene at a time)
    std::vector<Entity> mMonster;   // list of all the monsters (in one scene at a time)


    Chars();
    ~Chars();

    void setupPlayer(Coordinator* mEcs);
    void removePlayer(Coordinator* mEcs);

    void setupNpc(Game& g);
    void removeNpc(Coordinator* mEcs);

};
