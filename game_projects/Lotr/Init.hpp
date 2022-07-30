/*
*
*	Lotr
*
*	Init.hpp
*
*	Init class header, takes care of initializing the first startup and loading each level.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

class Init {

private:
    static Init instance;

    Init();     // private constructor, so we can't instantiate

public:
    ~Init();

    static Init& getInstance();

    static bool initLotr();
    static void unInitLotr();
    static void initLevel(uint32_t level);

};
