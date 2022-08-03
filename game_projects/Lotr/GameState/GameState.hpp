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

#include "IState.hpp"

class GameState {

public:
    
    enum class StateType {
        Title,
        Demo,
        Settings,
        Tutorial,
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

private:
    StateType mState;   // enum holding the current state (int)
    IState* mCurrentStateInstance = nullptr;    // the actual instance of the current game state

};
