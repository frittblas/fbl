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

#pragma once

class Game {

public:
    Game();
    ~Game();

    bool init();
    void unInit();
    void update();
    void loadLevel();
    void unLoadLevel();

private:


};
