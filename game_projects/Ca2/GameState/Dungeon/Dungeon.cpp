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
#include "../../Ecs/Ecs.hpp"
#include "../../Game.hpp"
#include "../../SysManager.hpp"

#include "../../Ecs/Systems/SpriteSystem.hpp"
#include "../../Ecs/Systems/CameraSystem.hpp"
#include "../../Ecs/Systems/LightSystem.hpp"

#include "../../Efx.hpp"
#include "../../Weather.hpp"
#include "../../Chars.hpp"
#include "../../Deck.hpp"
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

	static int access = 0;

	if (fbl_get_key_down(FBLK_SPACE) && access == 0) {
		g.mDeck->drawCard(g.mEcs, 1);
		access = 30;
	}
	if (fbl_get_key_down(FBLK_H) && access == 0) {
		g.mDeck->hideCards(g.mEcs);
		access = 30;
	}

	access--;
	if (access < 0) access = 0;

}

void Dungeon::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system
	g.mSysManager->mLightSystem->Update(g);					// update the light system

	//g.mWeather->tick();

	Efx::getInstance().tickTweens();

	processInput(g);

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick dungeon!" << std::endl;

}
