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
#include "Title.hpp"

int lotrTitleId, authorTextId;

// Title-class implementation

Title::Title() {

	fbl_load_ttf_font("anirm.ttf", 48);
	lotrTitleId = fbl_create_text(212, 175, 55, 255, "Lord of the rings");
	fbl_set_text_xy(lotrTitleId, 250, 100);

	fbl_load_ttf_font("anirm.ttf", 16);
	authorTextId = fbl_create_text(212, 175, 55, 255, "Hans Stromquist 2022");
	fbl_set_text_xy(authorTextId, 10, 520);

	std::cout << "Alloc Title!" << std::endl;

}

Title::~Title() {

	fbl_delete_text(lotrTitleId);
	fbl_delete_text(authorTextId);

	std::cout << "Destroy Title!" << std::endl;

}


// implement the virtual tick() function
void Title::tick() {

	int num = std::rand() / ((RAND_MAX + 1u) / 20); // random numbers from 0-19

	if (num == 0)
		std::cout << "Tick title!" << std::endl;

}
