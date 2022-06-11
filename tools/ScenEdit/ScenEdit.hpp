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



class ScenEdit {

    private:
        uint32_t numTiles = 0;
        bool showGUI = true;
        int keyAccess = 10;
        std::vector<int> guiId; // all the id's for the Gui elements.


    public:
        uint32_t bgRectId;
        uint32_t drawTileId;
        uint32_t drawTileX = 0;
        uint32_t drawTileY = 0;
        uint32_t mapMarkerId;
        int mapMarkerX = 0;
        int mapMarkerY = 0;

        const int tileSize = 32;
        const int spdFast = 5;
        const int spdMed = 10;

        ScenEdit();     // conscturctor, set everything up
        ~ScenEdit();    // destructor, clean everything up

        void tick();
        void getInput();
        void snapToGrid(int &x, int &y);
        void toggleGUI();

};
