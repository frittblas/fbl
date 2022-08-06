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
#include "Init.hpp"
#include "GameState/GameState.hpp"
#include "UserInput.hpp"

extern UserInput gInput;
extern GameState* gState;

void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	Init::getInstance().initLotr();

}

void fbl_game_loop()
{

	gInput.tick();	// get user input
	gState->tick(); // update the current state

}

void fbl_end()
{

	Init::getInstance().unInitLotr();

	std::cout<<"Bye!"<<std::endl;

}
