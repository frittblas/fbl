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
#include "GameState/GameState.hpp"
#include "UserInput.hpp"

// the only global objects, the map, with optional editor, prefixed with g
extern Game* gGame;
ScenEdit* gEditor;	// pointer to the map with optional editor
Coordinator gEcs;	// the Entity Component System
UserInput gInput;	// keyboard and mouse input from the user
GameState* gState;	// current game state

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

	gEditor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI
	gState = new GameState();

	gEcs.Init();

	gEcs.RegisterComponent<Position>();

	auto physicsSystem = gEcs.RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(gEcs.GetComponentType<Position>());
	gEcs.SetSystemSignature<PhysicsSystem>(signature);

	std::vector<Entity> entities(MAX_ENTITIES);


	for (auto& entity : entities)
	{
		entity = gEcs.CreateEntity();

		gEcs.AddComponent(
			entity,
			Position{ 0, 0 }
		);

	}

	physicsSystem->Init();

	int quit = 100;

	while (quit > 0)
	{

		physicsSystem->Update();

		quit--;
	}

	auto& pos = gEcs.GetComponent<Position>(0);

	std::cout << pos.x << std::endl;

	return true;

}

void Game::unInit() {

	gEditor->resetMap(0, 0);	// free tile-mem
	delete gEditor;

	delete gState;
}

void Game::update() {

	gInput.tick(*gGame);	// get user input
	gState->tick(); // update the current state

}


void Game::loadLevel() {

	bool success = Disk::getInstance().loadMap(*gEditor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

}

void Game::unLoadLevel() {

	gEditor->resetMap(0, 0);

}
