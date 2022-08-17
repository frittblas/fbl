/*
*
*	Ecs
*
*	PathSystem.hpp
*
*	The path system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class PathSystem : public System
{
public:

    enum Dir {
        Up,
        Right,
        Left,
        Down
    };

	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
