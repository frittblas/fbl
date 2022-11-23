/*
*
*	Ecs
*
*	MouseCtrlSystem.hpp
*
*	The mouse controller system header.
*
*	Hans Str�mquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class MouseCtrlSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
