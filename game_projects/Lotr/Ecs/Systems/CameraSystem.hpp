/*
*
*	Ecs
*
*	CameraSystem.hpp
*
*	The camera system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;

class CameraSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
