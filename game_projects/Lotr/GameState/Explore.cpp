/*
*
*	Lotr
*
*	Explore.cpp
*
*   Explore class implementation, takes care of the Explore game-state.
*
*	Hans Strömquist 2022
*
*/


#include "../../../src/fbl.hpp"
#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"
#include "../Game.hpp"
#include "../SysManager.hpp"

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"
#include "../Ecs/Systems/MouseCtrlSystem.hpp"
#include "../Ecs/Systems/CameraSystem.hpp"
#include "../Ecs/Systems/DialogueTrigSystem.hpp"
#include "../Ecs/Systems/LightSystem.hpp"

#include "../Weather.hpp"
#include "../Chars.hpp"
#include "Explore.hpp"

// Explore-class implementation

Explore::Explore() {

	std::cout << "Started Explore state." << std::endl;

}

Explore::~Explore() {

	std::cout << "Destroyed Explore state." << std::endl;

}

void Explore::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(*g.mEcs);			// update the path system
	g.mSysManager->mMouseCtrlSystem->Update(*g.mEcs);		// update the mouse control system
	g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system
	g.mSysManager->mDialogueTrigSystem->Update(g);			// update the dialogue trigger system
	g.mSysManager->mLightSystem->Update(*g.mEcs);			// update the light system

	g.mWeather->tick();

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick explore!" << std::endl;

}
