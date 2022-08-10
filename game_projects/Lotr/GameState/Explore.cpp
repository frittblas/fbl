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

	std::cout << "Started Explore state." << std::endl;

}

Explore::~Explore() {

	std::cout << "Destroyed Explore state." << std::endl;

}

void Explore::tick(Game& g) {

	static bool pressed = false;

	g.mSysManager->mSpriteSystem->Update(*g.mEcs);	// update the sprite system
	g.mSysManager->mPathSystem->Update(*g.mEcs);	// update the path system

	if (fbl_get_mouse_click(FBLMB_LEFT)) {
	
		pressed = true;
	
	}

	// test path, remove this
	if (fbl_get_mouse_release(FBLMB_LEFT) && pressed) {

		auto& p = g.mEcs->GetComponent<Path>(g.mChars->mFrodo);

		p.goalX = fbl_get_mouse_logical_x();
		p.goalY = fbl_get_mouse_logical_y();

		while (p.goalX % 32 != 0) p.goalX--;
		while (p.goalY % 32 != 0) p.goalY--;

		p.newPath = true;

		pressed = false;

	}


	int num = std::rand() / ((RAND_MAX + 1u) / 50); // random numbers from 0-49
	if (num == 0)
		std::cout << "Tick explore!" << std::endl;

}
