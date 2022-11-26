/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	RobotCollection.cpp
*
*   RobotCollection class implementation, takes care of upgrading te robots etc.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>

#include "../../../src/fbl.hpp"
#include "../Game.hpp"
#include "RobotCollection.hpp"

// id's for the collection-menu items
int gMenuBgSquareId, gMenuBgOutlineId;
int gResponseYadaId;
int gButtonLeft, gButtonRight;


// RobotCollection-class implementation

RobotCollection::RobotCollection() {

	showCollectionMenu();

	std::cout << "Started RobotCollection state." << std::endl;

}

RobotCollection::~RobotCollection() {

	hideCollectionMenu();

	std::cout << "Destroyed RobotCollection state." << std::endl;

}


// implement the virtual tick() function
void RobotCollection::tick(Game& g) {

	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49

	if (num == 0)
		std::cout << "Tick RobotCollection!" << std::endl;

}


void initCollectionMenu() {

	// set position and size of the text area
	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2 + Game::DeviceResH / 4;
	int width = 250;
	int height = 100;

	// create wine red text area
	gMenuBgSquareId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, true);
	fbl_set_prim_color(gMenuBgSquareId, 200, 50, 50, 150);
	fbl_fix_prim_to_screen(gMenuBgSquareId, true);

	// create white outline
	gMenuBgOutlineId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, false);
	fbl_set_prim_color(gMenuBgOutlineId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(gMenuBgOutlineId, true);

	fbl_load_ttf_font("font/garamond.ttf", 22);


	// Yes-type response, or single "ok"
	gResponseYadaId = fbl_create_text(255, 255, 255, 0, (char*)"Yada");
	fbl_set_text_align(gResponseYadaId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(gResponseYadaId, x - 185, y + 64);


	// ui yes / no and Ok buttons
	gButtonLeft = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(gButtonLeft, x - 215, y + 64);
	gButtonRight = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(gButtonRight, x, y + 64);


	// hide
	hideCollectionMenu();

}

void showCollectionMenu() {

	fbl_set_prim_active(gMenuBgSquareId, true);
	fbl_set_prim_active(gMenuBgOutlineId, true);

	fbl_set_text_active(gResponseYadaId, true);

	fbl_set_ui_elem_active(gButtonLeft, true);
	fbl_set_ui_elem_active(gButtonRight, true);

}

void hideCollectionMenu() {

	fbl_set_prim_active(gMenuBgSquareId, false);
	fbl_set_prim_active(gMenuBgOutlineId, false);

	fbl_set_text_active(gResponseYadaId, false);

	fbl_set_ui_elem_active(gButtonLeft, false);
	fbl_set_ui_elem_active(gButtonRight, false);

}
