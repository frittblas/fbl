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

	rainOn = false;

	for (int i = 0; i < 3; i++)
		rainLayerId[i] = 0;

	tint_r = 255;
	tint_g = 255;
	tint_b = 255;

	lightningOn = false;
	lightningTrigger = false;
	lightningTimer = 0;

	std::cout << "Initialized Weather subsystem." << std::endl;

}

Weather::~Weather() {

	std::cout << "Destroyed Weather subsystem." << std::endl;

}

void Weather::setWeather(TimeOfDay time, uint8_t rainLevel, uint8_t numClouds, bool lightning) {

	uint8_t rainAlpha = 255; // used to thin out the rain when evening/night

	// set time of day
	switch (time) {

		case TimeOfDay::Morning :
			tint_r = 220;
			tint_g = 220;
			tint_b = 220;
			fbl_set_lighting_tint(true, tint_r, tint_g, tint_b);
			rainAlpha = 160;
			break;
		case TimeOfDay::Day :
			tint_r = 255;
			tint_g = 255;
			tint_b = 255;
			fbl_set_lighting_tint(false, tint_r, tint_g, tint_b);	// turned off = full brightness
			rainAlpha = 180;
			break;
		case TimeOfDay::Evening :
			tint_r = 100;
			tint_g = 100;
			tint_b = 100;
			fbl_set_lighting_tint(true, tint_r, tint_g, tint_b);
			rainAlpha = 110;
			break;
		case TimeOfDay::Late :
			tint_r = 50;
			tint_g = 50;
			tint_b = 50;
			fbl_set_lighting_tint(true, tint_r, tint_g, tint_b);
			rainAlpha = 80;
			break;
		case TimeOfDay::Night :
			tint_r = 20;
			tint_g = 20;
			tint_b = 20;
			fbl_set_lighting_tint(true, tint_r, tint_g, tint_b);
			rainAlpha = 50;
			break;

	}

	initClouds(numClouds);
	initRain(rainLevel, rainAlpha);

	// set lightning on/off
	lightningOn = lightning;

}

void Weather::initClouds(uint8_t num) {

	// remove any existing clouds

	for (Cloud* curCloud : mCloud) {

		if (curCloud != nullptr) {
			fbl_delete_sprite(curCloud->id);
			delete curCloud;
		}

	}

	mCloud.clear();	// clear the vector

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
		tmpCloud->x = (float)(rand() % (Game::MapW * Game::TileSize));
		tmpCloud->y = (float)(rand() % 150 - 20);
		tmpCloud->speed = (float)((rand() % 8) + 3) / 10;
		tmpCloud->scale = (float)((rand() % 10) + 10) / 10;
		tmpCloud->alpha = rand() % 155 + 100;

		// some clouds go the other way
		if (rand() % 2 == 0) tmpCloud->speed = -tmpCloud->speed;

		fbl_set_sprite_xy(tmpCloud->id, (int)tmpCloud->x, (int)tmpCloud->y);
		fbl_set_sprite_scale(tmpCloud->id, tmpCloud->scale);
		fbl_set_sprite_alpha(tmpCloud->id, tmpCloud->alpha);

		// randomly flip clouds horizontally
		if (rand() % 2 == 0)
			fbl_set_sprite_flip(tmpCloud->id, 1);	// 1 = horizontal flip

		mCloud.push_back(tmpCloud);

	}

}

void Weather::initRain(uint8_t amount, uint8_t alpha) {

	if (rainOn) {

		// delete any emitters that exists
		for (int i = 0; i < 3; i++)
			fbl_delete_emitter(rainLayerId[i]);

		rainOn = false;

	}

	if (amount > 0) {

		rainLayerId[0] = fbl_create_emitter(450);
		fbl_set_emitter_params(rainLayerId[0], FBL_EMITTER_RAIN, Game::LogicalResW, 64, 150, amount, 5, 1.0, 1.0);	// id, type, spawn_w, spawn_h, life, rate, density, scale_start, scale_end
		fbl_set_emitter_particle_shape(rainLayerId[0], FBL_NO_PRIM, 38, 384, 3, 12);
		fbl_set_emitter_xy(rainLayerId[0], 30, -64);
		fbl_set_emitter_vel_xy(rainLayerId[0], -0.5, 1.5, true);	// crash if you call this with < 1.0 on either param using FLOWER
		fbl_set_emitter_color(rainLayerId[0], 30, 30, 200, alpha, true);
		fbl_set_emitter_color(rainLayerId[0], 30, 30, 200, 30, false);
		fbl_fix_emitter_to_screen(rainLayerId[0], true);

		rainLayerId[1] = fbl_create_emitter(450);
		fbl_set_emitter_params(rainLayerId[1], FBL_EMITTER_RAIN, Game::LogicalResW, 64, 150, amount, 5, 1.0, 1.0);
		fbl_set_emitter_particle_shape(rainLayerId[1], FBL_NO_PRIM, 35, 384, 3, 12);
		fbl_set_emitter_xy(rainLayerId[1], 30, -64);
		fbl_set_emitter_vel_xy(rainLayerId[1], -0.6, 2.5, true);
		fbl_set_emitter_color(rainLayerId[1], 30, 30, 200, alpha, true);
		fbl_set_emitter_color(rainLayerId[1], 30, 30, 200, 30, false);
		fbl_fix_emitter_to_screen(rainLayerId[1], true);

		rainLayerId[2] = fbl_create_emitter(350);
		fbl_set_emitter_params(rainLayerId[2], FBL_EMITTER_RAIN, Game::LogicalResW, 64, 110, amount, 5, 1.0, 1.0);
		fbl_set_emitter_particle_shape(rainLayerId[2], FBL_NO_PRIM, 32, 384, 3, 12);
		fbl_set_emitter_xy(rainLayerId[2], 30, -64);
		fbl_set_emitter_vel_xy(rainLayerId[2], -0.7, 5.0, true);
		fbl_set_emitter_color(rainLayerId[2], 100, 100, 255, alpha, true);
		fbl_set_emitter_color(rainLayerId[2], 100, 100, 200, 30, false);
		fbl_fix_emitter_to_screen(rainLayerId[2], true);

		rainOn = true;

	}

}

void Weather::tick(Game& g) {


	// clouds
	for (Cloud* curCloud : mCloud) {

		curCloud->x += curCloud->speed;

		if (curCloud->x < -200 || curCloud->x >(Game::MapW * Game::TileSize) + 50)
			curCloud->speed = -curCloud->speed;

		fbl_set_sprite_xy(curCloud->id, (int)curCloud->x, (int)curCloud->y);

	}

	// lightning
	if (lightningOn) {

		// lightning every 10 seconds
		if (fbl_get_raw_frames_count() % 600 == 0) {
			lightningTrigger = true;
			lightningTimer = 20;
			std::cout << "Lightning!" << std::endl;
		}

		if (lightningTrigger) {

			switch (lightningTimer) {

				case 20 :
					fbl_set_lighting_tint(false, 0, 0, 0);	// turn off tint (becomes bright!)
					break;
				case 15:
					fbl_set_lighting_tint(true, tint_r, tint_g, tint_g);	// back to what it was
					break;
				case 11:
					// randomly flash again
					if(rand() % 2 == 0)
						fbl_set_lighting_tint(false, 0, 0, 0);
					break;
				case 8:
					fbl_set_lighting_tint(true, tint_r, tint_g, tint_g);
					break;

			}

			lightningTimer--;
			if (lightningTimer < 1)
				lightningTrigger = false;

		}

	}

	//std::cout << "num particles1: " << fbl_get_num_active_particles(rainLayerId[0]) << std::endl;
	//std::cout << "num particles2: " << fbl_get_num_active_particles(rainLayerId[1]) << std::endl;
	//std::cout << "num particles3: " << fbl_get_num_active_particles(rainLayerId[2]) << std::endl;

}
