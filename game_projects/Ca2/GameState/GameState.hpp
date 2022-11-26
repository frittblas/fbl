/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	GameState.hpp
*
*	GameState class header, state maching dealing with game states.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include "IState.hpp"

class Game;

class GameState {

public:
    
    enum class StateType {
        // these will reload resources (sprites, text etc)
        Title,
        Demo,
        Settings,
        Tutorial,
        // these share resources
        Explore,
        Dialogue,
        Shop,
        Fight,
        RobotCollection
    };

    GameState();
    ~GameState();

    void  change(Game& g, StateType newState);
    StateType get();
    void tick(Game& g);

private:
    StateType mState;   // enum holding the current state
    IState* mCurrentStateInstance;    // the actual instance of the current game state

};
