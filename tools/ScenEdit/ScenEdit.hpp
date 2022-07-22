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
        const int tileSize = 32;

        const int sceenWidthInTiles = 30;
        const int sceenHeightInTiles = 17;

        const int spdFast = 5;
        const int spdMed = 10;
        const int spdSlow = 25;

        uint32_t mapWidth = 45; // sceenWidthInTiles
        uint32_t mapHeight = 25; // sceenHeightInTiles
        uint32_t mapWtextId, mapHtextId;    // id's for text objects
        uint32_t layerTextId, animFramesTextId, animSpeedTextId;
        uint32_t kinematicBoxId, animatedBoxId;
        uint32_t bgRectId, mapMarkerId;

        std::vector<TileData*> tile;    // vector that holds all the tiles
        TileData tileSettings;    // holds the parameters for the current tile to be drawn

        ScenEdit();     // constructor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void tick();
        void getInput();
        void processMouse(int button);
        void addTile();
        void copyTile();
        void removeTile();
        void showTileInfo();
        void resetMap();
        void snapToGrid(uint32_t &x, uint32_t &y);
        void toggleGUI();

};
