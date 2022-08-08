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
#include "Ecs/Systems/SpriteSystem.hpp"

#include "Game.hpp"
#include "UserInput.hpp"
#include "Progress.hpp"
#include "GameState/GameState.hpp"

// the only global object (file scope!), the map, with optional editor, prefixed with g
// this is assigned to the Game-class member variable mMap, so there is no global state at all.
ScenEdit* gEditor;	// pointer to the map with optional editor, has to be called gEditor bc it's externed in GuiFuncs.cpp

std::shared_ptr<SpriteSystem> s;

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

	// init the Ecs
	mEcs->Init();
	
	// register components
	mEcs->RegisterComponent<Position>();
	mEcs->RegisterComponent<Sprite>();

	// register systems
	auto spriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	s = spriteSystem;

	// set up what components the systems require
	Signature signature;
	signature.set(mEcs->GetComponentType<Position>());
	signature.set(mEcs->GetComponentType<Sprite>());
	mEcs->SetSystemSignature<SpriteSystem>(signature);

	// create the player entity
	Entity player = mEcs->CreateEntity();

	// add components to the entity
	mEcs->AddComponent(player, Position{64, 64});
	//								 id id id id num tx ty   w   h   anim fr spd dir dirl
	mEcs->AddComponent(player, Sprite{0, 0, 0, 0, 4, 0, 224, 32, 32, true, 2, 12, 1, 1});

	spriteSystem->Init(*this->mEcs);

	/*
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
	*/
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
	mState->tick(*this);			// update the current state

	s->Update(*this->mEcs);

}

void Game::loadLevel() {

	bool success = Disk::getInstance().loadMap(*gEditor, "assets/map.scn"); // this calls fbl_destroy_all_sprites()

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

	// set up graphics for the player
	s->Init(*this->mEcs);

}

void Game::unLoadLevel() {

	mMap->resetMap(0, 0);

}
