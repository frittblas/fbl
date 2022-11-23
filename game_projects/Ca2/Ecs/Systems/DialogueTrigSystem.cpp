/*
*
*	Ecs
*
*	DialogueTrigSystem.cpp
*
*	The dialogue trigger system.
*
*
*	Hans Strömquist 2022
*
*/

#include "../../../../src/fbl.hpp"

#include "../Ecs.hpp"
#include "../Components.hpp"

#include "../../Game.hpp"
#include "../../GameState/GameState.hpp"
#include "../../Chars.hpp"
#include "../../LuaDialogue.hpp"

#include "DialogueTrigSystem.hpp"

// these are from LuaDialogue.cpp
extern int gButtonTalk;
extern int gCurrentDialogueId;

void DialogueTrigSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& trig = ecs.GetComponent<DialogueTrigger>(entity);
	}

	std::cout << "Dialogue trigger component system initialized!" << std::endl;

}

void DialogueTrigSystem::Update(Game& g) {
	
	bool triggered = false;

	auto& player = g.mEcs->GetComponent<Position>(g.mChars->mFrodo);

	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& trig = g.mEcs->GetComponent<DialogueTrigger>(entity);

		// if the player is adjacent to an Npc with a dialogue-trigger, show the talk button
		if ((pos.x == (player.x + Game::TileSize) && pos.y == player.y) ||
			(pos.x == (player.x - Game::TileSize) && pos.y == player.y) ||
			(pos.y == (player.y + Game::TileSize) && pos.x == player.x) ||
			(pos.y == (player.y - Game::TileSize) && pos.x == player.x)) {

			triggered = true;
			showTalkButton(true);

			// check response from talk button
			if (fbl_get_ui_elem_val(gButtonTalk)) {
				gCurrentDialogueId = trig.dialogueId;	// set the correct dialogue id
				g.mState->change(g, GameState::StateType::Dialogue);
			}

		}
		else if (!triggered)
			showTalkButton(false);

	}

}
