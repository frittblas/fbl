/*
*
*	Lotr
*
*	GameState.cpp
*
*	GameState class header, state maching dealing with game states.
*
*	Hans Strömquist 2022
*
*/


class GameState {

private:
    
    enum class State {
        Title,
        Demo,
        Settings,
        Tutorial,
        Exploring,
        Dialogue,
        Shopping,
        Fighting,
        CardCollection
    };

    State mState;

public:
    GameState();
    ~GameState();

    void  change(State state);
    State get();

};
