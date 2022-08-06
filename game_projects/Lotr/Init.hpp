/*
*
*	Lotr
*
*	Init.hpp
*
*	Init class header, takes care of initializing the first startup and loading each level.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

class Init {

public:
    Init();
    ~Init();

    bool initLotr();
    void unInitLotr();
    void loadLevel();
    void unLoadLevel();
    void update();

};
