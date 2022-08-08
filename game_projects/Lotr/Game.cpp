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
#include "Ecs/Systems/PhysicsSystem.hpp"

#include "Game.hpp"
#include "UserInput.hpp"
#include "Progress.hpp"
#include "GameState/GameState.hpp"

// the only global object (file scope!), the map, with optional editor, prefixed with g
// this is assigned to the Game-class member variable mMap, so there is no global state at all.
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

	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture

	// create instances of the Game-class sub systems
	gEditor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI
	mMap = gEditor;					// assign gEditor pointer to mMap, so we can avoid global state. Only use mMap after this.
	mEcs = new Coordinator();
	mState = new GameState();
	mInput = new UserInput();
	mProgress = new Progress();

	mEcs->Init();
	mEcs->RegisterComponent<Position>();
	auto physicsSystem = mEcs->RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(mEcs->GetComponentType<Position>());
	mEcs->SetSystemSignature<PhysicsSystem>(signature);

	std::vector<Entity> entities(MAX_ENTITIES);

	for (auto& entity : entities)
	{
		entity = mEcs->CreateEntity();
		mEcs->AddComponent(
			entity,
			Position{ 0, 0 }
		);
	}

	physicsSystem->Init(*this->mEcs);

	int quit = 100;
	while (quit > 0)
	{
		physicsSystem->Update(*this->mEcs);
		quit--;
	}

	auto& pos = mEcs->GetComponent<Position>(0);
	std::cout << pos.x << std::endl;

	return true;

}

void Game::unInit() {

	mMap->resetMap(0, 0);	// free tile-mem

	delete mMap;
	delete mState;
	delete mInput;
	delete mProgress;

}

void Game::update() {

	mInput->tick(*this);	// get user input
	mState->tick();			// update the current state

}


void Game::loadLevel() {

	bool success = Disk::getInstance().loadMap(*gEditor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

}

void Game::unLoadLevel() {

	mMap->resetMap(0, 0);

}
