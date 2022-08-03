/*
*
*	FBL ENGINE
*
*	Disk.cpp
*
*	Save and Load functionality for the tile editor for fbl
*
*	Hans Strömquist 2022
*
*/


#include <SDL.h>    // for RWops (export map to binary format)
#include "Disk.hpp"

Disk Disk::instance;

// Disk-class implementation

Disk::Disk() {}
Disk::~Disk() {}

Disk &Disk::getInstance() {

    return instance;

}

bool Disk::saveMap(ScenEdit& editor, std::string filename) {

    std::ofstream outFile;

    outFile.open(filename, std::ios::out); // out = overwrite every time

    if (outFile.fail()) {
        return false;
    }

    // first write the header "ScenEditMap"
    outFile << "ScenEditMap" << std::endl;
    std::cout << "ScenEditMap" << std::endl;    // echo to the screen

    // then write mapW and mapH and tile size
    outFile << editor.mapWidth << " " << editor.mapHeight << " " << editor.tileSize << std::endl;
    std::cout << editor.mapWidth << " " << editor.mapHeight << " " << editor.tileSize << std::endl;

    // then, bg color (rgb)
    outFile << (uint32_t)editor.bgColorR << " " << (uint32_t)editor.bgColorG << " " << (uint32_t)editor.bgColorB << std::endl;
    std::cout << (uint32_t)editor.bgColorR << " " << (uint32_t)editor.bgColorG << " " << (uint32_t)editor.bgColorB << std::endl;

    // then, night time tint color (rgba)
    outFile << (uint32_t)editor.tintColorR << " " << (uint32_t)editor.tintColorG << " " << (uint32_t)editor.tintColorB << " " << (uint32_t)editor.tintColorOn << std::endl;
    std::cout << (uint32_t)editor.tintColorR << " " << (uint32_t)editor.tintColorG << " " << (uint32_t)editor.tintColorB << " " << (uint32_t)editor.tintColorOn << std::endl;

    // then write all the tile data from the tile-vector, one line at a time
    for (TileData* curTile : editor.tile) {

        if (curTile != nullptr) {
            
            outFile << curTile->x << " " << curTile->y << " " << curTile->textureX << " " << curTile->textureY << " " << curTile->layer
                << " " << curTile->kinematic << " " << curTile->animated << " " << curTile->animFrames << " " << curTile->animSpeed << std::endl;

            std::cout << curTile->x << " " << curTile->y << " " << curTile->textureX << " " << curTile->textureY << " " << curTile->layer
                << " " << curTile->kinematic << " " << curTile->animated << " " << curTile->animFrames << " " << curTile->animSpeed << std::endl;

        }

    }

    outFile.close();

    return true;

}

bool Disk::loadMap(ScenEdit& editor, std::string filename) {

    std::ifstream inFile;

    inFile.open(filename, std::ios::in);

    if (inFile.fail()) {
        return false;
    }

    // first read the header "ScenEditMap"
    std::string header;
    inFile >> header;
    if (header.compare("ScenEditMap") != 0)
    {
        std::cout << "Incorrect header for the map file!" << std::endl;
        return false;
    }

    // then read mapW and mapH and tile size
    inFile >> editor.mapWidth >> editor.mapHeight >> editor.tileSize;
    std::cout << editor.mapWidth << " " << editor.mapHeight << " " << editor.tileSize << std::endl;

    // resize the current map to the new values, also setting the new tile size if that changed
    editor.resetMap(editor.mapWidth, editor.mapHeight);

    // then, bg color (rgb)
    uint32_t r, g, b, on;

    inFile >> r >> g >> b;
    editor.bgColorR = (uint8_t)r;
    editor.bgColorG = (uint8_t)g;
    editor.bgColorB = (uint8_t)b;
    std::cout << (uint32_t)editor.bgColorR << " " << (uint32_t)editor.bgColorG << " " << (uint32_t)editor.bgColorB << std::endl;

    // then, night time tint color (rgba)
    inFile >> r >> g >> b >> on;
    editor.tintColorR = (uint8_t)r;
    editor.tintColorG = (uint8_t)g;
    editor.tintColorB = (uint8_t)b;
    editor.tintColorOn = (uint8_t)on;
    std::cout << (uint32_t)editor.tintColorR << " " << (uint32_t)editor.tintColorG << " " << (uint32_t)editor.tintColorB << " " << (uint32_t)editor.tintColorOn << std::endl;
    

    // then write all the tile data from the tile-vector to the tile settings, one line at a time
    while (inFile.peek() != EOF) {

        inFile >> editor.tileSettings.x >> editor.tileSettings.y >> editor.tileSettings.textureX >> editor.tileSettings.textureY
            >> editor.tileSettings.layer >> editor.tileSettings.kinematic >> editor.tileSettings.animated
            >> editor.tileSettings.animFrames >> editor.tileSettings.animSpeed;

        std::cout << editor.tileSettings.x << " " << editor.tileSettings.y << " " << editor.tileSettings.textureX << " " << editor.tileSettings.textureY << " "
            << editor.tileSettings.layer << " " << editor.tileSettings.kinematic << " " << editor.tileSettings.animated << " "
            << editor.tileSettings.animFrames << " " << editor.tileSettings.animSpeed << " " << std::endl;

        // and add the tile to the map
        editor.addTile();

    }

    inFile.close();

    // set bg color
    fbl_set_clear_color(editor.bgColorR, editor.bgColorG, editor.bgColorB, 255);

    // set night time tint if alpha higher than 0
    if (editor.tintColorOn > 0)
        fbl_set_lighting_tint(true, editor.tintColorR, editor.tintColorG, editor.tintColorB);

    // reset the cursor coordinates and texture_xy to 0, 0
    editor.tileSettings.x = 0;
    editor.tileSettings.y = 0;
    editor.tileSettings.textureX = 0;
    editor.tileSettings.textureY = 0;

    // show info on the up/left most tile if in stand alone mode
    if(editor.standAlone)
        editor.showTileInfo();

    return true;
}

bool Disk::exportMapBin(ScenEdit& editor, std::string filename) {

    // save map in binary format so it can be loaded by fbl_load_scenedit_map()
    // fbl_load_scenedit_map() will use RWops so it can load files on android and stuff.
    // NOTE: fix the TileData struct to only use uint32_t for convenience (no bools).
    // also remove kinematic and go with the terrain variable instead. 2 buttons
    // that change the terrain value.
    // included SDL.h to this file so we can use RWops to both save and load maps in binary.


    return true;
}
