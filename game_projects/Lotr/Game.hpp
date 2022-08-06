/*
*
*	Lotr
*
*	Game.hpp
*
*	Game class header, takes care of initializing the first startup and to load each level.
*
*	Hans Strömquist 2022
*
*/

class Game {

public:
    Game();
    ~Game();

    bool initLotr();
    void unInitLotr();
    void loadLevel();
    void unLoadLevel();
    void update();

};
