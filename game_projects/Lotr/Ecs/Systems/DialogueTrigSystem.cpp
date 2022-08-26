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

#include "DialogueTrigSystem.hpp"

void DialogueTrigSystem::Init(Coordinator& ecs) {

	for (auto const& entity : mEntities)
	{
		auto& pos = ecs.GetComponent<Position>(entity);
		auto& trig = ecs.GetComponent<DialogueTrigger>(entity);


	}

	std::cout << "Dialogue trigger component system initialized!" << std::endl;

}

void DialogueTrigSystem::Update(Game& g) {
	
	for (auto const& entity : mEntities)
	{
		auto& pos = g.mEcs->GetComponent<Position>(entity);
		auto& trig = g.mEcs->GetComponent<DialogueTrigger>(entity);

		auto& player = g.mEcs->GetComponent<Position>(g.mChars->mFrodo);

		if (pos.x == player.x + Game::TileSize)
			g.mState->change(g, GameState::StateType::Dialogue);


	}

}
