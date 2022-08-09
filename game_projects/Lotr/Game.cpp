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
#include "Ecs/Components.hpp"

#include "Ecs/Systems/SpriteSystem.hpp"
#include "Ecs/Systems/PathSystem.hpp"

#include "Game.hpp"

#include "SysManager.hpp"
#include "UserInput.hpp"
#include "Chars.hpp"
#include "Objects.hpp"
#include "Progress.hpp"
#include "GameState/GameState.hpp"

// the only global object (file scope!), the map, with optional editor, prefixed with g
// this is assigned to the Game-class member variable mMap, so there is no global state at all.
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
	fbl_set_clear_color(33, 68, 33, 255);	// forest green
	//fbl_create_threadpool();

	// init pathfinding
	if (fbl_pathf_init() == FBL_PATHF_OUT_OF_MEM) {
		// do stuff if no mem
	}

	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture

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

	// init the Ecs
	mEcs->Init();
	
	// register components
	mEcs->RegisterComponent<Position>();
	mEcs->RegisterComponent<Sprite>();
	mEcs->RegisterComponent<Path>();

	// register systems
	//auto spriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	mSysManager->mSpriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	mSysManager->mPathSystem = mEcs->RegisterSystem<PathSystem>();

	// set up what components the systems require
	Signature sig1;
	sig1.set(mEcs->GetComponentType<Position>());
	sig1.set(mEcs->GetComponentType<Sprite>());
	mEcs->SetSystemSignature<SpriteSystem>(sig1);

	Signature sig2;
	sig2.set(mEcs->GetComponentType<Position>());
	sig2.set(mEcs->GetComponentType<Path>());
	mEcs->SetSystemSignature<PathSystem>(sig2);

	// create the player entity
	mChars->mFrodo = mEcs->CreateEntity();

	// add components to the entity
								  //	 x   y
	mEcs->AddComponent(mChars->mFrodo, Position{ 64, 64 });
								  //  id id id id num tx ty   w   h   anim fr spd dir dirl
	mEcs->AddComponent(mChars->mFrodo, Sprite{ 0, 0, 0, 0, 4, 0, 224, 32, 32, true, 2, 12, 1, 1 });
										//	x	y
	mEcs->AddComponent(mChars->mFrodo, Path{ 0, 256, 128, true });

	mSysManager->mSpriteSystem->Init(*this->mEcs);
	mSysManager->mPathSystem->Init(*this->mEcs);

	/*
	auto& pos = mEcs->GetComponent<Position>(0);
	std::cout << pos.x << std::endl;
	*/
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

	// set up graphics for the player
	mSysManager->mSpriteSystem->Init(*this->mEcs);

}

void Game::unLoadLevel() {

	mMap->resetMap(0, 0);

}
