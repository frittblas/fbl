/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	SysManager.cpp
*
*	SysManager class implementation, keeps pointers to all systems in the Ecs.
*
*	Hans Strömquist 2022
*
*/

#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "SysManager.hpp"

// general
#include "Ecs/Systems/SpriteSystem.hpp"
#include "Ecs/Systems/PathSystem.hpp"
#include "Ecs/Systems/MouseCtrlSystem.hpp"
#include "Ecs/Systems/CameraSystem.hpp"
#include "Ecs/Systems/DialogueTrigSystem.hpp"
#include "Ecs/Systems/LightSystem.hpp"

// robot specific
#include "Ecs/Systems/Race/AutoAimSystem.hpp"
#include "Ecs/Systems/Race/LaserSystem.hpp"

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
	mEcs->RegisterComponent<DialogueTrigger>();
	mEcs->RegisterComponent<Light>();

	// robot specific
	mEcs->RegisterComponent<Addon>();
	mEcs->RegisterComponent<Stats>();
	mEcs->RegisterComponent<AutoAim>();
	mEcs->RegisterComponent<Laser>();

	// register systems
	mSpriteSystem = mEcs->RegisterSystem<SpriteSystem>();
	mPathSystem = mEcs->RegisterSystem<PathSystem>();
	mMouseCtrlSystem = mEcs->RegisterSystem<MouseCtrlSystem>();
	mCameraSystem = mEcs->RegisterSystem<CameraSystem>();
	mDialogueTrigSystem = mEcs->RegisterSystem<DialogueTrigSystem>();
	mLightSystem = mEcs->RegisterSystem<LightSystem>();

	// robot specific
	mAutoAimSystem = mEcs->RegisterSystem<AutoAimSystem>();
	mLaserSystem = mEcs->RegisterSystem<LaserSystem>();

	// set up what components the systems require
	Signature sign;
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Sprite>());
	mEcs->SetSystemSignature<SpriteSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Path>());
	mEcs->SetSystemSignature<PathSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Path>());
	sign.set(mEcs->GetComponentType<MouseCtrl>());
	mEcs->SetSystemSignature<MouseCtrlSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Camera>());
	mEcs->SetSystemSignature<CameraSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<DialogueTrigger>());
	mEcs->SetSystemSignature<DialogueTrigSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Light>());
	mEcs->SetSystemSignature<LightSystem>(sign);

	// robot specific
	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Stats>());
	sign.set(mEcs->GetComponentType<AutoAim>());
	mEcs->SetSystemSignature<AutoAimSystem>(sign);

	sign.reset();
	sign.set(mEcs->GetComponentType<Position>());
	sign.set(mEcs->GetComponentType<Stats>());
	sign.set(mEcs->GetComponentType<AutoAim>());
	sign.set(mEcs->GetComponentType<Laser>());
	mEcs->SetSystemSignature<LaserSystem>(sign);

}
