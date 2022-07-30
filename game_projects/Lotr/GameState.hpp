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
    
    enum State {
        Title,
        Settings,
        Tutorial,
        Wandering,
        Dialogue,
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
