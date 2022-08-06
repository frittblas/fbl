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

Init* gInit;

void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	gInit = new Init();

}

void fbl_game_loop()
{

	gInit->update();

}

void fbl_end()
{

	gInit->unInitLotr();
	//std::cout<<"Bye!"<<std::endl;

}
