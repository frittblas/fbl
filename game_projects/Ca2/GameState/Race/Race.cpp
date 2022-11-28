/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Race.cpp
*
*   Race class implementation, takes care of the Racing game-state.
*
*	Hans Strömquist 2022
*
*/


#include "../../../../src/fbl.hpp"
#include "../../Ecs/Ecs.hpp"
#include "../../Ecs/Components.hpp"
#include "../../Game.hpp"
#include "../../SysManager.hpp"

#include "../../Ecs/Systems/SpriteSystem.hpp"
#include "../../Ecs/Systems/PathSystem.hpp"
#include "../../Ecs/Systems/MouseCtrlSystem.hpp"
#include "../../Ecs/Systems/CameraSystem.hpp"
#include "../../Ecs/Systems/DialogueTrigSystem.hpp"
#include "../../Ecs/Systems/LightSystem.hpp"

#include "../../Weather.hpp"
#include "../../Chars.hpp"
#include "Race.hpp"

// Race-class implementation

Race::Race() {

	std::cout << "Started Race state." << std::endl;

}

Race::~Race() {

	std::cout << "Destroyed Race state." << std::endl;

}

void Race::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(g);					// update the path system, note the g as argument
	g.mSysManager->mMouseCtrlSystem->Update(*g.mEcs);		// update the mouse control system
	g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system
	g.mSysManager->mDialogueTrigSystem->Update(g);			// update the dialogue trigger system, note the g as argument
	g.mSysManager->mLightSystem->Update(*g.mEcs);			// update the light system

	g.mWeather->tick();

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick race!" << std::endl;

}
