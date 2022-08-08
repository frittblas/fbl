/*
*
*	Lotr
*
*	SysManager.hpp
*
*	SysManager class header, handles the systems for the Ecs, keeping 
*
*	Hans Strömquist 2022
*
*/

#pragma once


class SpriteSystem;

class SysManager {

public:
    std::shared_ptr<SpriteSystem> mSpriteSystem;

    SysManager();
    ~SysManager();

};
