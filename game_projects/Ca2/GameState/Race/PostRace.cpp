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
#include "../../Chars.hpp"
#include "../../SoundManager.hpp"
#include "../../UserInput.hpp"
#include "../../../Ca2/GameState/GameState.hpp"
#include "../../Progress.hpp"
#include "../../Robots.hpp"
#include "../../Addons.hpp"
#include "Race.hpp"
#include "PostRace.hpp"


// PostRace-class implementation

PostRace::PostRace() {

	mSelectedAddon = notSet;
	mKeyAccess = 0;

	std::cout << "PostRace constructor." << std::endl;

}

PostRace::~PostRace() {


	std::cout << "PostRace destructor." << std::endl;

}

void PostRace::tick(Game& g) {
	static int col = 120;
	static int step = 2;
	if(Race::sRaceState == Race::RaceState::Dead) {
		fbl_set_lighting_tint(true, col, 120, 120);
		col += step;
		if (col > 253 || col < 120) step = -step;
	}
	else {
		// buy button
		if (fbl_get_ui_elem_val(mBuyButton) > 0)
			buySelectedItem(g);

		selectAddon(g);
	}

	// continue button down middle
	if (fbl_get_ui_elem_val(mContinueButton) > 0) {
		if (Race::sRaceState == Race::RaceState::Dead) {
			g.mState->change(g, GameState::StateType::Title);
			g.mInput->access = g.mInput->buttonDelay * 2;
		} else {
			g.mState->change(g, GameState::StateType::Explore);
			g.mChars->stopPlayerPathing(g);
			g.mProgress->mCompletedRaces++;	// successfully completed a race!

			// first check if player is dead, and assign new robot as fav
			auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[0]);
			if (sta.hp < 0.1) {
				// find your next available robot
				for (int i = 0; i < Robots::NumRobots; i++) {
					if (g.mRobots->mOwnedRobots[i] != g.mRobots->Unassigned) {
						auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[i]);
						if (sta.hp > 0.1) {
							g.mProgress->mFavRobot = i;
							std::cout << "NEW FAV::::::::::::::::::: " << sta.name << std::endl;
							break;
						}

					}
				}
			}


		}
	}


}

void PostRace::gameOver() {

	int tmpId = -1;

	// game over text
	fbl_load_ttf_font("font/garamond.ttf", 48);
	tmpId = fbl_create_text(255, 69, 0, 255, (char*)"GAME OVER");
	fbl_set_text_align(tmpId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(tmpId, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3);

	// Continue game button
	fbl_load_ttf_font("font/garamond.ttf", 20);
	mContinueButton = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(mContinueButton, Game::DeviceResW / 2, 500);	// down middle

	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Quit");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, Game::DeviceResW / 2 + 32, 500);

}

void PostRace::updateContextHelp(std::string msg) {

	fbl_load_ttf_font("font/roboto.ttf", 16);
	fbl_update_text(mContextHelp, 255, 255, 255, 255, (char*)msg.c_str());
	fbl_load_ttf_font("font/roboto.ttf", 18);
}

void PostRace::updateAddonInfo(Game& g, bool empty) {


	// get selected addon component
	if (mSelectedAddon != notSet && !empty) {
		auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);

		fbl_update_text(mAddonName, 255, 255, 255, 0, (char*)"Name: %s", add.name.c_str());
		fbl_update_text(mAddonLevel, 255, 255, 255, 0, (char*)"Level:  %d", add.level);
		std::string rarity = "Common";
		if (add.rarity == 2) rarity = "Rare";
		if (add.rarity == 3) rarity = "Epic";
		fbl_update_text(mAddonRarity, 255, 255, 255, 0, (char*)"Rarity:  %s", rarity.c_str());
		fbl_update_text(mAddonPassive, 255, 255, 255, 0, (char*)"Type: %s", add.passive ? "Passive" : "Active");

		fbl_update_text(mAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:  %s", add.equippedBy != notSet ? "Yes" : "No");
		fbl_update_text(mAddonPrice, 255, 255, 255, 0, (char*)"Price:  %d", add.price);

	}
	else if (empty) {	// erase all the info if empty is requested

		fbl_update_text(mAddonName, 255, 255, 255, 0, (char*)"Name:");
		fbl_update_text(mAddonLevel, 255, 255, 255, 0, (char*)"Level:");
		fbl_update_text(mAddonRarity, 255, 255, 255, 0, (char*)"Rarity:");
		fbl_update_text(mAddonPassive, 255, 255, 255, 0, (char*)"Type:");
		fbl_update_text(mAddonEquipped, 255, 255, 255, 0, (char*)"Equipped:");
		fbl_update_text(mAddonPrice, 255, 255, 255, 0, (char*)"Price:");

	}

}

void PostRace::selectAddon(Game& g) {

	bool allOff = true;

	for (int i = 0; i < g.mAddons->NumAddons; i++) {	// loop through all buttons and check if one was pressed

		if (g.mAddons->mShopAddons[i] == g.mAddons->Unassigned) continue;	// only bother with addons actually in the shop

		auto& add = g.mEcs->GetComponent<Addon>(g.mAddons->mShopAddons[i]);	// get the component of each button

		if (fbl_get_ui_elem_val(add.uiId) < 1) continue;	// if the current button was not pressed continue

		allOff = false;		// this is not true anymore bc a button is pressed

		if (mSelectedAddon == g.mAddons->mShopAddons[i]) continue;		// skip if the pressed button isn't a new one

		// turn off the last pressed button in favor for the new one.
		if (mSelectedAddon != notSet) {
			auto& add2 = g.mEcs->GetComponent<Addon>(mSelectedAddon);
			fbl_set_ui_elem_val(add2.uiId, 0);
		}

		mSelectedAddon = g.mAddons->mShopAddons[i];		// set the selectedAddon to the current entity


		updateAddonInfo(g, false);

		std::cout << "Updated addon info." << std::endl;
		std::cout << "Selected: " << mSelectedAddon << ", i: " << g.mAddons->mShopAddons[i] << std::endl;

	}

	if (allOff && mSelectedAddon != notSet) {	// if all buttons are off, show empty info
		updateAddonInfo(g, true);
		mSelectedAddon = notSet;
		std::cout << "All off." << std::endl;
	}

}

void PostRace::prepShop(Game& g) {

	// populate the mShopAddons array with up to 3 random addons from mAllAddons

	// init shop array to unassigned
	for (int i = 0; i < g.mAddons->NumAddons; i++)
		g.mAddons->mShopAddons[i] = g.mAddons->Unassigned;

	// populate the shop-addons array woth random addons from the all-addons array
	std::vector<int> availableIndices;
	// 1
	for (int i = 0; i < g.mAddons->NumAddons; ++i)
		if (g.mAddons->mAllAddons[i] != g.mAddons->Unassigned)
			availableIndices.push_back(i);
	// 2
	std::random_shuffle(availableIndices.begin(), availableIndices.end());
	int numCopies = std::min(3, static_cast<int>(availableIndices.size()));
	// 3
	for (int i = 0; i < numCopies; i++)
		g.mAddons->mShopAddons[availableIndices[i]] = g.mAddons->mAllAddons[availableIndices[i]];

	// position the addons correctly and activate ui elements
	g.mAddons->showAddonsInShop(g.mEcs);

}

void PostRace::buySelectedItem(Game& g) {

	// has to select an item
	if (mSelectedAddon == notSet) {
		updateContextHelp("Please select an item to buy!");
		return;
	}

	auto& add = g.mEcs->GetComponent<Addon>(mSelectedAddon);

	// has to afford
	if(add.price >  g.mProgress->mFunds) {
		updateContextHelp("You can't afford that!");
		return;
	}

	// all went well, claim the addon and remove it from the shop array, and deactivate the ui element
	g.mAddons->claimAddon(add.type);
	g.mAddons->mShopAddons[add.type] = g.mAddons->Unassigned;

	fbl_set_ui_elem_active(add.uiId, false);

	// nothing selected now
	mSelectedAddon = notSet;

	// pay
	g.mProgress->mFunds -= add.price;

	// update gui
	fbl_update_text(mFundsText, 255, 255, 255, 255, "Coins: %d", g.mProgress->mFunds);

	updateContextHelp("Thank you for your purchase!");

}

void PostRace::initPostRaceMenu(Game& g) {

	// set position and size of the text area
	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2;
	int width = 384;
	int height = 203;
	int tmpId = -1;		// used by text, sprite and prim-object that do not need to be accessed later.

	// first get rid of the ui elements (race addons have hold AND click buttons)
	fbl_destroy_all_ui_elems();

	fbl_load_ttf_font("font/roboto.ttf", 16);

	std::string msg = "";
	auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mOwnedRobots[g.mProgress->mFavRobot]);

	switch (Race::sRaceState) {
		case Race::RaceState::First:
			msg = "Congratulations! You placed 1st! Buy something will ya!";
			sta.xp += 4;
			break;
		case Race::RaceState::Second:
			msg = "Very nice you placed 2nd! Please buy whatever you like.";
			sta.xp += 3;
			break;
		case Race::RaceState::Third:
			msg = "Hmm.. You placed 3rd. Not that great but please buy some stuff!";
			sta.xp += 2;
			break;
		case Race::RaceState::Fourth:
			msg = "That was not good, you placed last! Please buy something anyway :)";
			break;
	}

	mContextHelp = fbl_create_text(255, 255, 255, 0, (char*)"%s (Race %d)", msg.c_str(), g.mProgress->mCompletedRaces + 1);
	fbl_set_text_align(mContextHelp, FBL_ALIGN_CENTER);
	fbl_set_text_xy(mContextHelp, x, y - height - 20);

	// create the context help white divider line
	tmpId = fbl_create_prim(FBL_LINE, x - width, y - height, x + width, y - height, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(tmpId, true);


	// create gray menu area
	tmpId = fbl_create_sprite(32, 480, 12, 7, 0);
	//fbl_set_sprite_color(mRobotBgSquareId, 0, 255, 0);
	fbl_set_sprite_xy(tmpId, x, y - 20);
	fbl_set_sprite_scale(tmpId, 64);
	fbl_set_sprite_layer(tmpId, 8);
	fbl_fix_sprite_to_screen(tmpId, true);


	// white robot-bg
	tmpId = fbl_create_sprite(448, 192, 64, 64, 0);
	fbl_set_sprite_xy(tmpId, x + 200, y - 123);
	fbl_set_sprite_layer(tmpId, 5);
	fbl_fix_sprite_to_screen(tmpId, true);
	fbl_set_sprite_is_light(tmpId, true);

	// red cross
	if (sta.hp < 0.1) {
		tmpId = fbl_create_sprite(192, 480, 32, 32, 0);
		fbl_set_sprite_xy(tmpId, x + 200, y - 123);
		fbl_set_sprite_layer(tmpId, 10);
		fbl_fix_sprite_to_screen(tmpId, true);
	}

	// create the white outline
	tmpId = fbl_create_prim(FBL_RECT, x, y - 20, width, height + 20, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(tmpId, true);

	// create the white divider line
	tmpId = fbl_create_prim(FBL_LINE, x, y - height, x, y + height, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(tmpId, true);


	fbl_load_ttf_font("font/roboto.ttf", 20);

	// The "Robot" and "Addons" text at the top
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Robot");
	fbl_set_text_align(tmpId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(tmpId, x + 200, y - 170);
	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Shop");
	fbl_set_text_align(tmpId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(tmpId, x - 190, y - 170);

	// add the N/A sprites in the shop
	tmpId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tmpId, x - 244, y - 94);
	fbl_set_sprite_layer(tmpId, 9);
	tmpId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tmpId, x - 194, y - 94);
	fbl_set_sprite_layer(tmpId, 9);
	tmpId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tmpId, x - 144, y - 94);
	fbl_set_sprite_layer(tmpId, 9);

	tmpId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tmpId, x - 194, y - 30);
	fbl_set_sprite_layer(tmpId, 9);

	tmpId = fbl_create_text(255, 255, 255, 0, (char*)sta.name.c_str());
	fbl_set_text_align(tmpId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(tmpId, x + 200, y - 80);

	fbl_load_ttf_font("font/roboto.ttf", 18);

	int* bonus = nullptr;

	if (g.mRobots->assignRobotXP(g, g.mProgress->mFavRobot)) {

		bonus = g.mRobots->levelUpRobot(g, g.mProgress->mFavRobot, true);

	}


	// stats
	mRobotLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:  %d  (xp: %d / %d)", sta.level, sta.xp, sta.nextLv);
	fbl_set_text_align(mRobotLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mRobotLevel, x + 140, y - 30);
	mRobotHp = fbl_create_text(255, 255, 255, 0, (char*)"Hp:  %d / %d", (int)sta.hp, sta.maxHp);
	fbl_set_text_align(mRobotHp, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mRobotHp, x + 140, y);
	mRobotSpeed = fbl_create_text(255, 255, 255, 0, (char*)"Speed:  %d", sta.speed);
	fbl_set_text_align(mRobotSpeed, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mRobotSpeed, x + 140, y + 30);
	if(sta.diag)
		mRobotDiag = fbl_create_text(255, 255, 255, 0, (char*)"Diagonals:  %s", "Yes");
	else
		mRobotDiag = fbl_create_text(255, 255, 255, 0, (char*)"Diagonals:  %s", "No");
	fbl_set_text_align(mRobotDiag, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mRobotDiag, x + 140, y + 60);
	mRobotEnergy = fbl_create_text(255, 255, 255, 0, (char*)"Energy:  %d / %d", (int)sta.energy, sta.maxEnergy);
	fbl_set_text_align(mRobotEnergy, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mRobotEnergy, x + 140, y + 90);

	// bonus stats as +1 etc
	if (bonus) {

		mRobotLevelBonus = fbl_create_text(83, 240, 87, 0, (char*)"+ %d", bonus[0]);
		fbl_set_text_align(mRobotLevelBonus, FBL_ALIGN_LEFT);
		fbl_set_text_xy(mRobotLevelBonus, x + 310, y - 30);
		mRobotHpBonus = fbl_create_text(83, 240, 87, 0, (char*)"+ %d", bonus[1]);
		fbl_set_text_align(mRobotHpBonus, FBL_ALIGN_LEFT);
		fbl_set_text_xy(mRobotHpBonus, x + 310, y);
		mRobotSpeedBonus = fbl_create_text(83, 240, 87, 0, (char*)"+ %d", bonus[2]);
		fbl_set_text_align(mRobotSpeedBonus, FBL_ALIGN_LEFT);
		fbl_set_text_xy(mRobotSpeedBonus, x + 310, y + 30);
		if (bonus[3]) {
			mRobotDiagBonus = fbl_create_text(83, 240, 87, 0, (char*)"Active");
			fbl_set_text_align(mRobotDiagBonus, FBL_ALIGN_LEFT);
			fbl_set_text_xy(mRobotDiagBonus, x + 310, y + 60);
		}
		mRobotEnergyBonus = fbl_create_text(83, 240, 87, 0, (char*)"+ %d", bonus[4]);
		fbl_set_text_align(mRobotEnergyBonus, FBL_ALIGN_LEFT);
		fbl_set_text_xy(mRobotEnergyBonus, x + 310, y + 90);

		mRobotLevelUp = fbl_create_text(83, 240, 87, 0, (char*)"LEVEL UP!");
		fbl_set_text_align(mRobotLevelUp, FBL_ALIGN_LEFT);
		fbl_set_text_xy(mRobotLevelUp, x + 140, y + 140);

		delete[] bonus;

	}


	// line separator down left
	tmpId = fbl_create_prim(FBL_LINE, x - width, 340, 480, 340, 0, false, false);
	fbl_set_prim_color(tmpId, 255, 255, 255, 255);
	fbl_fix_prim_to_screen(tmpId, true);

	// addon stats
	mAddonName = fbl_create_text(255, 255, 255, 0, (char*)"Name:");
	fbl_set_text_align(mAddonName, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonName, 115, 370);
	mAddonLevel = fbl_create_text(255, 255, 255, 0, (char*)"Level:");
	fbl_set_text_align(mAddonLevel, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonLevel, 115, 405);
	mAddonRarity = fbl_create_text(255, 255, 255, 0, (char*)"Rarity:");
	fbl_set_text_align(mAddonRarity, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonRarity, 115, 440);
	mAddonPassive = fbl_create_text(255, 255, 255, 0, (char*)"Type:");
	fbl_set_text_align(mAddonPassive, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonPassive, 305, 370);
	mAddonEquipped = fbl_create_text(255, 255, 255, 0, (char*)"Equipped:");
	fbl_set_text_align(mAddonEquipped, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonEquipped, 305, 405);
	mAddonPrice = fbl_create_text(255, 255, 255, 0, (char*)"Price:");
	fbl_set_text_align(mAddonPrice, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mAddonPrice, 305, 440);

	// Continue game button
	mContinueButton = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 0, 32, 32, NULL);
	fbl_set_ui_elem_xy(mContinueButton, x, 500);	// down middle

	tmpId = fbl_create_text(255, 255, 255, 0, (char*)"Continue");
	fbl_set_text_align(tmpId, FBL_ALIGN_LEFT);
	fbl_set_text_xy(tmpId, x + 32, 500);

	// Buy selected items button
	mBuyButton = fbl_create_ui_elem(FBL_UI_BUTTON_INTERVAL, 0, 352, 64, 32, NULL);
	fbl_set_ui_elem_xy(mBuyButton, 440, 315);

	auto& plog = g.mEcs->GetComponent<PathLogic>(g.mRobots->mOwnedRobots[g.mProgress->mFavRobot]);
	g.mProgress->mFunds += plog.coins;

	// funds
	mFundsText = fbl_create_text(255, 255, 255, 0, (char*)"Coins: %d", g.mProgress->mFunds);
	fbl_set_text_align(mFundsText, FBL_ALIGN_LEFT);
	fbl_set_text_xy(mFundsText, 115, 315);

	// put the current racing robot in the circle :)
	g.mRobots->showRobotInMenu(g.mEcs, -1, g.mRobots->mOwnedRobots[g.mProgress->mFavRobot]);
	auto& spr = g.mEcs->GetComponent<Sprite>(g.mRobots->mOwnedRobots[g.mProgress->mFavRobot]);
	fbl_set_sprite_xy(spr.id[0], fbl_get_sprite_x(spr.id[0]) + 16, fbl_get_sprite_y(spr.id[0]) + 16);
	fbl_set_sprite_layer((int)spr.id[0], 9);
	fbl_sort_sprites(FBL_SORT_BY_LAYER);

	// set up shop
	g.mAddons->initAddons(g.mEcs);
	g.mAddons->hideAddons(g.mEcs);
	prepShop(g);	// set up the items in the shop

	SoundManager::getInstance().loadAndPlayMusic("music/postrace.ogg", 60);

	// remove all lasers and particles and crosshairs
	for(int i = 0; i < 4; i++)
		if (g.mEcs->HasComponent<Laser>(g.mRobots->mRacingRobots[i])) {
			auto& las = g.mEcs->GetComponent<Laser>(g.mRobots->mRacingRobots[i]);
			fbl_set_prim_active(las.rayId, false);				// turn off ray
			fbl_set_emitter_active(las.particleId, false);		// turn off emitter
			fbl_set_prim_active(las.crossHairId, false);		// turn off crosshair
		}

}
