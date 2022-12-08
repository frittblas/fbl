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
			std::cout << "Running!" << std::endl;
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

		if(fbl_get_mouse_x() < xPickOffset && fbl_get_mouse_y() < yPickOffset){  // up left
			mPickedPosition = 0;	// this is the picked corner
			mPickTimer = -1; 		// stop the pick corner state
			assignPaths(g);			// assign new paths to the robots based on the selection
		}
		else if (fbl_get_mouse_x() > Game::LogicalResW - xPickOffset && fbl_get_mouse_y() < yPickOffset) {
			mPickedPosition = 1;
			mPickTimer = -1;
			assignPaths(g);
		}
		else if (fbl_get_mouse_x() < xPickOffset && fbl_get_mouse_y() > cMazeSizeY * Game::TileSize - yPickOffset) {
			mPickedPosition = 2;
			mPickTimer = -1;
			assignPaths(g);
		}
		else if (fbl_get_mouse_x() > Game::LogicalResW - xPickOffset && fbl_get_mouse_y() > cMazeSizeY * Game::TileSize - yPickOffset) {
			mPickedPosition = 3;
			mPickTimer = -1;
			assignPaths(g);
		}

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

	setupPickStart();

	mNumRacers = numRacers;

	addBorder();
	fbl_set_clear_color(50, 50, 50, 0);

	int targetX = 15 * Game::TileSize;
	int targetY = 8 * Game::TileSize;

	// collect the path id's and stuff for the racing robots
	for (int i = 0; i < numRacers; i++) {
		auto& path = g.mEcs->GetComponent<Path>(g.mRobots->mRacingRobots[i]);
		auto& pos = g.mEcs->GetComponent<Position>(g.mRobots->mRacingRobots[i]);
		mPathId[i] = path.id;
		mStartPos[i][0] = pos.x;
		mStartPos[i][1] = pos.y;
		mUseDiag[i] = path.diag;

		path.goalX = targetX;
		path.goalY = targetY;
		path.newPath = false;

	}

	// set all paths to not started
	for (int i = 0; i < numRacers; i++)
		fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_NOT_STARTED);

	// brute force maze creation :) (with a density below 40 it's reasonable, 35 is really good.)
	do {

		resetMaze();
		randomizeMaze(density);

		for (int i = 0; i < numRacers; i++)
			fbl_pathf_set_path_status(mPathId[i], fbl_pathf_find_path(mPathId[i], mStartPos[i][0], mStartPos[i][1], targetX, targetY, mUseDiag[i]));

		tries++;

	}
	while (!mazeHasAllPaths(numRacers));


	// make robots not move at first
	for (int i = 0; i < mNumRacers; i++) fbl_pathf_set_path_status(mPathId[i], FBL_PATHF_NOT_STARTED);


	// set relevant tiles to walkable
	for (int i = 0; i < numRacers; i++)
		fbl_pathf_set_walkability(mStartPos[i][0] / Game::TileSize, mStartPos[i][1] / Game::TileSize, FBL_PATHF_WALKABLE);

	fbl_pathf_set_walkability(targetX / Game::TileSize, targetY / Game::TileSize, FBL_PATHF_WALKABLE);

	// create all the sprites at correct locations
	populateMaze();

	
	for (int i = 0; i < numRacers; i++)
		std::cout << "Path status: " << fbl_pathf_get_path_status(mPathId[i]) << std::endl;
		
	std::cout << "has all paths: " << mazeHasAllPaths(numRacers) << std::endl;

	std::cout << "Tries: " << tries << std::endl;

	std::cout << "Num sprites: " << fbl_get_num_sprites() << std::endl;

}

void Maze::exitMaze() {

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
				fbl_set_sprite_xy(id, i * Game::TileSize, j * Game::TileSize);

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

				//int id = fbl_create_sprite(64, 416, 32, 32, 0);
				//fbl_set_sprite_xy(id, i * Game::TileSize, j * Game::TileSize);

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


bool Maze::mazeHasAllPaths(int numRacers) {

	for (int i = 0; i < numRacers; i++) {
		if (fbl_pathf_get_path_status(mPathId[i]) != FBL_PATHF_FOUND)
			return false;
	}

	return true;

}


void Maze::assignPaths(Game& g) {

	// randomize paths, then assign mPickedPosition to the player and position everything

	int arr[] = { mPathId[0], mPathId[1], mPathId[2], mPathId[3] };	// create ordered array with path id's as values

	std::cout << "before: " << std::endl;
	for (int i = 0; i < 4; i++)
		std::cout << "arr[i]: " << arr[i] << std::endl;


	// shuffle the array using the Fisher-Yates algorithm
	for (int i = mNumRacers - 1; i > 0; i--) {

		int j = rand() % (i + 1);

		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}

	std::cout << "efter: " << std::endl;
	for (int i = 0; i < 4; i++)
		std::cout << "arr[i]: " << arr[i] << std::endl;

	// set new random path id's
	for (int i = 0; i < mNumRacers; i++) {
		mPathId[i] = arr[i];
	}

	/*

	// assign unique random path id's to the robots
	for (int i = 0; i < mNumRacers; i++) {
		auto& path = g.mEcs->GetComponent<Path>(g.mRobots->mRacingRobots[i]);
		mPathId[i] = path.id;

		path.goalX = targetX;
		path.goalY = targetY;
		path.newPath = false;

	}
	


	// find the robot who has the picked path, and swap it with the players path(even if it's the player)
	for (int i = 0; i < mNumRacers; i++) {

		if(g_robot[i][R_PATH] == mPickedPosition) {	// found it

			// swap
			g_robot[i][R_PATH] = g_robot[PLAYER][R_PATH]
			g_robot[PLAYER][R_PATH] = mPickedPosition

			break;	// break loop

		}

	}
	*/
	/*

		// set new coordinates for the robots
		for(int i = 0; i < mNumRacers; i++) {

			if g_robot[i][R_PATH] == 0 then
				g_robot[i][R_X] = 0
				g_robot[i][R_Y] = 0
				elseif g_robot[i][R_PATH] == 1 then
				g_robot[i][R_X] = (cMazeSizeX - 1) * Game::TileSize
				g_robot[i][R_Y] = 0
				elseif g_robot[i][R_PATH] == 2 then
				g_robot[i][R_X] = 0
				g_robot[i][R_Y] = (cMazeSizeY - 1) * Game::TileSize
				elseif g_robot[i][R_PATH] == 3 then
				g_robot[i][R_X] = (cMazeSizeX - 1) * Game::TileSize
				g_robot[i][R_Y] = (cMazeSizeY - 1) * Game::TileSize
				end

		}*/

		// place robots	
		g.mRobots->showRobotInRace(g.mEcs, Robots::Charmy, 0);
		g.mRobots->showRobotInRace(g.mEcs, Robots::Alarmy, 1);
		g.mRobots->showRobotInRace(g.mEcs, Robots::Boingy, 2);
		g.mRobots->showRobotInRace(g.mEcs, Robots::Chompy, 3);
		
	
}