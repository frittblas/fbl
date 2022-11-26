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
#include "../Weather.hpp"
#include "RobotCollection.hpp"

// id's for the collection-menu items
int fMenuBgSquareId, fMenuBgOutlineId;
int fMenuDividerLine;
int fMenuRobotDescr, fMenuItemsDescr;
int fMenuButtonLeft, fMenuButtonRight;


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

	g.mWeather->tick();

	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49

	if (num == 0)
		std::cout << "Tick RobotCollection!" << std::endl;

}


void initCollectionMenu() {

	// set position and size of the text area
	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2;
	int width = 400;
	int height = 200;

	// create gray menu area
	/*
	fMenuBgSquareId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, true);
	fbl_set_prim_color(fMenuBgSquareId, 50, 50, 50, 220);
	fbl_fix_prim_to_screen(fMenuBgSquareId, true);
	*/

	fMenuBgSquareId = fbl_create_sprite(32, 480, 20, 10, 0);
	fbl_set_sprite_xy(fMenuBgSquareId, x - width, y - height);
	fbl_set_sprite_scale(fMenuBgSquareId, 40);
	fbl_fix_sprite_to_screen(fMenuBgSquareId, true);

	// create the white outline
	fMenuBgOutlineId = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, false);
	fbl_set_prim_color(fMenuBgOutlineId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fMenuBgOutlineId, true);

	// create the white divider line
	fMenuDividerLine = fbl_create_prim(FBL_LINE, x, y - height, x, y + height, 0, false, false);
	fbl_set_prim_color(fMenuDividerLine, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fMenuDividerLine, true);


	fbl_load_ttf_font("font/garamond.ttf", 20);

	// The "Robot" and "Items" text at the top
	fMenuRobotDescr = fbl_create_text(255, 255, 255, 0, (char*)"Robot");
	fbl_set_text_align(fMenuRobotDescr, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuRobotDescr, x + 200, y - 170);
	fMenuItemsDescr = fbl_create_text(255, 255, 255, 0, (char*)"Items");
	fbl_set_text_align(fMenuItemsDescr, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuItemsDescr, x - 200, y - 170);


	// next/previous robot arrows
	fMenuButtonLeft = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 32, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonLeft, x + 50, y - 130);
	fMenuButtonRight = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonRight, x + 350, y - 130);


	// hide
	hideCollectionMenu();

}

void showCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, true);
	fbl_set_prim_active(fMenuBgOutlineId, true);

	fbl_set_prim_active(fMenuDividerLine, true);

	fbl_set_text_active(fMenuRobotDescr, true);
	fbl_set_text_active(fMenuItemsDescr, true);

	fbl_set_ui_elem_active(fMenuButtonLeft, true);
	fbl_set_ui_elem_active(fMenuButtonRight, true);

}

void hideCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, false);
	fbl_set_prim_active(fMenuBgOutlineId, false);

	fbl_set_prim_active(fMenuDividerLine, false);

	fbl_set_text_active(fMenuRobotDescr, false);
	fbl_set_text_active(fMenuItemsDescr, false);

	fbl_set_ui_elem_active(fMenuButtonLeft, false);
	fbl_set_ui_elem_active(fMenuButtonRight, false);

}
