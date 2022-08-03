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

private:
    
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

    StateType mState;   // enum holding the current state (int)
    IState* mCurrentStateInstance = nullptr;    // the actual instance of the current game state

public:
    GameState();
    ~GameState();

    void  change(StateType state);
    StateType get();

};
