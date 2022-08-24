/*
*
*	Lotr
*
*	Game.cpp
*
*   Game class implementation, takes care of initializing the first startup and to load each level.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "../../tools/ScenEdit/Disk.hpp"

#include "Ecs/Ecs.hpp"

#include "Game.hpp"

#include "SysManager.hpp"
#include "UserInput.hpp"
#include "Chars.hpp"
#include "Objects.hpp"
#include "Progress.hpp"
#include "GameState/GameState.hpp"

#include "LuaDialogue.hpp"

// the only global object (file scope!), the map, with optional editor, prefixed with g
// this is assigned to the Game-class member variable mMap, so there is no real global state at all.
// Don't use this. Use mMap instead.
ScenEdit* gEditor;	// pointer to the map with optional editor, has to be called gEditor bc it's externed in GuiFuncs.cpp

// Game-class implementation

Game::Game() {

	init();

}
Game::~Game() {

	unInit();

}

bool Game::init() {

	fbl_engine_init(960, 540, 60);
	fbl_set_render_logical_size(LogicalResW, LogicalResH);
	fbl_set_clear_color(33, 68, 33, 255);	// forest green
	//fbl_create_threadpool();


	// init pathfinding
	if (fbl_pathf_init() == FBL_PATHF_OUT_OF_MEM) {
		// do stuff if no mem
	}

	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture
	fbl_load_ui_texture((char*)"ui.png");			// load ui texture

	fbl_lua_init("LotrDialogue.lua", registerFuncsToLua);

	// create instances of the Game-class sub systems
	gEditor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI
	mMap = gEditor;					// assign gEditor pointer to mMap, so we can avoid global state. Only use mMap after this.
	mEcs = new Coordinator();
	mSysManager = new SysManager();
	mState = new GameState();
	mInput = new UserInput();
	mChars = new Chars();
	mObjects = new Objects();
	mProgress = new Progress();

	mSysManager->setupEcs(mEcs);

	mChars->setupPlayer(mEcs, mSysManager);

	return true;

}

void Game::unInit() {

	mMap->resetMap(0, 0);	// free tile-mem

	delete mMap;
	delete mSysManager;
	delete mState;
	delete mInput;
	delete mChars;
	delete mObjects;
	delete mProgress;

}

void Game::update() {

	mInput->tick(*this);	// get user input
	mState->tick(*this);	// update the current state

}

void Game::loadLevel() {

	bool success = Disk::getInstance().loadMap(*mMap, "assets/map.scn"); // this calls fbl_destroy_all_sprites()

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

	// set up the map for path finding
	for (int i = 0; i < fbl_pathf_get_map_w(); i++) {
		for (int j = 0; j < fbl_pathf_get_map_h(); j++) {
			fbl_pathf_set_walkability(i, j, FBL_PATHF_WALKABLE);
		}
	}

	// set tiles to walkable/unwalkable

	// fbl_pathf_get_map_w() and mMap->mapWidth are the same, always 60*34

	for (int i = 0; i < fbl_pathf_get_map_w(); i++) {
		for (int j = 0; j < fbl_pathf_get_map_h(); j++) {
			int index = i + mMap->mapWidth * j;
			if (mMap->tile[index] != nullptr)
				if (mMap->tile[index]->kinematic)
					fbl_pathf_set_walkability(i, j, FBL_PATHF_UNWALKABLE);
		}
	}

	/*
	// this also works fine, 1d array to 2d coords :)
	for (uint32_t i = 0; i < (mMap->mapWidth * mMap->mapHeight); i++) {
		if (mMap->tile[i] != nullptr) {
			if (mMap->tile[i]->kinematic) {
				int x = i % mMap->mapWidth;
				int y = i / mMap->mapWidth;
				fbl_pathf_set_walkability(x, y, FBL_PATHF_UNWALKABLE);
			}
		}
	}
	*/

	mChars->setupPlayerGfx(mEcs, mSysManager);

}

void Game::unLoadLevel() {

	mMap->resetMap(0, 0);

}
