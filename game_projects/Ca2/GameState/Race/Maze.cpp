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
#include "Maze.hpp"


// Maze-class implementation

Maze::Maze() {

	std::cout << "Maze constructor." << std::endl;

}

Maze::~Maze() {

	std::cout << "Maze destructor." << std::endl;

}

void Maze::tick(Game& g) {

	if (mPickTimer > 0)
		pickStartPosition(g);
	else {

		if (mPickTimer == -150) { // wait a little before starting
			// make robots move
			for (int i = 0; i < mNumRacers; i++) fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_FOUND);
			std::cout << "Picked pos: " << mPickedPosition << std::endl;
			std::cout << "Running! Num sprites: " << fbl_get_num_sprites() << std::endl;
			mPickTimer--;
		}
		else if(mPickTimer > -160)
			mPickTimer--;

	}

}

void Maze::setupPickStart() {

	// black bg
	mBlackBgId = fbl_create_prim(FBL_NORMAL_RECT, 0, 0, Game::DeviceResW, Game::DeviceResH, 0, 0, 1);
	fbl_set_prim_color(mBlackBgId, 0, 0, 0, 255);
	mBlackBgFade = 255;

	// get ready text
	fbl_load_ttf_font("font/garamond.ttf", 48);
	mGetReadyTextId = fbl_create_text(255, 69, 0, 255, "GET READY!");
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

		for (int i = mFirstCircleId; i < (mFirstCircleId + 12); i++)
			fbl_set_prim_active(i, false);

		fbl_set_text_active(mGetReadyTextId, false);
		fbl_set_prim_active(mBlackBgId, false);
		fbl_set_prim_active(mTimeBarId, false);

	}

}


void Maze::initMaze(Game& g, int density, int numRacers) {

	int tries = 0; // number of brute force tries

	fbl_set_sprite_align(FBL_SPRITE_ALIGN_CENTER);	// in the race, sprites are drawn from the center bc. of physics :)

	setupPickStart();

	mNumRacers = numRacers;

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
		mStartPos[i][0] = pos.x;
		mStartPos[i][1] = pos.y;
	}

	// finally find paths for the new locations (will find immediately, maze is already in place (maze is a place))
	for (int i = 0; i < mNumRacers; i++)
		fbl_pathf_set_path_status(mPathId[i], fbl_pathf_find_path(mPathId[i], mStartPos[i][0], mStartPos[i][1], cTargetX, cTargetY, mUseDiag[i]));

	// don't start immediately
	stopPathing();
	
}
