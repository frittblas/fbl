/*
*
*	Lotr
*
*	Init.cpp
*
*   Init class implementation, takes care of initializing the first startup and each level.
*   Singleton.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "../../tools/ScenEdit/Disk.hpp"

#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Ecs/Systems/PhysicsSystem.hpp"

#include "GameState/GameState.hpp"
#include "Init.hpp"

// the only global objects, prefixed with g
ScenEdit* gEditor;	// pointer to the map with optional editor
Coordinator gEcs;	// the Entity Component System
GameState gState;	// current game state

Init Init::instance;

// Init-class implementation

Init::Init() {}
Init::~Init() {}

Init& Init::getInstance() {

	return instance;

}

bool Init::initLotr() {

	fbl_engine_init(960, 540, 60);

	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture

	gEditor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI

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

void Init::unInitLotr() {

	gEditor->resetMap(0, 0);	// free tile-mem
	delete gEditor;

}

void Init::initLevel(uint32_t level) {

	bool success = Disk::getInstance().loadMap(*gEditor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

}
