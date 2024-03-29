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
		mShopAddons[i] = Unassigned;
	}

	// create all the addon entities
	for (int i = 0; i < NumAddons; i++) {

		// create the addon entity
		tmpAddon = mEcs->CreateEntity();

		// add Addon-component with correct params to the entity
		switch (i) {

		case AutoAim1:
											   // type      name		 uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ AutoAim1, "Auto Aim Slow", 0, 0, 96, 1, 1, true, notSet, 4});

			break;
		case AutoAim2:
											  // type      name			 uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ AutoAim2, "Auto Aim Quick", 0, 0, 96, 2, 2, true, notSet, 8 });

			break;
		case AutoAim3:
												// type      name		 uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ AutoAim3, "Auto Aim Super", 0, 0, 96, 3, 4, true, notSet, 12 });

			break;
		case Laser1:
											 // type		name   uiId tx ty lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Laser1, "Laser Red", 0, 0, 128, 1, 1, false, notSet, 5 });

			break;
		case Laser2:
											// type			name     uiId tx ty lv rrty  psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Laser2, "Laser Green", 0, 0, 128, 2, 2, false, notSet, 10 });

			break;
		case Laser3:
												// type		name     uiId tx ty lv rrty  psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Laser3, "Laser Blue", 0, 0, 128, 3, 3, false, notSet, 20 });

			break;
		case Magnet1:
											 // type           name   uiId tx ty  lv rrty psv    eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Magnet1, "Magnet Weak", 0, 0, 160, 1, 1, true, notSet, 3 });

			break;
		case Magnet2:
											 // type      name   uiId tx ty	lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Magnet2, "Magnet Strong", 0, 0, 160, 2, 2, true, notSet, 7 });

			break;
		case Magnet3:
												// type    name			uiId tx ty	lv rrty psv  eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Magnet3, "Magnet Super", 0, 0, 160, 3, 3, true, notSet, 12 });

			break;
		case Turbo1:
											  // type     name      uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Turbo1, "Turbo Slow", 0, 0, 320, 1, 1, false, notSet, 5 });

			break;
		case Turbo2:
											  // type      name     uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Turbo2, "Turbo Fast", 0, 0, 320, 2, 2, false, notSet, 9 });

			break;
		case Turbo3:
											  // type   name		 uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Turbo3, "Turbo Super", 0, 0, 320, 3, 3, false, notSet, 14 });

			break;
		case Shield1:
												// type     name	  uiId tx ty  lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Shield1, "Shield Weak", 0, 0, 256, 1, 1, false, notSet, 5 });

			break;
		case Shield2:
												// type		name	   uiId tx ty	lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Shield2, "Shield Strong", 0, 0, 256, 2, 2, false, notSet, 10 });

			break;
		case Shield3:
												// type    name       uiId tx ty	lv rrty psv     eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Shield3, "Shield Super", 0, 0, 256, 3, 3, false, notSet, 20 });

			break;
		case Heal1:
											  // type   name		uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Heal1, "Heal Small", 0, 0, 288, 1, 1, false, notSet, 6 });

			break;
		case Heal2:
											  // type  name		 uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Heal2, "Heal Big", 0, 0, 288, 2, 2, false, notSet, 11 });

			break;
		case Heal3:
											 // type  name  uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Heal3, "Heal", 0, 0, 288, 3, 3, false, notSet, 17 });

			break;
		case Diag:
											  // type  name      uiId tx ty  lv rrty psv   eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ Diag, "Diagonals", 0, 0, 192, 1, 2, true, notSet, 3 });

			break;
		case RobotCtrl:
											   // type	  	 name		  uiId tx ty   lv rrty psv  eqp  price
			mEcs->AddComponent(tmpAddon, Addon{ RobotCtrl, "Robot Control", 0, 0, 224, 1, 2, true, notSet, 7 });

			break;

		}

		// add the entity to the array containing all addons
		mAllAddons[i] = tmpAddon;

	}
	/*
	for (int i = AutoAim1; i < NumAddons - 10; i++) {
		claimAddon(i);
	}
	
*/
	/*
	claimAddon(Laser1);
	claimAddon(AutoAim1);
	claimAddon(RobotCtrl);
	claimAddon(Shield1); */
	
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
		fbl_set_ui_elem_color(uiId, 255, 200, 200);
		break;
	case 2:
		fbl_set_ui_elem_color(uiId, 200, 255, 200);
		break;
	case 3:
		fbl_set_ui_elem_color(uiId, 200, 200, 255);
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

		if (e == Unassigned) continue;

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

void Addons::showAddonsInShop(Coordinator* mEcs) {

	int x = 236;
	int y = 176;


	for (Entity e : mShopAddons) {

		if (e == Unassigned) continue;

		auto& add = mEcs->GetComponent<Addon>(e);

		fbl_set_ui_elem_xy(add.uiId, x, y);			// position it
		fbl_set_ui_elem_active(add.uiId, true);		// set to active

		x += 50;

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
