/*
*
*	FBL ENGINE
*
*	Lotr.cpp
*
*	Contains the 3 required fbl functions for Lotr.
*	
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "../../tools/ScenEdit/Disk.hpp"

#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Ecs/Systems/PhysicsSystem.hpp"

ScenEdit* editor;
Coordinator gEcs;

void fbl_start()
{
	
	// make sure that the name of the asset folder is set to what you want (default = "assets/")
	// fbl_set_assets_folder_name("your_name/");

	fbl_engine_init(960, 540, 60);
	
	fbl_load_texture((char*)"spritesheet_.png");	// load sprite texture

	editor = new ScenEdit(false);	// create new instance of ScenEdit without editor GUI

	bool success = Disk::getInstance().loadMap(*editor, "assets/map.scn");

	if (success)
		std::cout << "Loaded map from assets/map.scn" << std::endl;
	else
		std::cout << "Error loading map!" << std::endl;

	/////////


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


	////////


}

void fbl_game_loop()
{

	if (fbl_get_key_down(FBLK_ESCAPE))
		fbl_engine_quit();

	if (fbl_get_key_down(FBLK_F9))
		fbl_set_window_mode(0);
	if (fbl_get_key_down(FBLK_F10))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN);
	if (fbl_get_key_down(FBLK_F11))
		fbl_set_window_mode(FBL_WINDOW_FULLSCREEN_DESKTOP);

}

void fbl_end()
{

	editor->resetMap(0, 0);	// free tile-mem
	delete editor;

	std::cout<<"Bye!"<<std::endl;

}
