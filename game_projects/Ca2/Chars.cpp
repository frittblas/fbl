/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Chars.cpp
*
*	Chars class implementation, handles the characters in the game, storing entity id's etc.
*
*	Hans Strömquist 2022
*
*/

#include "../../tools/ScenEdit/ScenEdit.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Chars.hpp"


// Chars-class implementation

Chars::Chars() {

	std::cout << "Initialized Chars subsystem." << std::endl;

}

Chars::~Chars() {

	std::cout << "Destroyed Chars subsystem." << std::endl;

}

void Chars::setupPlayer(Coordinator* mEcs) {

	// create the player entity
	mBrodo = mEcs->CreateEntity();

	// add components to the entity
										// x  y
	mEcs->AddComponent(mBrodo, Position{ 128, 160 });
								   // id id id id num tx ty   w   h   anim fr spd dir dirLast layer
	mEcs->AddComponent(mBrodo, Sprite{ 0, 0, 0, 0, 4, 0, 224, 32, 32, true, 2, 12, 1, 1, 1 });
								 // id  gX gY newPath speed diag pixelsPerFrame
	mEcs->AddComponent(mBrodo, Path{ 0, 0, 0, false, 2.0, FBL_PATHF_USE_DIAG, 1 });
									  // access
	mEcs->AddComponent(mBrodo, MouseCtrl{ 60 });
									// x  y	 damp  w  h
	mEcs->AddComponent(mBrodo, Camera{ 0, 0, 0.05, 2, 2 });
								  // id  tx  ty   w    h  scale
	mEcs->AddComponent(mBrodo, Light{ 0, 384, 0, 128, 128, 2.0 });

}

void Chars::removePlayer(Coordinator* mEcs) {

	mEcs->DestroyEntity(mBrodo);
	
}

void Chars::hidePlayer(Coordinator* mEcs) {

	auto& spr = mEcs->GetComponent<Sprite>(mBrodo);

	for(int i = 0; i < 4; i++)
		fbl_set_sprite_active(spr.id[i], false);

}

void Chars::stopPlayerPathing(Game& g) {

	// make sure that you don't path to the menu button for example, annoying
	auto& path = g.mEcs->GetComponent<Path>(g.mChars->mBrodo);
	fbl_pathf_set_path_status(path.id, FBL_PATHF_NOT_STARTED);
	path.newPath = false;
	auto& ctrl = g.mEcs->GetComponent<MouseCtrl>(g.mChars->mBrodo);
	ctrl.access = 30;

}


void Chars::setupNpc(Game& g) {

	// Npc characters start at tile->type == 10
	// type 0 - 9 are reserved for different terrain

	const int NpcOffset = 10;

	for (int i = 0; i < Game::MapW; i++) {
		for (int j = 0; j < Game::MapH; j++) {
			int index = i + g.mMap->mapWidth * j;
			if (g.mMap->tile[index] != nullptr)
				if (g.mMap->tile[index]->type > 9)	// if the type is 10 or more = not terrain anymore but Npc!

					switch (g.mMap->tile[index]->type) {

						case 10 :	// Npc with type 10 (-10) = 0 = slime
						case 11:	// type 11 (-10) = 1 = also slime :)

							int id = g.mEcs->CreateEntity();
							g.mChars->mNpc.push_back(id);
							float x = g.mMap->tile[index]->x;
							float y = g.mMap->tile[index]->y;
							uint8_t dialogueId = g.mMap->tile[index]->type - NpcOffset;
							g.mEcs->AddComponent(id, Position{ x, y });
							g.mEcs->AddComponent(id, DialogueTrigger{ dialogueId });
														// id  tx  ty   w    h  scale
							//g.mEcs->AddComponent(id, Light{ 0, 384, 0, 128, 128, 2.0 });
							std::cout << "Npc added." << std::endl;
							break;

					}
					
		}
	}

}

void Chars::removeNpc(Coordinator* mEcs) {

	for (Entity e : mNpc) {
		mEcs->DestroyEntity(e);
		std::cout << "Npc removed." << std::endl;
	}

	mNpc.clear();

}
