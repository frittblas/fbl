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

#include "Disk.hpp"

Disk Disk::instance;

// Disk-class implementation

Disk::Disk() {}
Disk::~Disk() {}

Disk &Disk::getInstance() {

    return instance;

}

bool Disk::saveMap() {


    return true;
}

bool Disk::loadMap() {

    return true;
}

bool Disk::exportMapLua() {

    return true;
}