/*
*
*	Lotr
*
*	Chars.cpp
*
*	Chars class implementation, handles the characters in the game, storing entity id's etc.
*
*	Hans Strömquist 2022
*
*/

#include "Chars.hpp"

#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"

#include "Game.hpp"

#include "SysManager.hpp"

#include "Ecs/Systems/SpriteSystem.hpp"
#include "Ecs/Systems/PathSystem.hpp"
#include "Ecs/Systems/MouseCtrlSystem.hpp"
#include "Ecs/Systems/CameraSystem.hpp"

// Chars-class implementation

Chars::Chars() {

	std::cout << "Initialized Chars subsystem." << std::endl;

}

Chars::~Chars() {

	std::cout << "Destroyed Chars subsystem." << std::endl;

}

void Chars::setupPlayer(Coordinator* mEcs, SysManager* mSysManager) {

	// create the player entity
	mFrodo = mEcs->CreateEntity();

	// add components to the entity
										// x  y
	mEcs->AddComponent(mFrodo, Position{ 64, 64 });
								   // id id id id num tx ty   w   h   anim fr spd dir dirLast
	mEcs->AddComponent(mFrodo, Sprite{ 0, 0, 0, 0, 4, 0, 224, 32, 32, true, 2, 12, 1, 1 });
								 // id  gX gY newPath
	mEcs->AddComponent(mFrodo, Path{ 0, 0, 0, false });
									  // clicked
	mEcs->AddComponent(mFrodo, MouseCtrl{ false });
									// x  y	 damp
	mEcs->AddComponent(mFrodo, Camera{ 0, 0, 0.1, (Game::LogicalResW / 2) - Game::TileSize*3, (Game::LogicalResH / 2) - Game::TileSize*3, 7 * Game::TileSize, 7 * Game::TileSize });

	//mSysManager->mSpriteSystem->Init(*this->mEcs);
	mSysManager->mPathSystem->Init(*mEcs);
	mSysManager->mCameraSystem->Init(*mEcs);

	/*
	auto& pos = mEcs->GetComponent<Position>(0);
	std::cout << pos.x << std::endl;
	*/

}

void Chars::setupPlayerGfx(Coordinator* mEcs, SysManager* mSysManager) {


	// set up graphics for the player
	mSysManager->mSpriteSystem->Init(*mEcs);

}
