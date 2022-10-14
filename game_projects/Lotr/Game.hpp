/*
*
*	Lotr
*
*	Game.hpp
*
*	Game class header, takes care of initializing stuff.
*   This class coordinates everything and keeps an instance of each subsystem.
*
*	Hans Strömquist 2022
*
*/

#pragma once

// forward declarations
class Coordinator;
class SysManager;
class SoundManager;
class ScenEdit;
class GameState;
class UserInput;
class Chars;
class Location;
class Objects;
class Weather;
class Progress;

class Game {

public:
    Coordinator* mEcs;	    // the Entity Component System
    SysManager* mSysManager;// keeps pointers to all systems in the Ecs.
    SoundManager* mSound;          // 
    ScenEdit* mMap;         // the game map, this points to gEditor
    GameState* mState;  	// current game state
    UserInput* mInput;	    // keyboard and mouse input from the user
    Chars* mChars;          // all the characters in the game, including the player.
    Location* mLocation;    // Locations (levels) in the game, loading, transitions etc.
    Objects* mObjects;      // all the game objects (movable blocks, shops etc)
    Weather* mWeather;      // Weather system (animated clouds and rain, day/night cycle)
    Progress* mProgress;    // general game progress

    static const int LogicalResW = 960;
    static const int LogicalResH = 540;

    static const int MapW = 60;
    static const int MapH = 34;

    static const int TileSize = 32;

    Game();
    ~Game();

    bool init();
    void unInit();
    void update();

};
