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
#include "../Addons.hpp"
#include "../Robots.hpp"
#include "../Weather.hpp"
#include "RobotCollection.hpp"


// id's for the collection-menu items
int fMenuBgSquareId, fMenuBgSquareId2, fMenuBgOutlineId, fMenuRobotBgSquareId;
int fMenuDividerLine, fSmallMenuDividerLine;
int fMenuButtonLeft, fMenuButtonRight;
int fMenuRobotDescr, fMenuItemsDescr;
int fMenuName, fMenuLevel, fMenuXp, fMenuHp, fMenuSpeed;
int fMenuDiag, fMenuEnergy, fMenuWeight;
int fMenuSlotNr[4], fMenuSlot[4];
int fMenuItemGrid[17];	// 10x5 grid, 17 lines
int fMenuItemInfoLine;
int fAddonName, fAddonLevel, fAddonRarity, fAddonPassive, fAddonEquipped, fAddonPrice;

// currectly and last selected addon
int selectedAddon = -1;
int lastSelectedAddon = -1;


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

void RobotCollection::updateAddonInfo(Game& g) {


	// get s component
	if (selectedAddon != -1) {
		auto& add = g.mEcs->GetComponent<Addon>(selectedAddon);

		fbl_update_text(fAddonName, 255, 255, 255, 0, (char*)"Name: %s", add.name.c_str());
		fbl_update_text(fAddonLevel, 255, 255, 255, 0, (char*)"Level:  %d", add.level);
		fbl_update_text(fAddonRarity, 255, 255, 255, 0, (char*)"Rarity:  %d", add.rarity);
		fbl_update_text(fAddonPassive, 255, 255, 255, 0, (char*)"Type: %s", add.passive ? "Passive" : "Active");
		fbl_update_text(fAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:  %s", add.equipped ? "Yes" : "No");
		fbl_update_text(fAddonPrice, 255, 255, 255, 0, (char*)"Price:  %d", add.price);

	}

}

// deal with all the clicking on stats and buttons :)
void RobotCollection::processInput(Game& g) {

	if (fbl_get_ui_elem_val(fMenuButtonLeft)) {
	
		cyclePages(g, -1);	// cycle back

	}

	if (fbl_get_ui_elem_val(fMenuButtonRight)) {

		cyclePages(g, 1);	// cycle forward

	}


	// try drag & drop, maybe :)

	//auto& add = g.mEcs->GetComponent<Addon>(g.mAddons->mOwnedAddons[0]);

	for (int i = 0; i < g.mAddons->NumAddons; i++) {	// loop through all buttons every frame checking if on was pressed

		auto& add = g.mEcs->GetComponent<Addon>(g.mAddons->mOwnedAddons[i]);	// get the component of each button

		if (fbl_get_ui_elem_val(add.uiId) > 0) {	// if the current button was pressed..

			if (selectedAddon != g.mAddons->mOwnedAddons[i]) {	// do the following only if the pressed button is a new one

				selectedAddon = g.mAddons->mOwnedAddons[i];		// set the selectedAddon to the current entity

				if (lastSelectedAddon == -1)
					lastSelectedAddon = selectedAddon;
				else fbl_set_ui_elem_val(lastSelectedAddon, 0);

				updateAddonInfo(g);

			}

		}

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

	// tried with prim rect, can't see robots :(
	/*
	fMenuBgSquareId2 = fbl_create_prim(FBL_RECT, x, y, width, height, 0, false, true);
	fbl_set_prim_color(fMenuBgSquareId2, 50, 50, 50, 220);
	fbl_fix_prim_to_screen(fMenuBgSquareId2, true);
	*/

	// white robot-bg
	fMenuRobotBgSquareId = fbl_create_sprite(448, 192, 64, 64, 0);
	fbl_set_sprite_xy(fMenuRobotBgSquareId, x + 168, y - 155);
	fbl_set_sprite_layer(fMenuRobotBgSquareId, 5);
	fbl_fix_sprite_to_screen(fMenuRobotBgSquareId, true);
	fbl_set_sprite_is_light(fMenuRobotBgSquareId, true);

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


	// the grid to the left 10x5, 17 lines (11 + 6)
	x = 105;
	y = 132;
	for (int i = 0; i < 11; i++) {
		fMenuItemGrid[i] = fbl_create_prim(FBL_LINE, x, y, x, y + 180, 0, false, false);
		fbl_set_prim_color(fMenuItemGrid[i], 150, 150, 150, 255);
		x += 35;
	}
	x = 105;
	y = 132;
	for (int i = 11; i < 17; i++) {
		fMenuItemGrid[i] = fbl_create_prim(FBL_LINE, x, y, x + 350, y, 0, false, false);
		fbl_set_prim_color(fMenuItemGrid[i], 150, 150, 150, 255);
		y += 36;
	}

	fMenuItemInfoLine = fbl_create_prim(FBL_LINE, 80, 340, 480, 340, 0, false, false);
	fbl_set_prim_color(fMenuItemInfoLine, 255, 255, 255, 255);

	// addon stats
	fAddonName = fbl_create_text(255, 255, 255, 0, (char*)"Name:");
	fbl_set_text_align(fAddonName, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonName, 105, 370);
	fAddonLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:");
	fbl_set_text_align(fAddonLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonLevel, 105, 405);
	fAddonRarity = fbl_create_text(255, 255, 255, 0, (char*)"Rarity:");
	fbl_set_text_align(fAddonRarity, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonRarity, 105, 440);
	fAddonPassive = fbl_create_text(255, 255, 255, 0, (char*)"Type:");
	fbl_set_text_align(fAddonPassive, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonPassive, 300, 370);
	fAddonEquipped = fbl_create_text(255, 255, 255, 0, (char*)"Equipped:");
	fbl_set_text_align(fAddonEquipped, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonEquipped, 300, 405);
	fAddonPrice = fbl_create_text(255, 255, 255, 0, (char*)"Price:");
	fbl_set_text_align(fAddonPrice, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonPrice, 300, 440);


	// hide
	hideCollectionMenu();

}

void showCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, true);
	fbl_set_sprite_active(fMenuRobotBgSquareId, true);
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

	// grid
	for (int i = 0; i < 17; i++) {
		fbl_set_prim_active(fMenuItemGrid[i], true);
	}

	fbl_set_prim_active(fMenuItemInfoLine, true);

	// addon stats

	fbl_set_text_active(fAddonName, true);
	fbl_set_text_active(fAddonLevel, true);
	fbl_set_text_active(fAddonRarity, true);
	fbl_set_text_active(fAddonPassive, true);
	fbl_set_text_active(fAddonEquipped, true);
	fbl_set_text_active(fAddonPrice, true);


}

void hideCollectionMenu() {

	fbl_set_sprite_active(fMenuBgSquareId, false);
	fbl_set_sprite_active(fMenuRobotBgSquareId, false);
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

	// grid
	for (int i = 0; i < 17; i++) {
		fbl_set_prim_active(fMenuItemGrid[i], false);
	}

	fbl_set_prim_active(fMenuItemInfoLine, false);

	fbl_set_text_active(fAddonName, false);
	fbl_set_text_active(fAddonLevel, false);
	fbl_set_text_active(fAddonRarity, false);
	fbl_set_text_active(fAddonPassive, false);
	fbl_set_text_active(fAddonEquipped, false);
	fbl_set_text_active(fAddonPrice, false);

}
