/*
*
*	Charming Alarming 2: Reasonable Robots
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

	mRainOn = false;
	mSnowOn = false;

	for (int i = 0; i < 3; i++) {
		mRainLayerId[i] = 0;
		mSnowLayerId[i] = 0;
	}

	mTint_r = 255;
	mTint_g = 255;
	mTint_b = 255;

	mLightningOn = false;
	mLightningTrigger = false;
	mLightningTimer = 0;

	std::cout << "Initialized Weather subsystem." << std::endl;

}

Weather::~Weather() {

	std::cout << "Destroyed Weather subsystem." << std::endl;

}

void Weather::setWeather(TimeOfDay time, uint8_t rainLevel, uint8_t snowLevel, uint8_t numClouds, bool doLightning) {

	uint8_t particleAlpha = 255; // used to thin out the rain when evening/night

	// set time of day
	switch (time) {

		case TimeOfDay::Morning :
			mTint_r = 220;
			mTint_g = 220;
			mTint_b = 220;
			fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_b);
			particleAlpha = 160;
			break;
		case TimeOfDay::Day :
			mTint_r = 255;
			mTint_g = 255;
			mTint_b = 255;
			fbl_set_lighting_tint(false, mTint_r, mTint_g, mTint_b);	// turned off = full brightness
			particleAlpha = 180;
			break;
		case TimeOfDay::Evening :
			mTint_r = 120;
			mTint_g = 120;
			mTint_b = 120;
			fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_b);
			particleAlpha = 110;
			break;
		case TimeOfDay::Late :
			mTint_r = 60;
			mTint_g = 60;
			mTint_b = 60;
			fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_b);
			particleAlpha = 80;
			break;
		case TimeOfDay::Night :
			mTint_r = 0;
			mTint_g = 0;
			mTint_b = 0;
			fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_b);
			particleAlpha = 40;
			break;

	}

	initClouds(numClouds);
	initRain(rainLevel, particleAlpha);
	initSnow(snowLevel, particleAlpha);

	// set lightning on/off
	mLightningOn = doLightning;

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
		tmpCloud->x = (float)(rand() % (Game::LogicalResW * 2));
		tmpCloud->y = (float)(rand() % 140 - 20);
		tmpCloud->speed = (float)((rand() % 6) + 3) / 10;
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

	if (mRainOn) {

		// delete any emitters that exists
		for (int i = 0; i < 3; i++)
			fbl_delete_emitter(mRainLayerId[i]);

		mRainOn = false;

	}

	if (amount > 0) {

		mRainLayerId[0] = fbl_create_emitter(450);
		fbl_set_emitter_params(mRainLayerId[0], FBL_EMITTER_RAIN, Game::DeviceResW, 64, 150, amount, 5, 1.0, 1.0);	// id, type, spawn_w, spawn_h, life, rate, density, scale_start, scale_end
		fbl_set_emitter_particle_shape(mRainLayerId[0], FBL_NO_PRIM, 38, 384, 3, 12);
		fbl_set_emitter_xy(mRainLayerId[0], 30, -64);
		fbl_set_emitter_vel_xy(mRainLayerId[0], -0.5, 1.5, true);	// crash if you call this with < 1.0 on either param using FLOWER
		fbl_set_emitter_color(mRainLayerId[0], 30, 30, 200, alpha, true);
		fbl_set_emitter_color(mRainLayerId[0], 30, 30, 200, 30, false);
		fbl_fix_emitter_to_screen(mRainLayerId[0], true);

		mRainLayerId[1] = fbl_create_emitter(450);
		fbl_set_emitter_params(mRainLayerId[1], FBL_EMITTER_RAIN, Game::DeviceResW, 64, 150, amount, 5, 1.0, 1.0);
		fbl_set_emitter_particle_shape(mRainLayerId[1], FBL_NO_PRIM, 35, 384, 3, 12);
		fbl_set_emitter_xy(mRainLayerId[1], 30, -64);
		fbl_set_emitter_vel_xy(mRainLayerId[1], -0.6, 2.5, true);
		fbl_set_emitter_color(mRainLayerId[1], 30, 30, 200, alpha, true);
		fbl_set_emitter_color(mRainLayerId[1], 30, 30, 200, 30, false);
		fbl_fix_emitter_to_screen(mRainLayerId[1], true);

		mRainLayerId[2] = fbl_create_emitter(350);
		fbl_set_emitter_params(mRainLayerId[2], FBL_EMITTER_RAIN, Game::DeviceResW, 64, 110, amount, 5, 1.0, 1.0);
		fbl_set_emitter_particle_shape(mRainLayerId[2], FBL_NO_PRIM, 32, 384, 3, 12);
		fbl_set_emitter_xy(mRainLayerId[2], 30, -64);
		fbl_set_emitter_vel_xy(mRainLayerId[2], -0.7, 5.0, true);
		fbl_set_emitter_color(mRainLayerId[2], 100, 100, 255, alpha, true);
		fbl_set_emitter_color(mRainLayerId[2], 100, 100, 200, 30, false);
		fbl_fix_emitter_to_screen(mRainLayerId[2], true);

		mRainOn = true;

	}

}

void Weather::initSnow(uint8_t amount, uint8_t alpha) {

	if (mSnowOn) {

		// delete any emitters that exists
		for (int i = 0; i < 3; i++)
			fbl_delete_emitter(mSnowLayerId[i]);

		mSnowOn = false;

	}

	if (amount > 0) {

		mSnowLayerId[0] = fbl_create_emitter(300);
		fbl_set_emitter_params(mSnowLayerId[0], FBL_EMITTER_SNOW, Game::DeviceResW, 64, 400, amount, 3, 0.4, 0.4);	// id, type, spawn_w, spawn_h, life, rate, density, scale_start, scale_end
		fbl_set_emitter_particle_shape(mSnowLayerId[0], FBL_NO_PRIM, 416, 128, 32, 32);
		fbl_set_emitter_xy(mSnowLayerId[0], 0, -64);
		fbl_set_emitter_vel_xy(mSnowLayerId[0], 0.5, 0.8, true);
		fbl_set_emitter_color(mSnowLayerId[0], 200, 200, 200, alpha, true);
		fbl_set_emitter_color(mSnowLayerId[0], 255, 255, 255, 0, false);
		fbl_fix_emitter_to_screen(mSnowLayerId[0], true);

		mSnowLayerId[1] = fbl_create_emitter(300);
		fbl_set_emitter_params(mSnowLayerId[1], FBL_EMITTER_SNOW, Game::DeviceResW, 64, 400, amount, 3, 0.6, 0.6);
		fbl_set_emitter_particle_shape(mSnowLayerId[1], FBL_NO_PRIM, 416, 128, 32, 32);
		fbl_set_emitter_xy(mSnowLayerId[1], 0, -64);
		fbl_set_emitter_vel_xy(mSnowLayerId[1], 0.5, 1.1, true);
		fbl_set_emitter_color(mSnowLayerId[1], 220, 220, 220, alpha, true);
		fbl_set_emitter_color(mSnowLayerId[1], 255, 255, 255, 0, false);
		fbl_fix_emitter_to_screen(mSnowLayerId[1], true);

		mSnowLayerId[2] = fbl_create_emitter(300);
		fbl_set_emitter_params(mSnowLayerId[2], FBL_EMITTER_SNOW, Game::DeviceResW, 64, 450, amount, 3, 0.8, 1.0);
		fbl_set_emitter_particle_shape(mSnowLayerId[2], FBL_NO_PRIM, 416, 128, 32, 32);
		fbl_set_emitter_xy(mSnowLayerId[2], 0, -64);
		fbl_set_emitter_vel_xy(mSnowLayerId[2], 0.5, 1.3, true);
		fbl_set_emitter_color(mSnowLayerId[2], 255, 255, 255, alpha, true);
		fbl_set_emitter_color(mSnowLayerId[2], 255, 255, 255, 0, false);
		fbl_fix_emitter_to_screen(mSnowLayerId[2], true);

		mSnowOn = true;

	}

}

void Weather::tickClouds() {

	for (Cloud* curCloud : mCloud) {

		curCloud->x += curCloud->speed;

		if (curCloud->x < -200 || (curCloud->x > ((Game::LogicalResW * 2) + 50)))
			curCloud->speed = -curCloud->speed;

		fbl_set_sprite_xy(curCloud->id, (int)curCloud->x, (int)curCloud->y);

	}

}

void Weather::tickLightning() {

	if (mLightningOn) {

		// flash about every 10 seconds
		if (fbl_get_raw_frames_count() % 600 == 0) {
			mLightningTrigger = true;
			mLightningTimer = 20;
			std::cout << "Lightning! frame:" << fbl_get_raw_frames_count() << std::endl;
		}

		if (mLightningTrigger) {

			switch (mLightningTimer) {

			case 20:
				fbl_set_lighting_tint(false, 0, 0, 0);	// turn off mTint (becomes bright!)
				break;
			case 15:
				fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_g);	// back to what it was
				break;
			case 11:
				// randomly flash again (2/3)
				if (rand() % 3 > 0)
					fbl_set_lighting_tint(false, 0, 0, 0);
				break;
			case 8:
				// randomly make 2nd flash slightly longer
				if (rand() % 3 > 0)
					fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_g);
				break;
			case 4:
				fbl_set_lighting_tint(true, mTint_r, mTint_g, mTint_g);
				break;

			}

			mLightningTimer--;
			if (mLightningTimer < 1)
				mLightningTrigger = false;

		}

	}

}

void Weather::tick() {


	tickClouds();
	tickLightning();

	//std::cout << "num particles1: " << fbl_get_num_active_particles(mSnowLayerId[0]) << std::endl;

}
