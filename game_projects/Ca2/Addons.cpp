/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Addons.cpp
*
*	Addons class, handles the addons in the game, both in race and explore mode.
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Addons.hpp"

// Addons-class implementation

Addons::Addons() {

	std::cout << "Initialized Addons subsystem." << std::endl;

}

Addons::~Addons() {

	std::cout << "Destroyed Addons subsystem." << std::endl;

}


void Addons::setupAddons(Coordinator* mEcs) {

	Entity tmpAddon;

	// init to unassigned
	for (int i = 0; i < NumAddons; i++) {
		mAllAddons[i] = Unassigned;
		mOwnedAddons[i] = Unassigned;
		mRacingAddons[i] = Unassigned;
	}

	// create all the addon entities
	for (int i = 0; i < NumAddons; i++) {

		// create the addon entity
		tmpAddon = mEcs->CreateEntity();

		// add Addon-component with correct params to the entity
		switch (i) {

		case AutoAim:
											 // type  uiId tx ty lv rrty psv eqp price
			mEcs->AddComponent(tmpAddon, Addon{ AutoAim, 0, 0, 96, 1, 1, true, false, 19});

			break;

		case Laser:
											// type uiId tx ty lv rrty psv eqp price
			mEcs->AddComponent(tmpAddon, Addon{ Laser, 0, 0, 64, 1, 2, false, false, 20 });

			break;

		}

		// add the entity to the array containing all robots
		mAllAddons[i] = tmpAddon;

	}

	claimAddon(Laser);

}

void Addons::initAddons(Coordinator* mEcs) {

	// create the ui elements for the addons (buttons)

	for (Entity e : mAllAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			add.uiId = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
		}
	}

	for (Entity e : mOwnedAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			add.uiId = fbl_create_ui_elem(FBL_UI_CHECKBOX, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
		}
	}

}

void Addons::removeAddons(Coordinator* mEcs) {

	for (Entity e : mAllAddons) {
		if (e != Unassigned) {
			mEcs->DestroyEntity(e);
			std::cout << "Addon removed from All-list." << std::endl;
		}
	}

	for (int i = 0; i < NumAddons; i++) {
		if (mOwnedAddons[i] != Unassigned) {
			mEcs->DestroyEntity(mOwnedAddons[i]);
			std::cout << "Addon removed from Owned-list." << std::endl;
		}
	}

}

void Addons::hideAddons(Coordinator* mEcs) {

	for (Entity e : mAllAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			fbl_set_ui_elem_active(add.uiId, false);	// don't show the button (ui element)
		}
	}

	for (Entity e : mOwnedAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			fbl_set_ui_elem_active(add.uiId, false);
		}
	}

}

void Addons::showAddonsInMenu(Coordinator* mEcs) {

	int x = 123;
	int y = 150;


	for (Entity e : mOwnedAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			fbl_set_ui_elem_xy(add.uiId, x, y);
			fbl_set_ui_elem_active(add.uiId, true);
		}

	}

}

void Addons::showAddonInRace(Coordinator* mEcs, Entity addon, int position) {

	int x, y;

	switch (position) {

	case 0:
		x = Game::TileSize * 3;
		y = 0;
		break;
	case 1:
		x = Game::LogicalResW - Game::TileSize * 4;
		y = 0;
		break;
	case 2:
		x = Game::TileSize * 3;
		y = Game::TileSize * 16;
		break;
	case 3:
		x = Game::LogicalResW - Game::TileSize * 4;
		y = Game::TileSize * 16;
		break;
	case 4:
		break;
	case 5:
		break;

	}

	auto& add = mEcs->GetComponent<Addon>(addon);

	fbl_set_ui_elem_xy(add.uiId, x, y);
	fbl_set_ui_elem_active(add.uiId, true);

}

void Addons::claimAddon(int nameIndex) {

	// remove entity from the "All" list and put it in the "Owned" list

	if (mAllAddons[nameIndex] != Unassigned) {

		mOwnedAddons[nameIndex] = mAllAddons[nameIndex];
		mAllAddons[nameIndex] = Unassigned;

	}

	for (Entity e : mOwnedAddons) std::cout << "Owned addon entity-id: " << e << std::endl;

}
