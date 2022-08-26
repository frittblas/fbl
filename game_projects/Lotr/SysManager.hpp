/*
*
*	Lotr
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

class SpriteSystem;
class PathSystem;
class MouseCtrlSystem;
class CameraSystem;
class DialogueTrigSystem;

class Coordinator;

class SysManager {

public:
    std::shared_ptr<SpriteSystem> mSpriteSystem;             // the sprite system
    std::shared_ptr<PathSystem> mPathSystem;                 // the path system
    std::shared_ptr<MouseCtrlSystem> mMouseCtrlSystem;       // the mouse controller system
    std::shared_ptr<CameraSystem> mCameraSystem;             // the camera system
    std::shared_ptr<DialogueTrigSystem> mDialogueTrigSystem; // the dialogue trigger system

    SysManager();
    ~SysManager();

    void setupEcs(Coordinator* mEcs);    // register componets and systems

};
