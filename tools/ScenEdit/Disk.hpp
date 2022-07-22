/*
*
*	FBL ENGINE
*
*	Disk.hpp
*
*	Save and Load functionality for the tile editor for fbl, header
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include <vector>


class Disk {

    private:
        static Disk instance;

        Disk();     // private constructor, so we can't instantiate

    public:
        ~Disk();

        static Disk &getInstance();

        static bool saveMap();
        static bool loadMap();
        static bool exportMapLua();

};
