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
#include "../UserInput.hpp"
#include "../Addons.hpp"
#include "../Robots.hpp"
#include "../Weather.hpp"
#include "GameState.hpp"
#include "RobotCollection.hpp"

const uint16_t fNumSlots = 6;	// number of passive and active slots
const uint16_t fNumLines = 17;	// lines for the grid to the left, 10x5 grid, 17 lines

// id's for the robot collection-menu
uint16_t fMenuBgSquareId, fMenuBgOutlineId, fMenuRobotBgSquareId;
uint16_t fMenuDividerLine, fSmallMenuDividerLine;
uint16_t fMenuButtonLeft, fMenuButtonRight;
uint16_t fMenuRobotDescr, fMenuAddonsDescr;
uint16_t fMenuName, fMenuLevel, fMenuXp, fMenuHp, fMenuSpeed;
uint16_t fMenuDiag, fMenuEnergy, fMenuWeight;
uint16_t fMenuSlotNr[fNumSlots], fMenuSlot[fNumSlots], fMenuSlotArrow[fNumSlots];
uint16_t fMenuActive, fMenuPassive, fMenuPassiveActive;
uint16_t fMenuAddonGrid[fNumLines];
uint16_t fMenuAddonInfoLine;
uint16_t fAddonName, fAddonLevel, fAddonRarity, fAddonPassive, fAddonEquipped, fAddonPrice;
uint16_t fSaveAndQuit, fSaveAndQuitText;

// the menu button (always visible when in a game), externed in Explore.cpp
uint16_t gRobotCollectionMenuButton;


// RobotCollection-class implementation
RobotCollection::RobotCollection() {

	mSelectedAddon = notSet;
	mCurrentRobotPage = 0;
	mKeyAccess = 0;
	mMouseDown = false;

	showCollectionMenu();

	// set all arrows to inactive
	for (int id : fMenuSlotArrow) fbl_set_sprite_active(id, false);

	std::cout << "Started RobotCollection state." << std::endl;

}

RobotCollection::~RobotCollection() {


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

void RobotCollection::updateAddonInfo(Game& g, bool empty) {


	// get s component
	if (mSelectedAddon != notSet && !empty) {
		auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);

		fbl_update_text(fAddonName, 255, 255, 255, 0, (char*)"Name: %s", add.name.c_str());
		fbl_update_text(fAddonLevel, 255, 255, 255, 0, (char*)"Level:  %d", add.level);
		fbl_update_text(fAddonRarity, 255, 255, 255, 0, (char*)"Rarity:  %d", add.rarity);
		fbl_update_text(fAddonPassive, 255, 255, 255, 0, (char*)"Type: %s", add.passive ? "Passive" : "Active");
		fbl_update_text(fAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:  %s", add.equippedBy != g.mAddons->Unassigned ? "Yes" : "No");
		fbl_update_text(fAddonPrice, 255, 255, 255, 0, (char*)"Price:  %d", add.price);

	}
	else if (empty) {	// erase all the info if empty is requested

		fbl_update_text(fAddonName, 255, 255, 255, 0, (char*)"Name:");
		fbl_update_text(fAddonLevel, 255, 255, 255, 0, (char*)"Level:");
		fbl_update_text(fAddonRarity, 255, 255, 255, 0, (char*)"Rarity:");
		fbl_update_text(fAddonPassive, 255, 255, 255, 0, (char*)"Type:");
		fbl_update_text(fAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:");
		fbl_update_text(fAddonPrice, 255, 255, 255, 0, (char*)"Price:");

	}

}

void RobotCollection::setFreeSlotsArrows(Game& g, bool empty) {

	// begin by setting all arrows to inactive
	for (int id : fMenuSlotArrow) fbl_set_sprite_active(id, false);

	if (!empty) {
		std::cout << "Set arrows" << std::endl;
		auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);
		auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[mCurrentRobotPage]);

		// take care of the passive slots (show an arrow if the selected addon is passive and the slot is free)
		if (add.passive && sta.passiveSlot[0] == notSet) fbl_set_sprite_active(fMenuSlotArrow[0], true);
		if (add.passive && sta.passiveSlot[1] == notSet) fbl_set_sprite_active(fMenuSlotArrow[1], true);

		// active slots
		for (int i = 0; i < 4; i++) {
			if (!add.passive && sta.activeSlot[i] == notSet) fbl_set_sprite_active(fMenuSlotArrow[i + 2], true);
		}

	}
	else if(empty)
		std::cout << "Set no arrows" << std::endl;

}

void RobotCollection::selectAddon(Game& g) {

	bool allOff = true;

	for (int i = 0; i < g.mAddons->NumAddons; i++) {	// loop through all buttons and check if one was pressed

		if (g.mAddons->mOwnedAddons[i] != g.mAddons->Unassigned) {	// only bother with addons you actually own

			auto& add = g.mEcs->GetComponent<Addon>(g.mAddons->mOwnedAddons[i]);	// get the component of each button

			if (fbl_get_ui_elem_val(add.uiId) > 0) {	// if the current button was pressed..

				allOff = false;		// this is not true anymore bc a button is pressed

				if (mSelectedAddon != g.mAddons->mOwnedAddons[i]) {		// do the following only if the pressed button is a new one

					// turn off the last pressed button in favor for the new one.
					if (mSelectedAddon != notSet) {
						auto& add2 = g.mEcs->GetComponent<Addon>(mSelectedAddon);
						fbl_set_ui_elem_val(add2.uiId, 0);
					}

					mSelectedAddon = g.mAddons->mOwnedAddons[i];		// set the selectedAddon to the current entity

					updateAddonInfo(g, false);
					setFreeSlotsArrows(g, false);
					std::cout << "Updated addon info." << std::endl;
					std::cout << "Selected: " << mSelectedAddon << ", i: " << g.mAddons->mOwnedAddons[i] << std::endl;

				}


			}

		}

	}

	if (allOff && mSelectedAddon != notSet) {	// if all buttons are off, show empty info
		updateAddonInfo(g, true);
		setFreeSlotsArrows(g, true);
		mSelectedAddon = notSet;
		std::cout << "All off." << std::endl;
	}

}

void RobotCollection::equipAddon(Game& g) {

	int slotX, slotY, mouseX, mouseY;

	if (fbl_get_mouse_click(FBLMB_LEFT)) mMouseDown = true;
	if (fbl_get_mouse_release(FBLMB_LEFT) && mMouseDown && mKeyAccess == 0) {	// first check if user clicked

		mKeyAccess = g.mInput->buttonDelay / 2;
		mMouseDown = false;

		mouseX = fbl_get_mouse_logical_x();
		mouseY = fbl_get_mouse_logical_y();

		for (int i = 0; i < fNumSlots; i++) {	// loop through all slots

			// set up the clicking zones.
			slotX = fbl_get_prim_x(fMenuSlot[i]) - Game::TileSize / 2;
			slotY = fbl_get_prim_y(fMenuSlot[i]) - Game::TileSize / 2;

			if (mouseX > slotX && mouseY > slotY && mouseX < (slotX + Game::TileSize) && mouseY < (slotY + Game::TileSize)) {	// if click is inside a slot



				std::cout << "Before." << std::endl;
				// get Addon and Stats components from the selected addon and the current robot
				auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);
				auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[mCurrentRobotPage]);
				std::cout << "After" << std::endl;

				// just continue; in case of any error, check passive/active restrictions etc.
				if (i < 2 && !add.passive) continue;
				if (i > 1 && add.passive) continue;

				// NOTE: can't move to the same slot if equipped! will crash "retrieving non-existent component"

				add.equippedBy = g.mRobots->mOwnedRobots[mCurrentRobotPage]; // the addon is now equipped by this robot (Entity id)

				// check if this addon is just moved between equipped slots
				// first set passive and active slots with a value of mSelectedAddon to notSet

				for (int j = 0; j < fNumSlots; j++) {

					if (j < 2)
						if(sta.passiveSlot[j] == mSelectedAddon) sta.passiveSlot[j] = notSet;
					else if(j > 1){
						if(sta.activeSlot[j - 2] == mSelectedAddon) sta.activeSlot[j] = notSet;
					}

				}

				// then assign the passive or active slots
				if (i < 2)
					sta.passiveSlot[i] = mSelectedAddon;
				else {
					sta.activeSlot[i - 2] = mSelectedAddon;
				}
				
				g.mAddons->showAddonAsEquipped(g.mEcs, mSelectedAddon, i);	// move the addon to the correct slot
				setFreeSlotsArrows(g, false);
				
				break;

			}

		}

	}

	mKeyAccess--;
	if (mKeyAccess < 0) mKeyAccess = 0;

}

// deal with all the clicking on stats and buttons
void RobotCollection::processInput(Game& g) {

	if (fbl_get_ui_elem_val(fMenuButtonLeft)) {

		cyclePages(g, -1);	// cycle back

	}

	if (fbl_get_ui_elem_val(fMenuButtonRight)) {

		cyclePages(g, 1);	// cycle forward

	}

	selectAddon(g);
	equipAddon(g);

	// the almighty menu button (very top left)
	if (fbl_get_ui_elem_val(gRobotCollectionMenuButton) > 0)
		g.mState->change(g, GameState::StateType::Explore);


	// save and quit button down left
	if (fbl_get_ui_elem_val(fSaveAndQuit) > 0) {
		// Save game somehow :)
		g.mState->change(g, GameState::StateType::Title);
		g.mInput->access = g.mInput->buttonDelay * 2;	// 1 second delay
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
	fMenuBgSquareId = fbl_create_sprite(32, 480, 20, 10, 0);
	fbl_set_sprite_xy(fMenuBgSquareId, x - width, y - height);
	fbl_set_sprite_scale(fMenuBgSquareId, 40);
	fbl_set_sprite_layer(fMenuBgSquareId, 4);
	fbl_fix_sprite_to_screen(fMenuBgSquareId, true);


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


	fbl_load_ttf_font("font/roboto.ttf", 20);

	// The "Robot" and "Addons" text at the top
	fMenuRobotDescr = fbl_create_text(255, 255, 255, 0, (char*)"Robot");
	fbl_set_text_align(fMenuRobotDescr, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuRobotDescr, x + 200, y - 170);
	fMenuAddonsDescr = fbl_create_text(255, 255, 255, 0, (char*)"Addons");
	fbl_set_text_align(fMenuAddonsDescr, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuAddonsDescr, x - 200, y - 170);


	fMenuName = fbl_create_text(255, 255, 255, 0, (char*)"Default name");
	fbl_set_text_align(fMenuName, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuName, x + 200, y - 80);

	fbl_load_ttf_font("font/roboto.ttf", 18);

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
	fMenuSlotNr[0] = fbl_create_text(255, 255, 255, 0, (char*)"P1");
	fbl_set_text_align(fMenuSlotNr[0], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[0], x + 42, y - 50);
	fMenuSlotNr[1] = fbl_create_text(255, 255, 255, 0, (char*)"P2");
	fbl_set_text_align(fMenuSlotNr[1], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[1], x + 342, y - 50);

	fMenuSlotNr[2] = fbl_create_text(255, 255, 255, 0, (char*)"A1");
	fbl_set_text_align(fMenuSlotNr[2], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[2], x + 42, y + 35);
	fMenuSlotNr[3] = fbl_create_text(255, 255, 255, 0, (char*)"A2");
	fbl_set_text_align(fMenuSlotNr[3], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[3], x + 342, y + 35);

	fMenuSlotNr[4] = fbl_create_text(255, 255, 255, 0, (char*)"A3");
	fbl_set_text_align(fMenuSlotNr[4], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[4], x + 42, y + 120);
	fMenuSlotNr[5] = fbl_create_text(255, 255, 255, 0, (char*)"A4");
	fbl_set_text_align(fMenuSlotNr[5], FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuSlotNr[5], x + 342, y + 120);

	// slots
	fMenuSlot[0] = fbl_create_prim(FBL_RECT, x + 50, y - 20, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[0], 0, 83, 255, 255);
	fbl_fix_prim_to_screen(fMenuSlot[0], true);
	fMenuSlot[1] = fbl_create_prim(FBL_RECT, x + 350, y - 20, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[1], 0, 83, 255, 255);
	fbl_fix_prim_to_screen(fMenuSlot[1], true);

	fMenuSlot[2] = fbl_create_prim(FBL_RECT, x + 50, y + 65, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[2], 255, 106, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[2], true);
	fMenuSlot[3] = fbl_create_prim(FBL_RECT, x + 350, y + 65, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[3], 255, 106, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[3], true);

	fMenuSlot[4] = fbl_create_prim(FBL_RECT, x + 50, y + 150, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[4], 255, 106, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[4], true);
	fMenuSlot[5] = fbl_create_prim(FBL_RECT, x + 350, y + 150, 15, 15, 0, false, false);
	fbl_set_prim_color(fMenuSlot[5], 255, 106, 0, 255);
	fbl_fix_prim_to_screen(fMenuSlot[5], true);


	// the animated arrows that point to available slots
	for (int i = 0; i < fNumSlots; i++) {
		fMenuSlotArrow[i] = fbl_create_sprite(256, 288, 16, 16, 0);
		fbl_set_sprite_xy(fMenuSlotArrow[i], x + 10, y - 27);
		fbl_set_sprite_layer(fMenuSlotArrow[i], 5);
		fbl_set_sprite_animation(fMenuSlotArrow[i], true, 256, 288, 16, 16, 4, 7, true);
		fbl_fix_sprite_to_screen(fMenuSlotArrow[i], true);
		if(i % 2 != 0)
			fbl_set_sprite_flip(fMenuSlotArrow[i], 1); // 1 = horizontal
	}

	fbl_set_sprite_xy(fMenuSlotArrow[0], x + 10, y - 27);
	fbl_set_sprite_xy(fMenuSlotArrow[1], x + 375, y - 27);
	fbl_set_sprite_xy(fMenuSlotArrow[2], x + 10, y + 58);
	fbl_set_sprite_xy(fMenuSlotArrow[3], x + 375, y + 58);
	fbl_set_sprite_xy(fMenuSlotArrow[4], x + 10, y + 143);
	fbl_set_sprite_xy(fMenuSlotArrow[5], x + 375, y + 143);

	// active/passive instr
	fMenuPassive = fbl_create_text(0, 83, 255, 0, (char*)"P = passive");
	fbl_set_text_align(fMenuPassive, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuPassive, x + 155, y + 144);
	fMenuActive = fbl_create_text(255, 106, 0, 0, (char*)"A = active");
	fbl_set_text_align(fMenuActive, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuActive, x + 155, y + 166);

	// rect around passive/active text
	fMenuPassiveActive = fbl_create_prim(FBL_RECT, x + 200, y + 155, 50, 32, 0, false, false);
	fbl_set_prim_color(fMenuPassiveActive, 150, 150, 150, 255);
	fbl_fix_prim_to_screen(fMenuPassiveActive, true);

	// next/previous robot arrows
	fMenuButtonLeft = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 32, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonLeft, x + 50, y - 125);
	fMenuButtonRight = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 128, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fMenuButtonRight, x + 350, y - 125);


	// the grid to the left 10x5, 17 lines (11 + 6)
	x = 105;
	y = 132;
	for (int i = 0; i < fNumLines - 6; i++) {
		fMenuAddonGrid[i] = fbl_create_prim(FBL_LINE, x, y, x, y + 180, 0, false, false);
		fbl_set_prim_color(fMenuAddonGrid[i], 150, 150, 150, 255);
		fbl_fix_prim_to_screen(fMenuAddonGrid[i], true);
		x += 35;
	}
	x = 105;
	y = 132;
	for (int i = fNumLines - 6; i < fNumLines; i++) {
		fMenuAddonGrid[i] = fbl_create_prim(FBL_LINE, x, y, x + 350, y, 0, false, false);
		fbl_set_prim_color(fMenuAddonGrid[i], 150, 150, 150, 255);
		fbl_fix_prim_to_screen(fMenuAddonGrid[i], true);
		y += 36;
	}

	// line separator down left
	fMenuAddonInfoLine = fbl_create_prim(FBL_LINE, 80, 340, 480, 340, 0, false, false);
	fbl_set_prim_color(fMenuAddonInfoLine, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fMenuAddonInfoLine, true);

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


	// the menu button
	gRobotCollectionMenuButton = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 128, 64, 32, NULL);
	fbl_set_ui_elem_xy(gRobotCollectionMenuButton, 40, 24);	// top left corner 8 px in

	// save and quit to menu
	fSaveAndQuit = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fSaveAndQuit, 123, 500);	// down left-ish

	fSaveAndQuitText = fbl_create_text(255, 255, 255, 0, (char*)"Save and Quit");
	fbl_set_text_align(fSaveAndQuitText, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fSaveAndQuitText, 155, 500);

	// hide
	hideCollectionMenu();

}

void showCollectionMenu() {

	// gray bg, white robot bg, and outline
	fbl_set_sprite_active(fMenuBgSquareId, true);
	fbl_set_sprite_active(fMenuRobotBgSquareId, true);
	fbl_set_prim_active(fMenuBgOutlineId, true);

	fbl_set_prim_active(fMenuDividerLine, true);

	// robot and addons top text
	fbl_set_text_active(fMenuRobotDescr, true);
	fbl_set_text_active(fMenuAddonsDescr, true);

	// robot stats
	fbl_set_text_active(fMenuName, true);
	fbl_set_text_active(fMenuLevel, true);
	//fbl_set_text_active(fMenuXp, true);
	fbl_set_text_active(fMenuHp, true);
	fbl_set_text_active(fMenuSpeed, true);
	fbl_set_text_active(fMenuDiag, true);
	fbl_set_text_active(fMenuEnergy, true);
	//fbl_set_text_active(fMenuWeight, true);

	for (int i = 0; i < fNumSlots; i++) {
		fbl_set_text_active(fMenuSlotNr[i], true);
		fbl_set_prim_active(fMenuSlot[i], true);
		fbl_set_sprite_active(fMenuSlotArrow[i], true);
	}


	// active/passive intr.
	fbl_set_text_active(fMenuActive, true);
	fbl_set_text_active(fMenuPassive, true);
	fbl_set_prim_active(fMenuPassiveActive, true);

	// left right arrows
	fbl_set_ui_elem_active(fMenuButtonLeft, true);
	fbl_set_ui_elem_active(fMenuButtonRight, true);

	// grid
	for (int i = 0; i < fNumLines; i++) {
		fbl_set_prim_active(fMenuAddonGrid[i], true);
	}

	fbl_set_prim_active(fMenuAddonInfoLine, true);

	// addon stats
	fbl_set_text_active(fAddonName, true);
	fbl_set_text_active(fAddonLevel, true);
	fbl_set_text_active(fAddonRarity, true);
	fbl_set_text_active(fAddonPassive, true);
	fbl_set_text_active(fAddonEquipped, true);
	fbl_set_text_active(fAddonPrice, true);

	// save and quit
	fbl_set_ui_elem_active(fSaveAndQuit, true);
	fbl_set_text_active(fSaveAndQuitText, true);

}

void hideCollectionMenu() {

	// gray bg
	fbl_set_sprite_active(fMenuBgSquareId, false);
	fbl_set_sprite_active(fMenuRobotBgSquareId, false);
	fbl_set_prim_active(fMenuBgOutlineId, false);

	fbl_set_prim_active(fMenuDividerLine, false);

	// robot and addons top text
	fbl_set_text_active(fMenuRobotDescr, false);
	fbl_set_text_active(fMenuAddonsDescr, false);

	// robot stats
	fbl_set_text_active(fMenuName, false);
	fbl_set_text_active(fMenuLevel, false);
	//fbl_set_text_active(fMenuXp, false);
	fbl_set_text_active(fMenuHp, false);
	fbl_set_text_active(fMenuSpeed, false);
	fbl_set_text_active(fMenuDiag, false);
	fbl_set_text_active(fMenuEnergy, false);
	//fbl_set_text_active(fMenuWeight, false);

	for (int i = 0; i < fNumSlots; i++) {
		fbl_set_text_active(fMenuSlotNr[i], false);
		fbl_set_prim_active(fMenuSlot[i], false);
		fbl_set_sprite_active(fMenuSlotArrow[i], false);
	}

	// active/passive instr.
	fbl_set_text_active(fMenuActive, false);
	fbl_set_text_active(fMenuPassive, false);
	fbl_set_prim_active(fMenuPassiveActive, false);

	// left right arrows
	fbl_set_ui_elem_active(fMenuButtonLeft, false);
	fbl_set_ui_elem_active(fMenuButtonRight, false);

	// grid
	for (int i = 0; i < fNumLines; i++) {
		fbl_set_prim_active(fMenuAddonGrid[i], false);
	}

	// separator line
	fbl_set_prim_active(fMenuAddonInfoLine, false);

	// addon description
	fbl_set_text_active(fAddonName, false);
	fbl_set_text_active(fAddonLevel, false);
	fbl_set_text_active(fAddonRarity, false);
	fbl_set_text_active(fAddonPassive, false);
	fbl_set_text_active(fAddonEquipped, false);
	fbl_set_text_active(fAddonPrice, false);

	// save and quit
	fbl_set_ui_elem_active(fSaveAndQuit, false);
	fbl_set_text_active(fSaveAndQuitText, false);

}
