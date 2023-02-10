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
		auto& path = g.mEcs->GetComponent<Path>(g.mChars->mBrodo);
		fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);	// this seems to work was 0 before
		path.newPath = false;
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

	if(fbl_get_raw_frames_count() % 60 == 0)
		std::cout << "Tick explore!" << std::endl;

}
