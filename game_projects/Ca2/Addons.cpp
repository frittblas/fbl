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
	}

	// create all the addon entities
	for (int i = 0; i < NumAddons; i++) {

		// create the addon entity
		tmpAddon = mEcs->CreateEntity();

		// add Addon-component with correct params to the entity
		switch (i) {

		case AutoAim:
											   // type      name   uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ AutoAim, "Auto Aim", 0, 0, 96, 1, 1, true, notSet, 19});

			break;
		case Laser:
											 // type		name   uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Laser, "Red Laser", 0, 0, 128, 1, 2, false, notSet, 20 });

			break;
		case Laser2:
											// type			name     uiId tx ty lv rrty  psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Laser2, "Green Laser", 0, 0, 128, 2, 3, false, notSet, 20 });

			break;
		case Magnet:
											 // type    name   uiId tx ty	lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Magnet, "Magnet", 0, 0, 160, 1, 2, true, notSet, 12 });

			break;
		case Turbo:
											  // type   name  uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Turbo, "Turbo", 0, 0, 320, 1, 2, false, notSet, 9 });

			break;
		case Shield:
												// type   name  uiId tx ty	lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Shield, "Shield", 0, 0, 256, 1, 2, false, notSet, 15 });

			break;
		case Heal:
											  // type  name uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Heal, "Heal", 0, 0, 288, 1, 2, false, notSet, 11 });

			break;
		case Diag:
											  // type  name      uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Diag, "Diagonals", 0, 0, 192, 1, 2, true, notSet, 5 });

			break;
		case RobotCtrl:
											   // type	  	 name		  uiId tx ty   lv rrty psv  eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ RobotCtrl, "Robot Control", 0, 0, 224, 1, 2, true, notSet, 12 });

			break;

		}

		// add the entity to the array containing all addons
		mAllAddons[i] = tmpAddon;

	}

	claimAddon(AutoAim);
	claimAddon(Laser);
	claimAddon(Laser2);
	claimAddon(Magnet);
	claimAddon(Turbo);
	claimAddon(Shield);
	claimAddon(Heal);
	claimAddon(Diag);
	claimAddon(RobotCtrl);

}

void Addons::initAddons(Coordinator* mEcs) {

	// create the ui elements for the addons (buttons)

	for (Entity e : mAllAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			add.uiId = fbl_create_ui_elem(FBL_UI_CHECKBOX_INTERVAL, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
			setAddonColor(add.uiId, add.level);
		}
	}

	for (Entity e : mOwnedAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			add.uiId = fbl_create_ui_elem(FBL_UI_CHECKBOX_INTERVAL, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
			setAddonColor(add.uiId, add.level);
		}
	}

}

void Addons::initRaceAddons(Coordinator* mEcs) {

	// create the ui elements for the addons (buttons)

	for (Entity e : mAllAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			if (add.passive)
				add.uiId = fbl_create_ui_elem(FBL_UI_CHECKBOX_INTERVAL, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
			else
				add.uiId = fbl_create_ui_elem(FBL_UI_BUTTON_HOLD, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);

			setAddonColor(add.uiId, add.level);
		}
	}

	for (Entity e : mOwnedAddons) {
		if (e != Unassigned) {
			auto& add = mEcs->GetComponent<Addon>(e);
			if (add.passive) {
				add.uiId = fbl_create_ui_elem(FBL_UI_CHECKBOX_INTERVAL, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);
				fbl_set_ui_elem_val(add.uiId, 1);	// set the passive addons to on by default
			}
			else
				add.uiId = fbl_create_ui_elem(FBL_UI_BUTTON_HOLD, add.tx, add.ty, Game::TileSize, Game::TileSize, NULL);

			setAddonColor(add.uiId, add.level);
		}
	}

}

void Addons::setAddonColor(int uiId, int level) {

	switch (level) {

	case 1:
		fbl_set_ui_elem_color(uiId, 255, 255, 255);
		break;
	case 2:
		fbl_set_ui_elem_color(uiId, 200, 255, 200);
		break;
	case 3:
		fbl_set_ui_elem_color(uiId, 255, 200, 200);
		break;

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
			if (add.equippedBy == notSet) {
				fbl_set_ui_elem_xy(add.uiId, x, y);			// position it
				fbl_set_ui_elem_active(add.uiId, true);		// set to active
			}
			else fbl_set_ui_elem_active(add.uiId, false);   // set to inactive if equipped (these are set in robotcollection)
			fbl_set_ui_elem_val(add.uiId, 0);				// uncheck it
			x += 35;
			if (x > 440) {	// if one row is filled continue on the next row
				x = 123;
				y += 36;
			}
		}

	}

}

void Addons::showAddonAsEquipped(Coordinator* mEcs, Entity addon, int position) {


	int x = Game::DeviceResW / 2;
	int y = Game::DeviceResH / 2;

	switch (position) {

	case 0:
		x += 50;
		y -= 20;
		break;
	case 1:
		x += 350;
		y -= 20;
		break;
	case 2:
		x += 50;
		y += 65;
		break;
	case 3:
		x += 350;
		y += 65;
		break;
	case 4:
		x += 50;
		y += 150;
		break;
	case 5:
		x += 350;
		y += 150;
		break;

	}

	auto& add = mEcs->GetComponent<Addon>(addon);

	fbl_set_ui_elem_xy(add.uiId, x, y);
	fbl_set_ui_elem_active(add.uiId, true);

	std::cout << "activated addon ui: " << add.uiId << " at x " << x << " and y " << y << std::endl;

}

void Addons::showAddonInRace(Coordinator* mEcs, Entity addon, int position) {

	int x = 0;
	int y = 0;

	switch (position) {

	case 0:
		x = Game::TileSize * 2 - Game::TileSize / 2;
		y = Game::TileSize * 4;
		break;
	case 1:
		x = Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2;
		y = Game::TileSize * 4;
		break;
	case 2:
		x = Game::TileSize * 2 - Game::TileSize / 2;
		y = Game::TileSize * 8;
		break;
	case 3:
		x = Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2;
		y = Game::TileSize * 8;
		break;
	case 4:
		x = Game::TileSize * 2 - Game::TileSize / 2;
		y = Game::TileSize * 11;
		break;
	case 5:
		x = Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2;
		y = Game::TileSize * 11;
		break;

	}

	auto& add = mEcs->GetComponent<Addon>(addon);

	fbl_set_ui_elem_xy(add.uiId, x, y);
	fbl_set_ui_elem_active(add.uiId, true);

	std::cout << "activated addon ui: " << add.uiId  << " at x " << x << " and y " << y << std::endl;

}

void Addons::claimAddon(int nameIndex) {

	// remove entity from the "All" list and put it in the "Owned" list

	if (mAllAddons[nameIndex] != Unassigned) {

		mOwnedAddons[nameIndex] = mAllAddons[nameIndex];
		mAllAddons[nameIndex] = Unassigned;

	}

	for (Entity e : mOwnedAddons) std::cout << "Owned addon entity-id: " << e << std::endl;

}
