/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Dungeon.cpp
*
*   Dungeon class implementation, takes care of the Dungeon game-state.
*
*	Hans Strömquist 2022
*
*/


#include "../../../../src/fbl.hpp"
#include "../../Game.hpp"
#include "../../SysManager.hpp"

#include "../../Ecs/Systems/SpriteSystem.hpp"
#include "../../Ecs/Systems/CameraSystem.hpp"
#include "../../Ecs/Systems/LightSystem.hpp"

#include "../../Weather.hpp"
#include "../../Chars.hpp"
#include "../GameState.hpp"
#include "Dungeon.hpp"


// Dungeon-class implementation

Dungeon::Dungeon() {

	std::cout << "Started Dungeon state." << std::endl;

}

Dungeon::~Dungeon() {

	std::cout << "Destroyed Dungeon state." << std::endl;

}

void Dungeon::processInput(Game& g) {


}

void Dungeon::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system
	g.mSysManager->mLightSystem->Update(g);					// update the light system

	//g.mWeather->tick();

	processInput(g);

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick dungeon!" << std::endl;

}
