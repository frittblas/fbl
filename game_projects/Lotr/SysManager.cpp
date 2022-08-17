/*
*
*	Lotr
*
*	SysManager.cpp
*
*	SysManager class implementation, keeps pointers to all systems in the Ecs.
*
*	Hans Strömquist 2022
*
*/

#include "SysManager.hpp"

#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"

#include "Ecs/Systems/SpriteSystem.hpp"
#include "Ecs/Systems/PathSystem.hpp"
#include "Ecs/Systems/MouseCtrlSystem.hpp"
#include "Ecs/Systems/CameraSystem.hpp"

// SysManager-class implementation

SysManager::SysManager() {

	std::cout << "Initialized SysManager subsystem." << std::endl;
}

SysManager::~SysManager() {

	std::cout << "Destroyed SysManager subsystem." << std::endl;

}

void SysManager::setupEcs(Coordinator *mEcs) {

	// init the Ecs
	mEcs->Init();

	// register components
	mEcs->RegisterComponent<Position>();
	mEcs->RegisterComponent<Sprite>();
	mEcs->RegisterComponent<Path>();
	mEcs->RegisterComponent<MouseCtrl>();
	mEcs->RegisterComponent<Camera>();

	// register systems
	//auto spriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	mSpriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	mPathSystem = mEcs->RegisterSystem<PathSystem>();
	mMouseCtrlSystem = mEcs->RegisterSystem<MouseCtrlSystem>();
	mCameraSystem = mEcs->RegisterSystem<CameraSystem>();

	// set up what components the systems require
	Signature sig;
	sig.set(mEcs->GetComponentType<Position>());
	sig.set(mEcs->GetComponentType<Sprite>());
	mEcs->SetSystemSignature<SpriteSystem>(sig);

	sig.reset();
	sig.set(mEcs->GetComponentType<Position>());
	sig.set(mEcs->GetComponentType<Path>());
	mEcs->SetSystemSignature<PathSystem>(sig);

	sig.reset();
	sig.set(mEcs->GetComponentType<Position>());
	sig.set(mEcs->GetComponentType<Path>());
	sig.set(mEcs->GetComponentType<MouseCtrl>());
	mEcs->SetSystemSignature<MouseCtrlSystem>(sig);

	sig.reset();
	sig.set(mEcs->GetComponentType<Position>());
	sig.set(mEcs->GetComponentType<Camera>());
	mEcs->SetSystemSignature<CameraSystem>(sig);

}
