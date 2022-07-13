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
    bool kinematic, animated;
    uint32_t animFrames;

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
        uint32_t mapWidth = 45; // 30
        uint32_t mapHeight = 25; // 17
        uint32_t drawTileX = 0;
        uint32_t drawTileY = 0;
        uint32_t mapWtextId, mapHtextId;
        uint32_t animFramesTextId;
        uint32_t bgRectId, drawTileId, mapMarkerId;
        uint32_t mapMarkerX = 0;
        uint32_t mapMarkerY = 0;

        const int tileSize = 32;
        const int spdFast = 5;
        const int spdMed = 10;
        const int spdSlow = 20;

        ScenEdit();     // constructor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void tick();
        void getInput();
        void processMouse(int button);
        void addTile();
        void removeTile();
        void resetMap();
        void snapToGrid(uint32_t &x, uint32_t &y);
        void toggleGUI();

};
