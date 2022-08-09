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

class SysManager {

public:
    std::shared_ptr<SpriteSystem> mSpriteSystem; // the sprite system
    std::shared_ptr<PathSystem> mPathSystem;     // the path system

    SysManager();
    ~SysManager();

};
