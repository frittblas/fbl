/*
*
*	Ecs
*
*	SpriteSystem.hpp
*
*	The sprite system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;
struct Sprite;

class SpriteSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Coordinator& ecs);
	bool isCard(Sprite& spr);
};
