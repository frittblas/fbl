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

#pragma once

#include <fstream>

#include "ScenEdit.hpp"


class Disk {

    private:
        static Disk instance;

        Disk();     // private constructor, so we can't instantiate

    public:
        ~Disk();

        static Disk &getInstance();

        static bool saveMap(ScenEdit& editor, std::string filename);
        static bool loadMap(ScenEdit& editor, std::string filename);    // this calls resetMap, which deletas all sprites!
        static bool exportMapLua(ScenEdit& editor, std::string filename);

};