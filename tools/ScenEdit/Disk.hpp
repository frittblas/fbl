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

        static bool saveMap(ScenEdit& editor, std::string filename);    // save map as in text format
        static bool loadMap(ScenEdit& editor, std::string filename);    // load text format map, this calls resetMap, which deletas all sprites!
        static bool exportMapBin(ScenEdit& editor, std::string filename); // save map in binary format, can be loaded by fbl_load_scenedit_map()

};
