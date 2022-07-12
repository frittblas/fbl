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


    public:


        Disk();     // constructor, set everything up
        ~Disk();    // destructor, clean everything up

        void saveMap();
        void loadMap();
        void exportMapLua();


};
