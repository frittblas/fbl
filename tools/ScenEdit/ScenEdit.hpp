/*
*
*	FBL ENGINE
*
*	ScenEdit.hpp
*
*	The tile editor for fbl, header
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>
#include <vector>
#include "../../src/fbl.hpp"
#include "TileData.hpp"

class ScenEdit {

    private:
        bool showGUI = true;
        int keyAccess = 10;
        std::vector<int> guiId; // all the id's for the Gui elements.

    public:

        // control when we can press button again
        const int spdFast = 5;
        const int spdMed = 10;
        const int spdSlow = 25;

        // these numbers will fill a screen with tiles (960x540)
        uint32_t screenWidthInTiles = 30;
        uint32_t screenHeightInTiles = 17;

        uint32_t mapWidth, mapHeight, tileSize;   // map wh and tile size
        uint8_t  bgColorR, bgColorG, bgColorB;   // bg color
        uint8_t  tintColorR, tintColorG, tintColorB, tintColorOn; // night time tint color
        uint32_t mapWtextId, mapHtextId;    // id's for text objects
        uint32_t layerTextId, animFramesTextId, animSpeedTextId;
        uint32_t kinematicBoxId, animatedBoxId; // checkbox id's
        uint32_t bgRectId, mapMarkerId; // gray alpha-filled rect, and the cursor

        std::vector<TileData*> tile;    // vector that holds all the tiles
        TileData tileSettings;    // holds the parameters for the current tile to be drawn

        ScenEdit();     // constructor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void setup(uint32_t mapW, uint32_t mapH, uint32_t tSize);
        void fitTilesToScreen();
        void tick();
        void getInput();
        void processMouse(int button);
        void addTile();
        void copyTile();
        void removeTile();
        void showTileInfo();
        void resetMap(uint32_t w, uint32_t h);
        void snapToGrid(uint32_t &x, uint32_t &y);
        void toggleGUI();

};
