/*
*
*	Ecs
*
*	RobotCtrlSystem.hpp
*
*	The robot controller system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class RobotCtrlSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
