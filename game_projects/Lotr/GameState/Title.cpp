/*
*
*	Lotr
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

int lotrTitleId, lotrSubTitleId, authorTextId;	// not really needed!

// Title-class implementation

Title::Title() {

	fbl_load_ttf_font("anirm.ttf", 48);
	lotrTitleId = fbl_create_text(212, 175, 55, 255, (char*)"Lord of the rings");
	fbl_set_text_xy(lotrTitleId, Game::LogicalResW / 2, 100);
	fbl_set_text_align(lotrTitleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("anirm.ttf", 26);
	lotrSubTitleId = fbl_create_text(154, 33, 34, 255, (char*)"a retarded journey");
	fbl_set_text_xy(lotrSubTitleId, Game::LogicalResW / 2, 200);
	fbl_set_text_align(lotrSubTitleId, FBL_ALIGN_CENTER);

	fbl_load_ttf_font("anirm.ttf", 16);
	authorTextId = fbl_create_text(212, 175, 55, 255, (char*)"Hans Stromquist 2022");
	fbl_set_text_xy(authorTextId, 10, 520);

	std::cout << "Started Title state." << std::endl;

}

Title::~Title() {

	/*
	fbl_delete_text(lotrTitleId);
	fbl_delete_text(authorTextId);
	fbl_delete_text(lotrSubTitleId);
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
