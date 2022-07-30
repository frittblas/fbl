/*
*
*	Lotr
*
*	GameState.cpp
*
*   Init class implementation, takes care of initializing the first startup and each level.
*
*	Hans Strömquist 2022
*
*/

#include "Init.hpp"

ScenEdit* gEditor;
Coordinator gEcs;

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