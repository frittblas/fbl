/*
*
*	FBL ENGINE
*
*	ScenEdit.hpp
*
*	The tile editor for fbl, header
*
*	Hans Str�mquist 2022
*
*/


#include <iostream>
#include <vector>

#include "../../src/fbl.hpp"

#include "TileData.hpp"

class ScenEdit {

    private:
        uint32_t numTiles = 0;
        uint8_t bgColorR, bgColorG, bgColorB;
        uint8_t tintColorR, tintColorG, tintColorB, tintColorA;
        bool showGUI = true;
        int keyAccess = 10;
        std::vector<int> guiId; // all the id's for the Gui elements.


    public:
        uint32_t mapWidth = 45; // 30
        uint32_t mapHeight = 25; // 17
        uint32_t drawTileX = 0;
        uint32_t drawTileY = 0;
        uint32_t mapWtextId, mapHtextId;
        uint32_t layerTextId, animFramesTextId, animSpeedTextId;
        uint32_t bgRectId, drawTileId, mapMarkerId;
        uint32_t mapMarkerX = 0;
        uint32_t mapMarkerY = 0;

        std::vector<TileData*> tile;

        const int tileSize = 32;

        const int sceenWidthInTiles = 30;
        const int sceenHeightInTiles = 17;

        const int spdFast = 5;
        const int spdMed = 10;
        const int spdSlow = 25;

        ScenEdit();     // constructor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void tick();
        void getInput();
        void processMouse(int button);
        void addTile();
        void removeTile();
        void showTileInfo();
        void resetMap();
        void snapToGrid(uint32_t &x, uint32_t &y);
        void toggleGUI();

};
