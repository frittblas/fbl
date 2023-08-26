/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	PostRace.hpp
*
*	PostRace class header, takes care of what happens after the race.
*	Either GAME OVER (if all your robots are dead) or eventual level up
*	and opportunity to buy one of 3 items (2 addons and a robot.)
*
*	Hans Strömquist 2023
*
*/


#include "../../../../src/fbl.hpp"
#include "../../Ecs/Ecs.hpp"
#include "../../Ecs/Components.hpp"
#include "../../Game.hpp"
#include "../../UserInput.hpp"
#include "../../../Ca2/GameState/GameState.hpp"
#include "../../Progress.hpp"
#include "../../Robots.hpp"
#include "../../Addons.hpp"
#include "Race.hpp"
#include "PostRace.hpp"



// PostRace-class implementation

PostRace::PostRace() {


	std::cout << "PostRace constructor." << std::endl;

}

PostRace::~PostRace() {


	std::cout << "PostRace destructor." << std::endl;

}

void PostRace::tick(Game& g) {
	/*
	static int col = 0;
	static int step = 5;
	fbl_set_sprite_color(fMenuBgSquareId, col, col, col);

	col += step;
	if (col > 254 || col < 1) step = -step; */


	static int col = 120;
	static int step = 2;
	if(Race::sRaceState == Race::RaceState::Dead) {
		fbl_set_lighting_tint(true, col, 120, 120);
		col += step;
		if (col > 253 || col < 120) step = -step;
	}

	// continue button down middle
	if (fbl_get_ui_elem_val(fContinue) > 0) {
		if (Race::sRaceState == Race::RaceState::Dead) {
			g.mState->change(g, GameState::StateType::Title);
			g.mInput->access = g.mInput->buttonDelay * 2;
		} else {
			g.mState->change(g, GameState::StateType::Explore);
			g.mInput->access = g.mInput->buttonDelay;
		}
	}

}

void PostRace::gameOver() {

	// game over text
	fbl_load_ttf_font("font/garamond.ttf", 48);
	int tmpId = fbl_create_text(255, 69, 0, 255, (char*)"GAME OVER");
	fbl_set_text_align(tmpId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(tmpId, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3);

	// Continue game button
	fbl_load_ttf_font("font/garamond.ttf", 20);
	fContinue = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fContinue, Game::DeviceResW / 2, 500);	// down middle

	fContinueText = fbl_create_text(255, 255, 255, 0, (char*)"Quit");
	fbl_set_text_align(fContinueText, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fContinueText, Game::DeviceResW / 2 + 32, 500);

}

void PostRace::updateItemInfo(Game& g, bool empty) {


	// get s component
	if (mSelectedAddon != notSet && !empty) {
		auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);

		fbl_update_text(fAddonName, 255, 255, 255, 0, (char*)"Name: %s", add.name.c_str());
		fbl_update_text(fAddonLevel, 255, 255, 255, 0, (char*)"Level:  %d", add.level);
		std::string rarity = "Common";
		if (add.rarity == 2) rarity = "Rare";
		if (add.rarity == 3) rarity = "Epic";
		fbl_update_text(fAddonRarity, 255, 255, 255, 0, (char*)"Rarity:  %s", rarity.c_str());
		fbl_update_text(fAddonPassive, 255, 255, 255, 0, (char*)"Type: %s", add.passive ? "Passive" : "Active");

		fbl_update_text(fAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:  %s", add.equippedBy != notSet ? "Yes" : "No");
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

void PostRace::initPostRaceMenu(Game& g) {

	// set position and size of the text area
	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2;
	int width = 384;
	int height = 203;

	fbl_load_ttf_font("font/roboto.ttf", 16);
	std::string msg = "";
	switch (Race::sRaceState) {
		case Race::RaceState::First:  msg = "Congratulations! You placed 1st! Buy something will ya!"; break;
		case Race::RaceState::Second: msg = "Very nice you placed 2nd! Please buy whatever you like."; break;
		case Race::RaceState::Third:  msg = "Hmm.. You placed 3rd. Not that great but please buy some stuff!"; break;
		case Race::RaceState::Fourth: msg = "That was not good, you placed last! Please buy something anyway :)"; break;
	}
	fContextHelp = fbl_create_text(255, 255, 255, 0, (char*)msg.c_str());
	fbl_set_text_align(fContextHelp, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fContextHelp, x, y - height - 20);

	// create the context help white divider line
	fContextLine = fbl_create_prim(FBL_LINE, x - width, y - height, x + width, y - height, 0, false, false);
	fbl_set_prim_color(fContextLine, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fContextLine, true);


	// create gray menu area
	fMenuBgSquareId = fbl_create_sprite(32, 480, 12, 7, 0);
	//fbl_set_sprite_color(fMenuBgSquareId, 0, 255, 0);
	fbl_set_sprite_xy(fMenuBgSquareId, x, y - 20);
	fbl_set_sprite_scale(fMenuBgSquareId, 64);
	fbl_set_sprite_layer(fMenuBgSquareId, 8);
	fbl_fix_sprite_to_screen(fMenuBgSquareId, true);


	// white robot-bg
	fMenuRobotBgSquareId = fbl_create_sprite(448, 192, 64, 64, 0);
	fbl_set_sprite_xy(fMenuRobotBgSquareId, x + 200, y - 123);
	fbl_set_sprite_layer(fMenuRobotBgSquareId, 5);
	fbl_fix_sprite_to_screen(fMenuRobotBgSquareId, true);
	fbl_set_sprite_is_light(fMenuRobotBgSquareId, true);


	// create the white outline
	fMenuBgOutlineId = fbl_create_prim(FBL_RECT, x, y - 20, width, height + 20, 0, false, false);
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
	fMenuAddonsDescr = fbl_create_text(255, 255, 255, 0, (char*)"Shop");
	fbl_set_text_align(fMenuAddonsDescr, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuAddonsDescr, x - 190, y - 170);

	// add the N/A sprites in the shop
	int tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, x - 244, y - 94);
	fbl_set_sprite_layer(tempId, 9);
	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, x - 194, y - 94);
	fbl_set_sprite_layer(tempId, 9);
	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, x - 144, y - 94);
	fbl_set_sprite_layer(tempId, 9);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, x - 194, y - 30);
	fbl_set_sprite_layer(tempId, 9);

	fMenuName = fbl_create_text(255, 255, 255, 0, (char*)"Charmy");
	fbl_set_text_align(fMenuName, FBL_ALIGN_CENTER);
	fbl_set_text_xy(fMenuName, x + 200, y - 80);

	fbl_load_ttf_font("font/roboto.ttf", 18);

	// stats
	fMenuLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:  %d  (xp: %d / %d)", 1, 0, 4);
	fbl_set_text_align(fMenuLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fMenuLevel, x + 140, y - 30);
	fMenuHp = fbl_create_text(255, 255, 255, 0, (char*)"Hp:  %d / %d", 10, 10);
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


	// line separator down left
	fMenuAddonInfoLine = fbl_create_prim(FBL_LINE, x - width, 340, 480, 340, 0, false, false);
	fbl_set_prim_color(fMenuAddonInfoLine, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(fMenuAddonInfoLine, true);

	// addon stats
	fAddonName = fbl_create_text(255, 255, 255, 0, (char*)"Name:");
	fbl_set_text_align(fAddonName, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonName, 115, 370);
	fAddonLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:");
	fbl_set_text_align(fAddonLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonLevel, 115, 405);
	fAddonRarity = fbl_create_text(255, 255, 255, 0, (char*)"Rarity:");
	fbl_set_text_align(fAddonRarity, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonRarity, 115, 440);
	fAddonPassive = fbl_create_text(255, 255, 255, 0, (char*)"Type:");
	fbl_set_text_align(fAddonPassive, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonPassive, 305, 370);
	fAddonEquipped = fbl_create_text(255, 255, 255, 0, (char*)"Equipped:");
	fbl_set_text_align(fAddonEquipped, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonEquipped, 305, 405);
	fAddonPrice = fbl_create_text(255, 255, 255, 0, (char*)"Price:");
	fbl_set_text_align(fAddonPrice, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fAddonPrice, 305, 440);

	// Continue game button
	fContinue = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(fContinue, x, 500);	// down middle

	fContinueText = fbl_create_text(255, 255, 255, 0, (char*)"->");
	fbl_set_text_align(fContinueText, FBL_ALIGN_LEFT);
	fbl_set_text_xy(fContinueText, x + 32, 500);

	// put the current racing robot in the circle :)
	g.mRobots->showRobotInMenu(g.mEcs, -1, g.mRobots->mRacingRobots[0]);
	auto& spr = g.mEcs->GetComponent<Sprite>(g.mRobots->mRacingRobots[0]);
	fbl_set_sprite_layer((int)spr.id[0], 9);
	fbl_sort_sprites(FBL_SORT_BY_LAYER);

	// populate the shop with 3 addons (or less if there's not that many left) and 50% chance to get a robot.
	switch (g.mProgress->mCompletedRaces) {

		case 1:
		case 2:
		case 3:

			break;
		case 4:
		case 5:
		case 6:

			break;
		case 7:
		case 8:
		case 9:

			break;
		case 10:
		case 11:
		case 12:

			break;

		default:
			break;


	}

}
