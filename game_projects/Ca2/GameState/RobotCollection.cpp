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
#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"
#include "../Game.hpp"
#include "../Robots.hpp"
#include "../Weather.hpp"
#include "RobotCollection.hpp"


// id's for the collection-menu items
int fMenuBgSquareId, fMenuBgOutlineId;
int fMenuDividerLine;
int fMenuButtonLeft, fMenuButtonRight;
int fMenuRobotDescr, fMenuItemsDescr;
int fMenuName, fMenuLevel, fMenuXp, fMenuHp, fMenuSpeed;
int fMenuDiag, fMenuEnergy, fMenuWeight;
int fMenuSlotNr[4], fMenuSlot[4];


// RobotCollection-class implementation

RobotCollection::RobotCollection() {


	mCurrentRobotPage = 0;
	showCollectionMenu();

	std::cout << "Started RobotCollection state." << std::endl;

}

RobotCollection::~RobotCollection() {

	hideCollectionMenu();	// NOTE: if you press ESC in the collection state, the set active fails. (is this up to date? (don't think so :)))

	std::cout << "Destroyed RobotCollection state." << std::endl;

}

void RobotCollection::cyclePages(Game& g, int dir) {

	// find next owned robot
	for (;;) {
		mCurrentRobotPage += dir;
		if (mCurrentRobotPage > g.mRobots->NumRobots - 1)
			mCurrentRobotPage = 0;
		if (mCurrentRobotPage < 0)
			mCurrentRobotPage = g.mRobots->NumRobots - 1;
		if (g.mRobots->mOwnedRobots[mCurrentRobotPage] != g.mRobots->Unassigned)
			break;
	}

	// get stats component
	auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[mCurrentRobotPage]);

	// update the page
	g.mRobots->hideRobots(g.mEcs);
	g.mRobots->showRobotInMenu(g.mEcs, mCurrentRobotPage);

	fbl_update_text(fMenuName, 255, 255, 255, 0, (char*)sta.name.c_str());
	fbl_update_text(fMenuLevel, 255, 255, 255, 0, (char*)"Level:  %d  (xp: %d / %d)", sta.level, sta.xp, sta.nextLv);
	fbl_update_text(fMenuHp, 255, 255, 255, 0, (char*)"Hp:  %d", sta.hp);
	fbl_update_text(fMenuSpeed, 255, 255, 255, 0, (char*)"Speed:  %d", sta.speed);
	fbl_update_text(fMenuDiag, 255, 255, 255, 0, (char*)"Diagonals:  %s", sta.diag ? "Yes" : "No");
	fbl_update_text(fMenuSpeed, 255, 255, 255, 0, (char*)"Energy:  %d / %d", sta.energy, sta.maxEnergy);

	std::cout << mCurrentRobotPage << std::endl;
	std::cout << sta.name << std::endl;

}

// deal with all the clicking on stats and buttons :)
void RobotCollection::processInput(Game& g) {

	if (fbl_get_ui_elem_val(fMenuButtonLeft)) {
	
		cyclePages(g, -1);

	}

	if (fbl_get_ui_elem_val(fMenuButtonRight)) {

		cyclePages(g, 1);

	}

}

// implement the virtual tick() function
void RobotCollection::tick(Game& g) {

	g.mWeather->tick();
	
	processInput(g);

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
	fbl_set_sprite_layer(fMenuBgSquareId, 4);
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


	fMenuName = fbl_create_text(255, 255, 255, 0, (char*)"Default name");
	fbl_set_text_align(fMenuName, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuName, x + 200, y - 80);

	fbl_load_ttf_font("font/garamond.ttf", 18);

	// stats
	fMenuLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:  %d  (xp: %d / %d)", 1, 0, 4);
	fbl_set_text_align(fMenuLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuLevel, x + 140, y - 30);
	fMenuHp = fbl_create_text(255, 255, 255, 0, (char*)"Hp:  %d", 10);
	fbl_set_text_align(fMenuHp, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuHp, x + 140, y);
	fMenuSpeed = fbl_create_text(255, 255, 255, 0, (char*)"Speed:  %d", 1);
	fbl_set_text_align(fMenuSpeed, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSpeed, x + 140, y + 30);
	fMenuDiag = fbl_create_text(255, 255, 255, 0, (char*)"Diagonals:  %s", "Yes");
	fbl_set_text_align(fMenuDiag, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuDiag, x + 140, y + 60);
	fMenuEnergy = fbl_create_text(255, 255, 255, 0, (char*)"Energy:  %d / %d", 5, 10);
	fbl_set_text_align(fMenuEnergy, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuEnergy, x + 140, y + 90);

	// slot numbers
	fMenuSlotNr[0] = fbl_create_text(255, 255, 255, 0, (char*)"1");
	fbl_set_text_align(fMenuSlotNr[0], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[0], x + 47, y - 50);
	fMenuSlotNr[1] = fbl_create_text(255, 255, 255, 0, (char*)"2");
	fbl_set_text_align(fMenuSlotNr[1], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[1], x + 347, y - 50);
	fMenuSlotNr[2] = fbl_create_text(255, 255, 255, 0, (char*)"3");
	fbl_set_text_align(fMenuSlotNr[2], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[2], x + 47, y + 80);
	fMenuSlotNr[3] = fbl_create_text(255, 255, 255, 0, (char*)"4");
	fbl_set_text_align(fMenuSlotNr[3], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[3], x + 347, y + 80);

	// slots
	fMenuSlot[0] = fbl_create_prim(FBL_RECT, x + 50, y - 20, 16, 16, 0, false, false);
	fbl_set_prim_color(fMenuSlot[0], 0, 255, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[0], true);
	fMenuSlot[1] = fbl_create_prim(FBL_RECT, x + 350, y - 20, 16, 16, 0, false, false);
	fbl_set_prim_color(fMenuSlot[1], 255, 0, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[1], true);
	fMenuSlot[2] = fbl_create_prim(FBL_RECT, x + 50, y + 110, 16, 16, 0, false, false);
	fbl_set_prim_color(fMenuSlot[2], 255, 0, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[2], true);
	fMenuSlot[3] = fbl_create_prim(FBL_RECT, x + 350, y + 110, 16, 16, 0, false, false);
	fbl_set_prim_color(fMenuSlot[3], 255, 0, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[3], true);

	// next/previous robot arrows
	fMenuButtonLeft = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 32, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonLeft, x + 50, y - 125);
	fMenuButtonRight = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonRight, x + 350, y - 125);


	// hide
	hideCollectionMenu();

}

void showCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, true);
	fbl_set_prim_active(fMenuBgOutlineId, true);

	fbl_set_prim_active(fMenuDividerLine, true);

	fbl_set_text_active(fMenuRobotDescr, true);
	fbl_set_text_active(fMenuItemsDescr, true);

	fbl_set_text_active(fMenuName, true);
	fbl_set_text_active(fMenuLevel, true);
	//fbl_set_text_active(fMenuXp, true);
	fbl_set_text_active(fMenuHp, true);
	fbl_set_text_active(fMenuSpeed, true);
	fbl_set_text_active(fMenuDiag, true);
	fbl_set_text_active(fMenuEnergy, true);
	//fbl_set_text_active(fMenuWeight, true);

	for (int i = 0; i < 4; i++) {
		fbl_set_text_active(fMenuSlotNr[i], true);
		fbl_set_prim_active(fMenuSlot[i], true);
	}

	fbl_set_ui_elem_active(fMenuButtonLeft, true);
	fbl_set_ui_elem_active(fMenuButtonRight, true);


}

void hideCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, false);
	fbl_set_prim_active(fMenuBgOutlineId, false);

	fbl_set_prim_active(fMenuDividerLine, false);

	fbl_set_text_active(fMenuRobotDescr, false);
	fbl_set_text_active(fMenuItemsDescr, false);

	fbl_set_text_active(fMenuName, false);
	fbl_set_text_active(fMenuLevel, false);
	//fbl_set_text_active(fMenuXp, false);
	fbl_set_text_active(fMenuHp, false);
	fbl_set_text_active(fMenuSpeed, false);
	fbl_set_text_active(fMenuDiag, false);
	fbl_set_text_active(fMenuEnergy, false);
	//fbl_set_text_active(fMenuWeight, false);

	for (int i = 0; i < 4; i++) {
		fbl_set_text_active(fMenuSlotNr[i], false);
		fbl_set_prim_active(fMenuSlot[i], false);
	}

	fbl_set_ui_elem_active(fMenuButtonLeft, false);
	fbl_set_ui_elem_active(fMenuButtonRight, false);

}
