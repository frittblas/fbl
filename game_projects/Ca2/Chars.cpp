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
#include "../../tools/ScenEdit/GuiFuncs.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Chars.hpp"


// Chars-class implementation

Chars::Chars() {

	mFadingNpcId = 0;
	mFadeCounter = 255;

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

						case StorySlime:		// Npc with type 10 (-10) = 0 = slime
						case StorySlime + 1:	// type 11 (-10) = 1 = also slime :)
						case StorySlime + 2:
						case StorySlime + 3:
						case InfoSlime:
						case InfoSlime + 1:
						case InfoSlime + 2:
						case InfoSlime + 3:
						case InfoSlime + 4:
						case InfoSlime + 5:
						case InfoSlime + 6:
						case EventSlime:
						case EventSlime + 1:
						case EventSlime + 2:
						case EventSlime + 3:
						case ChestMan:
						case ChestMan + 1:
						case ChestMan + 2:
						case ChestMan + 3:
						case ChestMan + 4:
						case Other:
						case Other + 1:

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

void Chars::removeEntityFromNpcList(Entity e) {
	mNpc.erase(std::remove(mNpc.begin(), mNpc.end(), e), mNpc.end());
	for (Entity e : mNpc) std::cout << e << " ";
}

void Chars::checkNPC(Game& g, int npc) {

	for (Entity e : mNpc) {
	
		auto& pos = g.mEcs->GetComponent<Position>(e);
		auto& player = g.mEcs->GetComponent<Position>(g.mChars->mBrodo);

		if ((pos.x == (player.x + Game::TileSize) && pos.y == player.y) ||
			(pos.x == (player.x - Game::TileSize) && pos.y == player.y) ||
			(pos.y == (player.y + Game::TileSize) && pos.x == player.x) ||
			(pos.y == (player.y - Game::TileSize) && pos.x == player.x)) {

			switch (npc) {
				case StorySlime:
				case InfoSlime:
				case EventSlime:
					removeAndStartFade(g, e, (int)pos.x, (int)pos.y);
				break;
				case ChestMan:
					openChestMan(g, (int)pos.x, (int)pos.y);
					break;
			}

			removeEntityFromNpcList(e);

			// add the npc to the restore list
			NpcRestorer tmpRest = { (int)pos.x, (int)pos.y, (NpcName)npc, e};
			g.mChars->mRestoreList.push_back(tmpRest);

		}
	
	}

}

void Chars::removeAndStartFade(Game& g, Entity e, int x, int y) {

	// remove the sprite
	int index = getIndexAtPos(x, y);
	if (g.mMap->tile[index] != nullptr) {
		mFadingNpcId = g.mMap->tile[index]->id;
	}

	// set tile to walkable
	fbl_pathf_set_walkability(x / Game::TileSize, y / Game::TileSize, FBL_PATHF_WALKABLE);

	// remove dialogue trigger
	if (g.mEcs->HasComponent<DialogueTrigger>(e))
		g.mEcs->RemoveComponent<DialogueTrigger>(e);

	// start the fade by setting counter to one less
	mFadeCounter = 254;

}

void Chars::resetFadeCounter() {

	mFadeCounter = 255;

}

void Chars::removeUsedNpcTile(Game& g, int x, int y, int npc) {

	// deactivate sprite
	int index = getIndexAtPos(x, y);
	fbl_set_sprite_active(g.mMap->tile[index]->id, false);
	// set tile to walkable
	fbl_pathf_set_walkability(x / Game::TileSize, y / Game::TileSize, FBL_PATHF_WALKABLE);

}

void Chars::openChestMan(Game& g, int x, int y) {

	// switch the sprite to open chest
	int index = getIndexAtPos(x, y);
	if (g.mMap->tile[index] != nullptr) {
		fbl_set_sprite_image(g.mMap->tile[index]->id, 128, 480, 32, 32, 0);
	}

}

void Chars::restoreNpcs(Game& g) {

	for (NpcRestorer r : mRestoreList) {

		switch (r.name) {

			case StorySlime:
			case InfoSlime:
			case EventSlime:
				removeUsedNpcTile(g, r.x, r.y, r.name);
				removeEntityFromNpcList(r.e);
				break;
			case ChestMan:
				openChestMan(g, r.x, r.y);
				removeEntityFromNpcList(r.e);
				break;

		}

		std::cout << "Npc restored." << std::endl;
	}

}

void Chars::clearRestoreList() {

	mRestoreList.clear();

}
