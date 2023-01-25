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
// Stats does not have it's own system YET!
class AutoAimSystem;
class LaserSystem;

class Coordinator;

class SysManager {

public:
    std::shared_ptr<SpriteSystem> mSpriteSystem;             // the sprite system
    std::shared_ptr<PathSystem> mPathSystem;                 // the path system
    std::shared_ptr<MouseCtrlSystem> mMouseCtrlSystem;       // the mouse controller system
    std::shared_ptr<CameraSystem> mCameraSystem;             // the camera system
    std::shared_ptr<DialogueTrigSystem> mDialogueTrigSystem; // the dialogue trigger system
    std::shared_ptr<LightSystem> mLightSystem;               // the light system

    std::shared_ptr<AutoAimSystem> mAutoAimSystem;           // the auto aim system
    std::shared_ptr<LaserSystem> mLaserSystem;               // the laser system

    SysManager();
    ~SysManager();

    void setupEcs(Coordinator* mEcs);                        // register componets and systems

};
