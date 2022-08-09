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


#include "../../../src/fbl.hpp"

#include "../Game.hpp"
#include "Explore.hpp"

#include "../Ecs/Ecs.hpp"
#include "../Ecs/Components.hpp"

#include "../SysManager.hpp"

#include "../Ecs/Systems/SpriteSystem.hpp"
#include "../Ecs/Systems/PathSystem.hpp"

#include "../Chars.hpp"


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


	// test path, remove this
	if (fbl_get_mouse_release(FBLMB_LEFT)) {

		auto& p = g.mEcs->GetComponent<Path>(g.mChars->mFrodo);

		p.goalX = fbl_get_mouse_x();
		p.goalY = fbl_get_mouse_y();

		while (p.goalX % 32 != 0) p.goalX--;
		while (p.goalY % 32 != 0) p.goalY--;

		p.newPath = true;

	}

	if (num == 0)
		std::cout << "Tick explore!" << std::endl;

}
