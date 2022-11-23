/*
*
*	Ecs
*
*	DialogueTrigSystem.hpp
*
*	The dialogue trigger system header.
*
*	Hans Strömquist 2022
*
*/
#pragma once

#include "../System.hpp"

class Coordinator;
class Game;

class DialogueTrigSystem : public System
{
public:
	void Init(Coordinator& ecs);
	void Update(Game& g);
};
