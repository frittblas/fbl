/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Maze.cpp
*
*   Maze class implementation, takes care of the maze generation etc..
*
*	Hans Strömquist 2022
*
*/


#include "../../../../src/fbl.hpp"
#include "../../Ecs/Ecs.hpp"
#include "../../Ecs/Components.hpp"
#include "../../Game.hpp"
#include "../../Addons.hpp"
#include "Maze.hpp"

// In order to connect PathLogicSystem, Laser, CaptureFlags, DeathMatch to Maze
// I had to use these class statics
Maze::aFlag Maze::sFlag[Maze::cMaxFlags];
Maze::aCoin Maze::sCoin[Maze::cMaxCoins];
bool Maze::sPickDone;
bool Maze::sStartingOut;
bool Maze::sUpdatePaths;
int  Maze::sGameMode;
CaptureFlags *Maze::sCF;
DeathMatch	 *Maze::sDM;

// Maze-class implementation

Maze::Maze() {

	sPickDone = false;
	sStartingOut = true;
	sUpdatePaths = false;
	sGameMode = 0;

	sCF = new CaptureFlags();
	sDM = new DeathMatch();

	std::cout << "Maze constructor." << std::endl;

}

Maze::~Maze() {

	delete sCF;
	delete sDM;

	std::cout << "Maze destructor." << std::endl;

}

void Maze::tick(Game& g) {

	if (mPickTimer > 0)
		pickStartPosition(g);
	else {

		sPickDone = true;

		if (mPickTimer == -240) { // wait a little before starting
			// make robots move (this gets taken care of in handleBases instead (PathLogicSystem))
			//for (int i = 0; i < mNumRacers; i++) fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_FOUND);
			std::cout << "Picked pos: " << mPickedPosition << std::endl;
			std::cout << "Running! Num sprites: " << fbl_get_num_sprites() << std::endl;
			mPickTimer--;
			sStartingOut = false;
		}
		else if(mPickTimer > -250)
			mPickTimer--;

		updateGUI(g);

	}

}

void Maze::setupPickStart() {

	// black bg
	mBlackBgId = fbl_create_prim(FBL_NORMAL_RECT, 0, 0, Game::DeviceResW, Game::DeviceResH, 0, 0, 1);
	fbl_set_prim_color(mBlackBgId, 0, 0, 0, 255);
	mBlackBgFade = 255;

	// get ready text
	fbl_load_ttf_font("font/garamond.ttf", 48);
	mGetReadyTextId = fbl_create_text(255, 69, 0, 255, (char*)"GET READY!");
	fbl_set_text_align(mGetReadyTextId, FBL_ALIGN_CENTER);
	fbl_set_text_xy(mGetReadyTextId, fbl_get_screen_w() / 2, fbl_get_screen_h() / 3);

	// circle = type 6 (starts at id's 1-11)

	mCircleSize[0] = 5;
	mCircleSize[1] = 30;
	mCircleSize[2] = 55;

	int xOffset = Game::TileSize / 2 + 96;

	mFirstCircleId = fbl_create_prim(FBL_CIRCLE, xOffset, Game::TileSize / 2, 0, 0, mCircleSize[0], true, false);
	fbl_create_prim(FBL_CIRCLE, xOffset, Game::TileSize / 2, 0, 0, mCircleSize[1], true, false);
	fbl_create_prim(FBL_CIRCLE, xOffset, Game::TileSize / 2, 0, 0, mCircleSize[2], true, false);

	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, Game::TileSize / 2, 0, 0, mCircleSize[0], true, false);
	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, Game::TileSize / 2, 0, 0, mCircleSize[1], true, false);
	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, Game::TileSize / 2, 0, 0, mCircleSize[2], true, false);

	fbl_create_prim(FBL_CIRCLE, xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[0], true, false);
	fbl_create_prim(FBL_CIRCLE, xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[1], true, false);
	fbl_create_prim(FBL_CIRCLE, xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[2], true, false);

	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[0], true, false);
	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[1], true, false);
	fbl_create_prim(FBL_CIRCLE, Game::LogicalResW - xOffset, cMazeSizeY * Game::TileSize - Game::TileSize / 2, 0, 0, mCircleSize[2], true, false);

	mTimeBarId = fbl_create_prim(FBL_RECT, fbl_get_screen_w() / 2, (cMazeSizeY - 1) * Game::TileSize, mTimeToPick * 60, Game::TileSize / 2, 0, false, true);
	fbl_set_prim_color(mTimeBarId, 0, 255, 0, 255);

	mTimeBarRed = 0;

	mPickTimer = (mGetReadyTimer + mTimeToPick) * 60; // 8 seconds(i.e 3 + 5)


}

void Maze::pickStartPosition(Game& g) {

	// do some nice circle effects

	for (int i = 0; i < 3; i++) {

		mCircleSize[i]++;

		if(mCircleSize[i] > 55)
			mCircleSize[i] = 5;

	}

	for (int i = mFirstCircleId; i < (mFirstCircleId + 12); i++) {

		if ((i + 2) % 3 == 0)
			fbl_set_prim_size(i, 0, 0, mCircleSize[0]);
		else if ((i + 1) % 3 == 0)
			fbl_set_prim_size(i, 0, 0, mCircleSize[1]);
		else if (i % 3 == 0)
			fbl_set_prim_size(i, 0, 0, mCircleSize[2]);

	}


	// handle pick selection
	int xPickOffset = Game::TileSize * 6;
	int yPickOffset = Game::TileSize * 3;

	if (fbl_get_mouse_click(FBLMB_LEFT) > 0 && mPickTimer < mTimeToPick * 60) {	// if clicked and after GET READY phase 

		if(fbl_get_mouse_logical_x() < xPickOffset && fbl_get_mouse_logical_y() < yPickOffset){  // up left
			mPickedPosition = 0;	// this is the picked corner
			mPickTimer = -1; 		// stop the pick corner state
			assignPaths(g);			// assign new paths to the robots based on the selection
		}
		else if (fbl_get_mouse_logical_x() > Game::LogicalResW - xPickOffset && fbl_get_mouse_logical_y() < yPickOffset) {
			mPickedPosition = 1;
			mPickTimer = -1;
			assignPaths(g);
		}
		else if (fbl_get_mouse_logical_x() < xPickOffset && fbl_get_mouse_logical_y() > cMazeSizeY * Game::TileSize - yPickOffset) {
			mPickedPosition = 2;
			mPickTimer = -1;
			assignPaths(g);
		}
		else if (fbl_get_mouse_logical_x() > Game::LogicalResW - xPickOffset && fbl_get_mouse_logical_y() > cMazeSizeY * Game::TileSize - yPickOffset) {
			mPickedPosition = 3;
			mPickTimer = -1;
			assignPaths(g);
		}

		std::cout << "mouse xy: " << fbl_get_mouse_logical_x() << ", " << fbl_get_mouse_logical_y() << std::endl;

	}

	mPickTimer--;	//count down

	if(mPickTimer < (mTimeToPick + 1) * 60) {
		mBlackBgFade -= 4;
		fbl_set_prim_color(mBlackBgId, 0, 0, 0, mBlackBgFade);
	}

	if(mPickTimer == mTimeToPick * 60){
		fbl_set_prim_active(mBlackBgId, false);	// deactivate bg after the fadeout
		fbl_set_text_active(mGetReadyTextId, false);
	}

	// time bar
	if(mPickTimer < mTimeToPick * 60){
		fbl_set_prim_size(mTimeBarId, mPickTimer, Game::TileSize / 2, 0);
		int green_fade = mPickTimer; // 45
		if(green_fade > 255) green_fade = 255;
		if(green_fade < 1) green_fade = 0;
		fbl_set_prim_color(mTimeBarId, mTimeBarRed, green_fade, 0, 255);
		mTimeBarRed++;
		if(mTimeBarRed > 255) mTimeBarRed = 255;
		//print(mPickTimer - 45)
	}



	if (mPickTimer < 1) {


		if (mPickTimer == 0) {
			mPickedPosition = rand() % 4; // if time runs out, assign a random position
			assignPaths(g);
		}

		// create gui for the robots () coins health etc
		createGUI();
		// create the N/A images for unequipped addons
		createAddonSlots();
		// add the addon gui buttons
		showAddons(g);
		std::cout << "created GUI!" << std::endl;

		for (int i = mFirstCircleId; i < (mFirstCircleId + 12); i++)
			fbl_set_prim_active(i, false);

		fbl_set_text_active(mGetReadyTextId, false);
		fbl_set_prim_active(mBlackBgId, false);
		fbl_set_prim_active(mTimeBarId, false);

	}

}


void Maze::initMaze(Game& g, int density, int numRacers, int gameMode) {

	int tries = 0; // number of brute force tries

	sGameMode = gameMode;
	mNumRacers = numRacers;

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);	// in the race, sprites are drawn from the center bc. of physics :)

	setupPickStart();
	addBorder();
	fbl_set_clear_color(50, 50, 50, 0);

	// set up starting positions (make this part of showRobotInRace()?)
	mStartPos[0][0] = Game::TileSize * 3;
	mStartPos[0][1] = 0;
	mStartPos[1][0] = Game::LogicalResW - Game::TileSize * 4;
	mStartPos[1][1] = 0;
	mStartPos[2][0] = Game::TileSize * 3;
	mStartPos[2][1] = Game::TileSize * 16;
	mStartPos[3][0] = Game::LogicalResW - Game::TileSize * 4;
	mStartPos[3][1] = Game::TileSize * 16;

	// collect the path id's and stuff for the racing robots
	for (int i = 0; i < numRacers; i++) {
		auto& path = g.mEcs->GetComponent<Path>(g.mRobots->mRacingRobots[i]);
		auto& pos = g.mEcs->GetComponent<Position>(g.mRobots->mRacingRobots[i]);
		mPathId[i] = path.id;
		mStartPos[i][0] = pos.x;
		mStartPos[i][1] = pos.y;
		mUseDiag[i] = path.diag;

		path.goalX = cTargetX;
		path.goalY = cTargetY;
		path.newPath = false;

	}

	// set all paths to not started
	for (int i = 0; i < numRacers; i++)
		fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_NOT_STARTED);

	// brute force maze creation :) (with a density below 40 it's reasonable, 35 and below is really good (fast).)
	do {

		resetMaze();
		randomizeMaze(density);

		for (int i = 0; i < cMaxRacers; i++)
			fbl_pathf_set_path_status(i, fbl_pathf_find_path(i, mStartPos[i][0], mStartPos[i][1], cTargetX, cTargetY, true));

		tries++;

	}
	while (!mazeHasAllPaths());


	// make robots not move at first
	stopPathing();


	// set relevant tiles to walkable
	for (int i = 0; i < cMaxRacers; i++)
		fbl_pathf_set_walkability(mStartPos[i][0] / Game::TileSize, mStartPos[i][1] / Game::TileSize, FBL_PATHF_WALKABLE);

	fbl_pathf_set_walkability(cTargetX / Game::TileSize, cTargetY / Game::TileSize, FBL_PATHF_WALKABLE);

	// create all the sprites at correct locations
	populateMaze();

	// add the flag and coins
	addItems();
	
	for (int i = 0; i < numRacers; i++)
		std::cout << "Path status: " << fbl_pathf_get_path_status(mPathId[i]) << std::endl;
		
	std::cout << "has all paths: " << mazeHasAllPaths() << std::endl;

	std::cout << "Tries: " << tries << std::endl;

	std::cout << "Num sprites: " << fbl_get_num_sprites() << std::endl;


}

void Maze::stopPathing() {

	for (int i = 0; i < mNumRacers; i++)
		fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_NOT_STARTED);

}

void Maze::resetMaze() {

	for (int i = 3; i < cMazeSizeX - 3; i++)
		for (int j = 0; j < cMazeSizeY; j++)
			fbl_pathf_set_walkability(i, j, FBL_PATHF_WALKABLE); // set all to walkable

}


void Maze::randomizeMaze(int density) {

	for (int i = 3; i < cMazeSizeX - 3; i++)
		for (int j = 0; j < cMazeSizeY; j++)
			if ((rand() % 100) < density)
				fbl_pathf_set_walkability(i, j, FBL_PATHF_UNWALKABLE); // set these to unwalkable

}



void Maze::populateMaze() {

	// creates sprites with correct coordinates as obstacles

	for (int i = 3; i < cMazeSizeX - 3; i++) {
		for (int j = 0; j < cMazeSizeY; j++) {

			// add a block if it's unwalkable
			if (fbl_pathf_get_walkability(i, j) == FBL_PATHF_UNWALKABLE) {

				int id = fbl_create_sprite(32, 416, 32, 32, 0);
				fbl_set_sprite_xy(id, i * Game::TileSize + 16, j * Game::TileSize + 16);	// these get drawn from the center
				fbl_set_sprite_phys(id, true, FBL_RECT, FBL_PHYS_KINEMATIC, false);

			}

		}

	}

}

void Maze::showAddons(Game& g) {

	// hide all addons to begin with (necessary when one robot dies and the next starts)
	g.mAddons->hideAddons(g.mEcs);

	// first robot is always the players
	auto& sta = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[0]);

	for (int i = 0; i < 6; i++) {

		if (sta.slot[i] != g.mAddons->notSet) {		// only show addons that are equipped

			g.mAddons->showAddonInRace(g.mEcs, sta.slot[i], i);
			std::cout << "sta.slot[i]: " << sta.slot[i] << " i= " << i << std::endl;

		}

	}

}

void Maze::addItems() {


	// add flags in the middle
	for (int i = 0; i < cMaxFlags; i++) {
		sFlag[i].id = fbl_create_sprite(265, 324, 17, 24, 0);
		fbl_set_sprite_xy(sFlag[i].id, cTargetX + 16, cTargetY + 16);	// drawn from the center
		fbl_set_sprite_layer(sFlag[i].id, 6);
		sFlag[i].state = FlagState::Center;	// start in the center
	}

	// add random amount of coins (from 12 to 25)
	for (int i = 0; i < Maze::cMaxCoins; i++) sCoin[i].id = -1;	// set all id's to -1
	int numCoins = rand() % (Maze::cMaxCoins / 2) + 10;
	std::cout << "numCoins: " << numCoins << std::endl;
	for (int i = 0; i < numCoins; i++) {
		sCoin[i].id = fbl_create_sprite(320, 288, 16, 16, 0);
		fbl_set_sprite_layer(sCoin[i].id, 5);
		fbl_set_sprite_animation(sCoin[i].id, true, 320, 288, 16, 16, 2, 30, true);
		int x = 3 + rand() % (cMazeSizeX - 6);
		int y = rand() % cMazeSizeY;
		while (fbl_pathf_get_walkability(x, y) == FBL_PATHF_UNWALKABLE || (x * Game::TileSize == cTargetX && y * Game::TileSize == cTargetY)) {
			int x = 3 + rand() % (cMazeSizeX - 6);
			y = rand() % cMazeSizeY;
		}
		fbl_set_sprite_xy(sCoin[i].id, x * Game::TileSize + 16, y * Game::TileSize + 16);		// drawn from the center
	}

}

void Maze::addBorder() {

	// creates sprites with correct coordinates as obstacles

	for (int i = 0; i < cMazeSizeX + 6; i++) {
		for (int j = 0; j < cMazeSizeY; j++) {

			// draw a frame left and right of maze
			if ((i >= 0 && i < 3) || (i > 26 && i < 30)) {
				
				int id = fbl_create_sprite(64, 416, 32, 32, 0);
				fbl_set_sprite_xy(id, i * Game::TileSize + 16, j * Game::TileSize + 16);
				if(i == 2 || i == 27)
					fbl_set_sprite_phys(id, true, FBL_RECT, FBL_PHYS_KINEMATIC, false);
					
			}

		}

	}

	// add a row of unwalkable tiles at the very bottom, one step below screen (not visible)
	for (int i = 0; i < cMazeSizeX; i++)
		fbl_pathf_set_walkability(i, cMazeSizeY, FBL_PATHF_UNWALKABLE);

	// and two columns left and right of the maze
	for (int j = 0; j < cMazeSizeY; j++)
		fbl_pathf_set_walkability(2, j, FBL_PATHF_UNWALKABLE);
	for (int j = 0; j < cMazeSizeY; j++)
		fbl_pathf_set_walkability(27, j, FBL_PATHF_UNWALKABLE);

}

// create the gui in each corner showing how many flags and coins each robot has got, lifebars etc etc
void Maze::createGUI() {

	// 96x64 gui thing in all 4 corners
	int id = fbl_create_sprite(416, 288, 96, 64, 0);
	fbl_set_sprite_xy(id, 48, 32);
	fbl_set_sprite_layer(id, 1);

	id = fbl_create_sprite(416, 288, 96, 64, 0);
	fbl_set_sprite_xy(id, 912, 32);
	fbl_set_sprite_layer(id, 1);

	id = fbl_create_sprite(416, 288, 96, 64, 0);
	fbl_set_sprite_xy(id, 48, 512);
	fbl_set_sprite_layer(id, 1);

	id = fbl_create_sprite(416, 288, 96, 64, 0);
	fbl_set_sprite_xy(id, 912, 512);
	fbl_set_sprite_layer(id, 1);

	// add base markers (spinning circles) top left, top right, down left, down right, draw from center
	id = fbl_create_sprite(224, 352, 32, 32, 0);
	fbl_set_sprite_xy(id, 3 * Game::TileSize + 16, 16);
	fbl_set_sprite_animation(id, true, 224, 352, 32, 32, 9, 7, true);
	fbl_set_sprite_color(id, 255, 127, 80); // coral red
	fbl_set_sprite_alpha(id, 150);

	id = fbl_create_sprite(224, 352, 32, 32, 0);
	fbl_set_sprite_xy(id, (cMazeSizeX - 4) * Game::TileSize + 16, 16);
	fbl_set_sprite_animation(id, true, 224, 352, 32, 32, 9, 7, true);
	fbl_set_sprite_color(id, 80, 200, 120); // emerald green
	fbl_set_sprite_alpha(id, 150);

	id = fbl_create_sprite(224, 352, 32, 32, 0);
	fbl_set_sprite_xy(id, 3 * Game::TileSize + 16, (cMazeSizeY - 1) * Game::TileSize + 16);
	fbl_set_sprite_animation(id, true, 224, 352, 32, 32, 9, 7, true);
	fbl_set_sprite_color(id, 0, 150, 255); // bright blue
	fbl_set_sprite_alpha(id, 150);

	id = fbl_create_sprite(224, 352, 32, 32, 0);
	fbl_set_sprite_xy(id, (cMazeSizeX - 4) * Game::TileSize + 16, (cMazeSizeY - 1) * Game::TileSize + 16);
	fbl_set_sprite_animation(id, true, 224, 352, 32, 32, 9, 7, true);
	fbl_set_sprite_color(id, 251, 236, 93); // nice yellow
	fbl_set_sprite_alpha(id, 150);


	// text items
	fbl_load_ttf_font("font/garamond.ttf", 15);
	gui[0].levelTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 1);
	fbl_set_text_xy(gui[0].levelTextId, 38, 9);
	gui[1].levelTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 1);
	fbl_set_text_xy(gui[1].levelTextId, 902, 9);
	gui[2].levelTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 1);
	fbl_set_text_xy(gui[2].levelTextId, 38, 489);
	gui[3].levelTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 1);
	fbl_set_text_xy(gui[3].levelTextId, 902, 489);


	fbl_load_ttf_font("font/garamond.ttf", 16);
	gui[0].flagTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[0].flagTextId, 78, 26);
	gui[0].coinTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[0].coinTextId, 38, 26);

	gui[1].flagTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[1].flagTextId, 942, 26);
	gui[1].coinTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[1].coinTextId, 902, 26);

	gui[2].flagTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[2].flagTextId, 78, 506);
	gui[2].coinTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[2].coinTextId, 38, 506);

	gui[3].flagTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[3].flagTextId, 942, 506);
	gui[3].coinTextId = fbl_create_text(255, 255, 255, 255, (char*)"%d", 0);
	fbl_set_text_xy(gui[3].coinTextId, 902, 506);

	// red and blue hp/pow rects
	gui[0].hpRectId = fbl_create_prim(FBL_NORMAL_RECT, 33, 43, 50, 3, 0, false, true);
	//fbl_set_prim_color(gui[0].hpRectId, 0, 237, 55, 36);
	gui[0].powRectId = fbl_create_prim(FBL_NORMAL_RECT, 33, 53, 50, 3, 0, false, true);
	fbl_set_prim_color(gui[0].powRectId, 0, 162, 232, 255);

	gui[1].hpRectId = fbl_create_prim(FBL_NORMAL_RECT, 897, 43, 50, 3, 0, false, true);
	//fbl_set_prim_color(gui[0].hpRectId, 0, 237, 55, 36);
	gui[1].powRectId = fbl_create_prim(FBL_NORMAL_RECT, 897, 53, 50, 3, 0, false, true);
	fbl_set_prim_color(gui[1].powRectId, 0, 162, 232, 255);

	gui[2].hpRectId = fbl_create_prim(FBL_NORMAL_RECT, 33, 523, 50, 3, 0, false, true);
	//fbl_set_prim_color(gui[0].hpRectId, 0, 237, 55, 36);
	gui[2].powRectId = fbl_create_prim(FBL_NORMAL_RECT, 33, 533, 50, 3, 0, false, true);
	fbl_set_prim_color(gui[2].powRectId, 0, 162, 232, 255);

	gui[3].hpRectId = fbl_create_prim(FBL_NORMAL_RECT, 897, 523, 50, 3, 0, false, true);
	//fbl_set_prim_color(gui[0].hpRectId, 0, 237, 55, 36);
	gui[3].powRectId = fbl_create_prim(FBL_NORMAL_RECT, 897, 533, 50, 3, 0, false, true);
	fbl_set_prim_color(gui[3].powRectId, 0, 162, 232, 255);


	//  you have to sort the sprite list for the layer parameter to have effect
	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

void Maze::updateGUI(Game& g) {

	int barMaxWidth = 50;  // Maximum width of hp and power bar
	int barPercentage = 0; // how many % are filled?
	int barWidth = 0;	   // the final bar

	for (int i = 0; i < mNumRacers; i++) {

		auto& stat = g.mEcs->GetComponent<Stats>(g.mRobots->mRacingRobots[i]);
		auto& plog = g.mEcs->GetComponent<PathLogic>(g.mRobots->mRacingRobots[i]);

		// update number of flags, coins and hp + power (energy)
		if (plog.baseX == Game::TileSize * 3 && plog.baseY == 0)
			setOneUIbox(stat, plog, 0, g.mRobots->mRacingRobots[i]);
		if (plog.baseX == Game::LogicalResW - Game::TileSize * 4 && plog.baseY == 0)
			setOneUIbox(stat, plog, 1, g.mRobots->mRacingRobots[i]);
		if (plog.baseX == Game::TileSize * 3 && plog.baseY == Game::TileSize * 16)
			setOneUIbox(stat, plog, 2, g.mRobots->mRacingRobots[i]);
		if (plog.baseX == Game::LogicalResW - Game::TileSize * 4 && plog.baseY == Game::TileSize * 16)
			setOneUIbox(stat, plog, 3, g.mRobots->mRacingRobots[i]);

	}

}

void Maze::setOneUIbox(Stats stat, PathLogic plog, int base, int entity) {

	int barMaxWidth = 50; // Maximum width of hp pand power bar
	double barPercentage = 0.0;
	int barWidth = 0;

	// update number of flags, coins and level
	if (gui[base].level != stat.level) {
		fbl_update_text(gui[base].levelTextId, 255, 255, 255, 255, (char*)"%d", stat.level);
		gui[base].level = stat.level;
		std::cout << "updated level text for player " << entity << std::endl;
	}
	if (gui[base].coins != plog.coins) {
		fbl_update_text(gui[base].coinTextId, 255, 255, 255, 255, (char*)"%d", plog.coins);
		gui[base].coins = plog.coins;
		std::cout << "updated coin text for player " << entity << std::endl;
	}
	if (gui[base].flags != plog.flags) {
		fbl_update_text(gui[base].flagTextId, 255, 255, 255, 255, (char*)"%d", plog.flags);
		gui[base].flags = plog.flags;
		std::cout << "updated flag text for player " << entity << std::endl;
	}

	// hp and power meters
	barPercentage = static_cast<double>(stat.hp) / stat.maxHp;  // calc the percentage
	barWidth = std::round(barMaxWidth * barPercentage);			// calc width of the bar
	fbl_set_prim_size(gui[base].hpRectId, barWidth, 3, 0);		// resize the rect
	// same with power (energy) bar
	barPercentage = static_cast<double>(stat.energy) / stat.maxEnergy;
	barWidth = std::round(barMaxWidth * barPercentage);
	fbl_set_prim_size(gui[base].powRectId, barWidth, 3, 0);

}

void Maze::createAddonSlots() {

	// here follows the N/A images for the unequipped addons

	int tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::TileSize * 2 - Game::TileSize / 2, Game::TileSize * 4);
	fbl_set_sprite_layer(tempId, 2);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2, Game::TileSize * 4);
	fbl_set_sprite_layer(tempId, 2);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::TileSize * 2 - Game::TileSize / 2, Game::TileSize * 8);
	fbl_set_sprite_layer(tempId, 2);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2, Game::TileSize * 8);
	fbl_set_sprite_layer(tempId, 2);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::TileSize * 2 - Game::TileSize / 2, Game::TileSize * 11);
	fbl_set_sprite_layer(tempId, 2);

	tempId = fbl_create_sprite(384, 288, Game::TileSize, Game::TileSize, 0);
	fbl_set_sprite_xy(tempId, Game::LogicalResW - Game::TileSize * 2 + Game::TileSize / 2, Game::TileSize * 11);
	fbl_set_sprite_layer(tempId, 2);

	// and some text
	fbl_load_ttf_font("font/garamond.ttf", 16);
	tempId = fbl_create_text(255, 255, 255, 255, (char*)"Passive");
	fbl_set_text_xy(tempId, Game::TileSize - 10, Game::TileSize * 3);

	tempId = fbl_create_text(255, 255, 255, 255, (char*)"Passive");
	fbl_set_text_xy(tempId, Game::LogicalResW - Game::TileSize * 2 - 10, Game::TileSize * 3);

	// and some keys to press :)
	tempId = fbl_create_text(255, 255, 255, 255, (char*)"A");
	fbl_set_text_xy(tempId, Game::TileSize * 2 - 22, Game::TileSize * 7);

	tempId = fbl_create_text(255, 255, 255, 255, (char*)"S");
	fbl_set_text_xy(tempId, Game::LogicalResW - Game::TileSize * 2 + 10, Game::TileSize * 7);

	tempId = fbl_create_text(255, 255, 255, 255, (char*)"Z");
	fbl_set_text_xy(tempId, Game::TileSize * 2 - 22, Game::TileSize * 10);

	tempId = fbl_create_text(255, 255, 255, 255, (char*)"X");
	fbl_set_text_xy(tempId, Game::LogicalResW - Game::TileSize * 2 + 10, Game::TileSize * 10);

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

bool Maze::mazeHasAllPaths() {

	for (int i = 0; i < cMaxRacers; i++)
		if (fbl_pathf_get_path_status(i) != FBL_PATHF_FOUND)
			return false;

	return true;

}


void Maze::assignPaths(Game& g) {

	// randomize paths, then assign mPickedPosition to the player and position everything

	int arr[] = { 0, 1, 2, 3 };	// create ordered array with starting positions
	
	// shuffle the array using the Fisher-Yates algorithm
	for (int i = cMaxRacers - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
	
	// find if a robot has the picked path, swap it with the players path (even if it's the player)
	for (int i = 0; i < cMaxRacers; i++) {
		if(arr[i] == mPickedPosition) {	// found it
			// swap
			arr[i] = arr[0];
			arr[0] = mPickedPosition;
			break;	// break loop
		}
	}

	// place robots	at the new random locations
	for (int i = 0; i < mNumRacers; i++)
		g.mRobots->showRobotInRace(g.mEcs, g.mRobots->mRacingRobots[i], arr[i]);

	for (int i = 0; i < mNumRacers; i++) {
		auto& pos = g.mEcs->GetComponent<Position>(g.mRobots->mRacingRobots[i]);
		auto& plog = g.mEcs->GetComponent<PathLogic>(g.mRobots->mRacingRobots[i]);
		mStartPos[i][0] = pos.x;
		mStartPos[i][1] = pos.y;

		// set the robots base coords
		plog.baseX = pos.x;
		plog.baseY = pos.y;

	}

	// finally find paths for the new locations (will find immediately, maze is already in place (maze is a place))
	for (int i = 0; i < mNumRacers; i++)
		fbl_pathf_set_path_status(mPathId[i], fbl_pathf_find_path(mPathId[i], mStartPos[i][0], mStartPos[i][1], cTargetX, cTargetY, mUseDiag[i]));

	// don't start immediately
	stopPathing();
	
}
