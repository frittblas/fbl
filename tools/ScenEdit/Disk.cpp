/*
*
*	FBL ENGINE
*
*	Disk.cpp
*
*	Save and Load functionality for the tile editor for fbl.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

#include <string>
#include <fstream>

#include "ScenEdit.hpp"
#include "Disk.hpp"

extern "C" {
    char* engine_get_platform_asset_path(const char* file); // from fbl.c
    char* read_file_to_buf(const char* filename);   // from lua.c
}

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
    outFile << (uint32_t)editor.mapWidth << " " << (uint32_t)editor.mapHeight << " " << (uint32_t)editor.tileSize << std::endl;
    std::cout << (uint32_t)editor.mapWidth << " " << (uint32_t)editor.mapHeight << " " << (uint32_t)editor.tileSize << std::endl;

    // then, bg color (rgb)
    outFile << (uint32_t)editor.bgColorR << " " << (uint32_t)editor.bgColorG << " " << (uint32_t)editor.bgColorB << std::endl;
    std::cout << (uint32_t)editor.bgColorR << " " << (uint32_t)editor.bgColorG << " " << (uint32_t)editor.bgColorB << std::endl;

    // then, night time tint color (rgba)
    outFile << (uint32_t)editor.tintColorR << " " << (uint32_t)editor.tintColorG << " " << (uint32_t)editor.tintColorB << " " << (uint32_t)editor.tintColorOn << std::endl;
    std::cout << (uint32_t)editor.tintColorR << " " << (uint32_t)editor.tintColorG << " " << (uint32_t)editor.tintColorB << " " << (uint32_t)editor.tintColorOn << std::endl;

    // then write all the tile data from the tile-vector, one line at a time
    for (TileData* curTile : editor.tile) {

        if (curTile != nullptr) {

            // NOTE: cast these with uint32_t and test or maybe not
            
            outFile << curTile->x << " " << curTile->y << " " << curTile->textureX << " " << curTile->textureY << " " << curTile->layer
                << " " << curTile->type << " " << (uint32_t)curTile->animated << " " << curTile->animFrames << " " << curTile->animSpeed << std::endl;

            std::cout << curTile->x << " " << curTile->y << " " << curTile->textureX << " " << curTile->textureY << " " << curTile->layer
                << " " << curTile->type << " " << (uint32_t)curTile->animated << " " << curTile->animFrames << " " << curTile->animSpeed << std::endl;

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
    

    // then write all the tile data from the file to the tile settings, one line at a time
    while (inFile.peek() != EOF) {

        inFile >> editor.tileSettings.x >> editor.tileSettings.y >> editor.tileSettings.textureX >> editor.tileSettings.textureY
            >> editor.tileSettings.layer >> editor.tileSettings.type >> (bool)editor.tileSettings.animated
            >> editor.tileSettings.animFrames >> editor.tileSettings.animSpeed;

        std::cout << editor.tileSettings.x << " " << editor.tileSettings.y << " " << editor.tileSettings.textureX << " " << editor.tileSettings.textureY << " "
            << editor.tileSettings.layer << " " << editor.tileSettings.type << " " << (bool)editor.tileSettings.animated << " "
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



bool Disk::loadMap_fbl(ScenEdit& editor, std::string filename) {

    // get the correct fbl path
    char* new_path = engine_get_platform_asset_path(filename.c_str());

    // read the file to buf using RWops
    char* buf = read_file_to_buf(new_path);

    std::string buf_cpp(buf);

    // no need to free new_path, it's static
    free(buf);

    //std::cout << bufs << std::endl;  // this prints everything exacly as the text file

    std::string word = "";
    int num_words = 0;
    std::ofstream outFile;

    outFile.open("out.txt", std::ios::out); // out = overwrite every time

    if (outFile.fail()) {
        return false;
    }

    //for (auto x : buf_cpp)
    for (std::string::size_type i = 0; i < buf_cpp.size(); i++)
    {
        //if (x == '\r') x = ' ';
        //if (x == '\n') continue;
        //if (x == ' ')
        if (buf_cpp[i] == '\r') {
            buf_cpp[i] = ' ';
            if(buf_cpp[i + 1] == '\n')
                buf_cpp[i + 1] = ' ';
        }
        if (buf_cpp[i] == ' ' && buf_cpp[i - 1] != ' ')
        {

            //word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());

            // NOTE: cast with uint8_t on the color :)

            switch (num_words) {

                case 0 :
                    // first read the header "ScenEditMap"
                    if (word.compare("ScenEditMap") != 0)
                    {
                        std::cout << "Incorrect header for the map file!" << std::endl;
                        return false;
                    }
                    outFile << word << std::endl;
                    break;
                case 1:
                    editor.mapWidth = std::stoi(word);
                    outFile << (uint32_t)editor.mapWidth << " ";
                    break;
                case 2:
                    editor.mapHeight = std::stoi(word);
                    outFile << (uint32_t)editor.mapHeight << " ";
                    break;
                case 3:
                    editor.tileSize = std::stoi(word);
                    outFile << (uint32_t)editor.tileSize << std::endl;
                    // resize the current map to the new values, also setting the new tile size if that changed
                    editor.resetMap(editor.mapWidth, editor.mapHeight);
                    break;
                case 4:
                    editor.bgColorR = std::stoi(word);
                    outFile << (uint32_t)editor.bgColorR << " ";
                    break;
                case 5:
                    editor.bgColorG = std::stoi(word);
                    outFile << (uint32_t)editor.bgColorG << " ";
                    break;
                case 6:
                    editor.bgColorB = std::stoi(word);
                    outFile << (uint32_t)editor.bgColorB << std::endl;
                    break;
                case 7:
                    editor.tintColorR = std::stoi(word);
                    outFile << (uint32_t)editor.tintColorR << " ";
                    break;
                case 8:
                    editor.tintColorG = std::stoi(word);
                    outFile << (uint32_t)editor.tintColorG << " ";
                    break;
                case 9:
                    editor.tintColorB = std::stoi(word);
                    outFile << (uint32_t)editor.tintColorB << " ";
                    break;
                case 10:
                    editor.tintColorOn = std::stoi(word);
                    outFile << (uint32_t)editor.tintColorOn << std::endl;
                    break;

                // now the tile-data
                case 11:
                    editor.tileSettings.x = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.x << " ";
                    break;
                case 12:
                    editor.tileSettings.y = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.y << " ";
                    break;
                case 13:
                    editor.tileSettings.textureX = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.textureX << " ";
                    break;
                case 14:
                    editor.tileSettings.textureY = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.textureY << " ";
                    break;
                case 15:
                    editor.tileSettings.layer = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.layer << " ";
                    break;
                case 16:
                    editor.tileSettings.type = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.type << " ";
                    break;
                case 17:
                    editor.tileSettings.animated = (bool)std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.animated << " ";
                    break;
                case 18:
                    editor.tileSettings.animFrames = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.animFrames << " ";
                    break;
                case 19:
                    editor.tileSettings.animSpeed = std::stoi(word);
                    outFile << (uint32_t)editor.tileSettings.animSpeed << std::endl;

                    // add the tile
                    editor.addTile();
                    break;

            }

            word = "";
            num_words++;
            if (num_words > 19) num_words = 11; // repeat for all tiles
        }
        else {
            //word = word + x;
            word = word + buf_cpp[i];
        }
    }

    //outFile << word << std::endl;
    outFile.close();


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

    // NOTE: also set layer and type to 0 and stuff

    // show info on the up/left most tile if in stand alone mode
    if (editor.standAlone)
        editor.showTileInfo();

    return true;
}
