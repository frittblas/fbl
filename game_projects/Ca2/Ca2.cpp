/*
*
*	Charming Alarming 2: Reasonable Robots
*
	Ca2.cpp
*
*	Entry point and game loop, contains the 3 required fbl functions start() loop() and end().
*	
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "Game.hpp"

Game* gGame;	// the Game-instance, only used in this file (file scope), aaand LuaDialogue.cpp sorry :)

void fbl_start()
{

	gGame = new Game();

}

void fbl_game_loop()
{

	gGame->update();

}

void fbl_end()
{

	delete gGame;

}
