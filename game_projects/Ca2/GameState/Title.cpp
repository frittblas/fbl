/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Title.cpp
*
*   Title class implementation, takes care of the title screen.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

#include "../../../src/fbl.hpp"
#include "../Game.hpp"
#include "Title.hpp"

int titleId, subTitleId, authorTextId, websiteTextId;	// not really needed!

// Title-class implementation

Title::Title() {

	fbl_load_ttf_font("font/garamond.ttf", 48);
	titleId = fbl_create_text(212, 175, 55, 255, (char*)"Charming Alarming 2");
	fbl_set_text_xy(titleId, Game::DeviceResW / 2, 100);
	fbl_set_text_align(titleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("font/garamond.ttf", 27);
	subTitleId = fbl_create_text(154, 33, 34, 255, (char*)"Reasonable Robots");
	fbl_set_text_xy(subTitleId, Game::DeviceResW / 2, 200);
	fbl_set_text_align(subTitleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("font/garamond.ttf", 17);
	authorTextId = fbl_create_text(212, 175, 55, 255, (char*)"Hans Stromquist 2022");
	fbl_set_text_xy(authorTextId, 10, Game::DeviceResH - 20);

	websiteTextId = fbl_create_text(212, 175, 55, 255, (char*)"www.frittblas.se");
	fbl_set_text_xy(websiteTextId, Game::DeviceResW - 140, Game::DeviceResH - 20);

	std::cout << "Started Title state." << std::endl;


}

Title::~Title() {

	/*
	fbl_delete_text(titleId);
	fbl_delete_text(subTitleId);
	fbl_delete_text(authorTextId);
	fbl_delete_text(websiteTextId);
	*/

	fbl_destroy_all_text_objects();

	std::cout << "Destroyed Title state." << std::endl;

}


// implement the virtual tick() function
void Title::tick(Game& g) {


	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49
	if (num == 0)
		std::cout << "Tick title!" << std::endl;

}
