/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Explore.cpp
*
*   Explore class implementation, takes care of the Explore game-state.
*
*	Hans Strömquist 2022
*
*/


#include "../../../src/fbl.hpp"
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
#include "GameState.hpp"
#include "Explore.hpp"

// this is from RobotCollection.cpp
extern uint16_t gRobotCollectionMenuButton;

// Explore-class implementation

Explore::Explore() {

	std::cout << "Started Explore state." << std::endl;

}

Explore::~Explore() {

	std::cout << "Destroyed Explore state." << std::endl;

}

void Explore::processInput(Game& g) {

	// the almighty menu button
	if (fbl_get_ui_elem_val(gRobotCollectionMenuButton) > 0) {
		g.mState->change(g, GameState::StateType::RobotCollection);
		g.mChars->stopPlayerPathing(g);
	}

}

void Explore::npcFade(Game& g) {

	if (g.mChars->mFadeCounter < 255) {
		fbl_set_sprite_alpha(g.mChars->mFadingNpcId, g.mChars->mFadeCounter);
		g.mChars->mFadeCounter -= 4;
		if (g.mChars->mFadeCounter < 5) {
			g.mChars->mFadeCounter = 255;
			fbl_set_sprite_active(g.mChars->mFadingNpcId, false);
		}
	}

}

void Explore::tick(Game& g) {

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);			// update the sprite system
	g.mSysManager->mPathSystem->Update(g);					// update the path system, note the g as argument
	g.mSysManager->mMouseCtrlSystem->Update(*g.mEcs);		// update the mouse control system
	g.mSysManager->mCameraSystem->Update(*g.mEcs);			// update the camera system
	g.mSysManager->mDialogueTrigSystem->Update(g);			// update the dialogue trigger system, note the g as argument
	g.mSysManager->mLightSystem->Update(g);					// update the light system

	g.mWeather->tick();

	processInput(g);

	npcFade(g);

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick explore!" << std::endl;

}
