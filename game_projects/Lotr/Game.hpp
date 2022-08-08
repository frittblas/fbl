/*
*
*	Lotr
*
*	Game.hpp
*
*	Game class header, takes care of initializing stuff and to load each level.
*   This class coordinates everything and keeps an instance of each subsystem.
*
*	Hans Strömquist 2022
*
*/

#pragma once

// forward declarations
class Coordinator;
class ScenEdit;
class GameState;
class UserInput;
class Progress;

class Game {

public:
    Coordinator* mEcs;	    // the Entity Component System
    ScenEdit* mMap;         // the game map, this points to gEditor
    GameState* mState;  	// current game state
    UserInput* mInput;	    // keyboard and mouse input from the user
    Progress* mProgress;    // general game progress

    Game();
    ~Game();

    bool init();
    void unInit();
    void update();
    void loadLevel();
    void unLoadLevel();

};
