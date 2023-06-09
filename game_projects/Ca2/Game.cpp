/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Game.cpp
*
*   Game class implementation, takes care of initializing the first startup, also has instances of all the subsystems.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "Ecs/Ecs.hpp"
#include "Game.hpp"
#include "SysManager.hpp"
#include "SoundManager.hpp"
#include "UserInput.hpp"
#include "Chars.hpp"
#include "Robots.hpp"
#include "Addons.hpp"
#include "Location.hpp"
#include "Objects.hpp"
#include "Weather.hpp"
#include "Progress.hpp"
#include "GameState/GameState.hpp"


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

	//std::cout.setstate(std::ios_base::failbit);	// deactivate cout

	// set this as default
	//DeviceResW = LogicalResW;
	//DeviceResH = LogicalResH;

	fbl_engine_init(DeviceResW, DeviceResH, 60);
	fbl_set_render_logical_size(DeviceResW, DeviceResH);
	fbl_set_clear_color(11, 168, 230, 255);
	//fbl_set_clear_color(33, 68, 33, 255);	// forest green
	//fbl_set_clear_color(35, 128, 45, 255);	// light forest green

	// show device current res, comment this out.
	int w, h;
	fbl_get_device_res(&w, &h);
	fbl_load_ttf_font("font/garamond.ttf", 16);
	fbl_create_text(255, 255, 255, 255, (char*)"native res: %d x %d", w, h);
	fbl_set_text_xy(0, 10, 20);


	//fbl_create_threadpool();
	fbl_phys_init();
	fbl_phys_set_gravity(0, 0);

	// init pathfinding
	if (fbl_pathf_init() == FBL_PATHF_OUT_OF_MEM) {
		// do stuff if no mem
	}

	fbl_load_texture((char*)"spritesheet.png");	// load sprite texture
	fbl_load_ui_texture((char*)"ui_1.png");			// load ui texture

	// fbl_lua_init("Ca2Dialogue.lua", registerFuncsToLua);

	// create instances of the Game-class sub systems
	gEditor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI
	mMap = gEditor;					// assign gEditor pointer to mMap, so we can avoid global state. Only use mMap after this.
	mEcs = new Coordinator();
	mSysManager = new SysManager();
	mSound = new SoundManager();
	mState = new GameState();
	mInput = new UserInput();
	mChars = new Chars();
	mRobots = new Robots();
	mAddons = new Addons();
	mLocation = new Location();
	mObjects = new Objects();
	mWeather = new Weather();
	mProgress = new Progress();

	std::cout << "starting sysmanager setup: " << std::endl;
	mSysManager->setupEcs(mEcs);
	std::cout << "done with sysmanager setup: " << std::endl;

	// start the snow particle effect
	//mWeather->setWeather(Weather::TimeOfDay::Day, 0, 6, 50, false);
	// start nice clouds
	mWeather->setWeather(Weather::TimeOfDay::Day, 0, 14, 80, false);

	return true;

}

void Game::unInit() {

	mMap->resetMap(0, 0);	// free tile-mem

	delete mEcs;
	delete mMap;
	delete mSysManager;
	delete mSound;
	delete mState;
	delete mInput;
	delete mChars;
	delete mRobots;
	delete mAddons;
	delete mLocation;
	delete mObjects;
	delete mWeather;
	delete mProgress;

	std::cout.clear();

}

void Game::update() {

	mInput->tick(*this);	// get general user input (get specific input from the different states' tick())
	mState->tick(*this);	// update the current state

}
