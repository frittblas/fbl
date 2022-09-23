/*
*
*	Lotr
*
*	Weather.cpp
*
*	Weather class implementation, handles the weather system.
*
*	Hans Strömquist 2022
*
*/
#include "../../src/fbl.hpp"
#include "Game.hpp"
#include "Weather.hpp"

// Weather-class implementation

Weather::Weather() {

	std::cout << "Initialized Weather subsystem." << std::endl;

}

Weather::~Weather() {

	std::cout << "Destroyed Weather subsystem." << std::endl;

}

void Weather::initClouds(int num) {

	// remove any existing clouds

	for (Cloud* curCloud : mCloud) {

		if (curCloud != nullptr) {
			delete curCloud;
		}

	}

	mCloud.clear();

	// create num clouds
	for (int i = 0; i < num; i++) {

		int id = 0;

		// add random cloud
		switch (rand() % 3) {
			case 0 :
				id = fbl_create_sprite(0, 320, 96, 32, 0); // the big one
				break;
			case 1:
				id = fbl_create_sprite(64, 352, 64, 32, 0); // the puffy one
				break;
			case 2:
				id = fbl_create_sprite(64, 384, 64, 32, 0); // the small one
				break;
		}

		// create a tmp cloud and set random params, then add to the list
		Cloud* tmpCloud = new Cloud();

		tmpCloud->id = id;
		tmpCloud->x = rand() % (Game::MapW * Game::TileSize);
		tmpCloud->y = rand() % 160 - 32;
		tmpCloud->speed = (float)((rand() % 8) + 3) / 10;
		tmpCloud->scale = (float)((rand() % 10) + 10) / 10;
		tmpCloud->alpha = rand() % 200 + 55;

		// some clouds go the other way
		if (rand() % 2 == 0) tmpCloud->speed = -tmpCloud->speed;

		fbl_set_sprite_xy(tmpCloud->id, tmpCloud->x, tmpCloud->y);
		fbl_set_sprite_scale(tmpCloud->id, tmpCloud->scale);
		fbl_set_sprite_alpha(tmpCloud->id, tmpCloud->alpha);

		// fix to screen? no
		//fbl_fix_sprite_to_screen(tmpCloud->id, true);

		mCloud.push_back(tmpCloud);

	}

}

void Weather::initRain() {
}

void Weather::tick(Game& g) {

	for (Cloud* curCloud : mCloud) {

		curCloud->x += curCloud->speed;

		if (curCloud->x < -200 || curCloud->x >(Game::MapW * Game::TileSize) + 50)
			curCloud->speed = -curCloud->speed;

		fbl_set_sprite_xy(curCloud->id, curCloud->x, curCloud->y);

	}

}
