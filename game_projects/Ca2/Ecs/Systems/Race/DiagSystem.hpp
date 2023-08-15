/*
*
*	Ecs
*
*	DiagSystem.hpp
*
*	The robot controller for diagonals system header.
*
*	Hans Strömquist 2023
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class DiagSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
