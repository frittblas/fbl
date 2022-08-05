/*
*
*	Lotr
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
        CardCollection
    };

    GameState();
    ~GameState();

    void  change(StateType state);
    StateType get();
    void tick();

private:
    StateType mState;   // enum holding the current state (int)
    IState* mCurrentStateInstance;    // the actual instance of the current game state

};
