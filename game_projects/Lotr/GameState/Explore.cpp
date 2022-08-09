/*
*
*	Lotr
*
*	Explore.cpp
*
*   Explore class implementation, takes care of the Explore game-state.
*
*	Hans Strömquist 2022
*
*/

#include <iostream>
#include "../Game.hpp"
#include "Explore.hpp"

#include "../Ecs/Ecs.hpp"
#include "../SysManager.hpp"

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"


// Explore-class implementation

Explore::Explore() {

	std::cout << "Alloc Explore!" << std::endl;

}

Explore::~Explore() {

	std::cout << "Destroy Explore!" << std::endl;

}

void Explore::tick(Game& g) {


	g.mSysManager->mSpriteSystem->Update(*g.mEcs);	// update the sprite system
	g.mSysManager->mPathSystem->Update(*g.mEcs);	// update the path system

	int num = std::rand() / ((RAND_MAX + 1u) / 20); // random numbers from 0-19

	if (num == 0)
		std::cout << "Tick explore!" << std::endl;

}
