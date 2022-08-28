/*
*
*	FBL ENGINE
*
*	Disk.hpp
*
*	Save and Load functionality for the tile editor for fbl, header.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

#pragma once

class ScenEdit;

class Disk {

    private:
        static Disk instance;

        Disk();     // private constructor, so we can't instantiate

    public:
        ~Disk();

        static Disk &getInstance();

        static bool saveMap(ScenEdit& editor, std::string filename);    // save map in text format
        static bool loadMap(ScenEdit& editor, std::string filename);    // load text format map, this calls resetMap, which deletes all sprites!
        static bool loadMap_fbl(ScenEdit& editor, std::string filename);   // save map with RWops and with correct path on all platforms.

};
