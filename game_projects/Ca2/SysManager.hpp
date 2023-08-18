/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	SysManager.hpp
*
*	SysManager class header, keeps pointers to all systems in the Ecs.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <iostream>

// general
class SpriteSystem;
class PathSystem;
class MouseCtrlSystem;
class CameraSystem;
class DialogueTrigSystem;
class LightSystem;

// robot specific
// Stats does not have it's own system!
class PathLogicSystem;
class DeathMatchSystem;
class BasicAISystem;
class AutoAimSystem;
class LaserSystem;
class MagnetSystem;
class TurboSystem;
class ShieldSystem;
class HealSystem;
class DiagSystem;
class RobotCtrlSystem;

class Coordinator;

class SysManager {

public:
    std::shared_ptr<SpriteSystem> mSpriteSystem;             // the sprite system
    std::shared_ptr<PathSystem> mPathSystem;                 // the path system
    std::shared_ptr<MouseCtrlSystem> mMouseCtrlSystem;       // the mouse controller system
    std::shared_ptr<CameraSystem> mCameraSystem;             // the camera system
    std::shared_ptr<DialogueTrigSystem> mDialogueTrigSystem; // the dialogue trigger system
    std::shared_ptr<LightSystem> mLightSystem;               // the light system

    std::shared_ptr<PathLogicSystem> mPathLogicSystem;       // flag capturing!
    std::shared_ptr<DeathMatchSystem> mDeathMatchSystem;     // death match
    std::shared_ptr<BasicAISystem> mBasicAISystem;           // basic non player AI
    std::shared_ptr<AutoAimSystem> mAutoAimSystem;           // the auto aim system
    std::shared_ptr<LaserSystem> mLaserSystem;               // the laser system
    std::shared_ptr<MagnetSystem> mMagnetSystem;             // the magnet system
    std::shared_ptr<TurboSystem> mTurboSystem;               // the turbo system
    std::shared_ptr<ShieldSystem> mShieldSystem;             // the shield system
    std::shared_ptr<HealSystem> mHealSystem;                 // the heal system
    std::shared_ptr<DiagSystem> mDiagSystem;                 // the diagonal system
    std::shared_ptr<RobotCtrlSystem> mRobotCtrlSystem;       // the robot control system

    SysManager();
    ~SysManager();

    void setupEcs(Coordinator* mEcs);                        // register componets and systems

};
