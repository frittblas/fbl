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

class SpriteSystem;

class Chars {

public:

    enum class Name {
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
        Sauron
    };

    Entity mFrodo;  // the player!

    // array of classic characters from the books
    Entity mAllChars[17]; // sizeof(Name)?

    std::vector<Entity> mAlly;      // list of all the allies (in one scene at a time)
    std::vector<Entity> mMonster;   // list of all the monsters (in one scene at a time)


    Chars();
    ~Chars();

};
