/*
*
*	FBL ENGINE
*
*	GuiFuncs.hpp
*
*	The fbl Gui-elements call functions through pointers when clicked.
*   These are the functions, header file
*
*	Hans Strömquist 2022
*
*/

#pragma once

int getIndexAtCursor();

int selectSpriteLeft(int x, int y);
int selectSpriteRight(int x, int y);
int selectSpriteUp(int x, int y);
int selectSpriteDown(int x, int y);
int incMapX(int x, int y);
int incMapY(int x, int y);
int incLayer(int x, int y);
int decLayer(int x, int y);
int incType(int x, int y);
int decType(int x, int y);
int toggleAnimation(int x, int y);
int incAnimFrames(int x, int y);
int decAnimFrames(int x, int y);
int incAnimSpeed(int x, int y);
int decAnimSpeed(int x, int y);

int saveMap(int x, int y);
int loadMap(int x, int y);
int loadMapFbl(int x, int y);
