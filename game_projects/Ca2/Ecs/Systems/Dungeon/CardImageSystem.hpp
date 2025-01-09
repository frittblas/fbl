/*
*
*	Ecs
*
*	CardImageSystem.hpp
*
*	The card image system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../../System.hpp"

class Coordinator;

class CardImageSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
};
