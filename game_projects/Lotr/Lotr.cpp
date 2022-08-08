/*
*
*	Lotr
*
*	Lotr.cpp
*
*	Entry point and game loop, contains the 3 required fbl functions start() loop() and end().
*	
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "Game.hpp"

Game* gGame;	// the Game-instance, only used in this file (file scope)

void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

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
