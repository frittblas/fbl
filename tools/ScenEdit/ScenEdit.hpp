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

struct TileData {

    uint32_t id;
    uint32_t x, y;
    uint32_t texture_x, texture_y;
    uint32_t solid;
    uint32_t animated;

};

class ScenEdit {

    private:
        uint32_t numTiles = 0;
        uint8_t bgColorR, bgColorG, bgColorB;
        bool showGUI = true;
        int keyAccess = 10;
        std::vector<int> guiId; // all the id's for the Gui elements.
        std::vector<TileData*> tile;


    public:
        uint32_t mapWidth = 30;
        uint32_t mapHeight = 16;
        uint32_t bgRectId;
        uint32_t drawTileId;
        uint32_t drawTileX = 0;
        uint32_t drawTileY = 0;
        uint32_t mapSizeXTextId;
        uint32_t mapSizeYTextId;
        uint32_t mapMarkerId;
        int mapMarkerX = 0;
        int mapMarkerY = 0;

        const int tileSize = 32;
        const int spdFast = 5;
        const int spdMed = 10;
        const int spdSlow = 20;

        ScenEdit();     // constructor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void tick();
        void getInput();
        void snapToGrid(int &x, int &y);
        void toggleGUI();

};
