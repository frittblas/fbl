/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Objects.cpp
*
*	Objects class implementation, handles cards, piles and decks.
*
*	Hans Strömquist 2022
*
*/

#include "../../src/fbl.hpp"
#include "Ecs/Ecs.hpp"
#include "Ecs/Components.hpp"
#include "Game.hpp"
#include "Deck.hpp"

// Objects-class implementation

Deck::Deck() {

	std::cout << "Initialized Deck subsystem." << std::endl;
}

Deck::~Deck() {

	std::cout << "Destroyed Deck subsystem." << std::endl;

}

void Deck::setupCards(Coordinator* mEcs) {

	// init to unassigned
	for (int i = 0; i < NumCards; i++) {
		mAllCards[i] = Unassigned;
		mCollectedCards[i] = Unassigned;
	}

	// create all the card entities
	for (int i = 0; i < NumCards; i++) {

		// add the entity to the array containing all cards
		mAllCards[i] = createCard(mEcs, i);

	}

	createStartingDeck(mEcs);

}

void Deck::createStartingDeck(Coordinator* mEcs) {

	// create a starting deck of 9 cards

	claimCard(mEcs, Move);
	claimCard(mEcs, Move);
	claimCard(mEcs, Move);
	claimCard(mEcs, Laser);
	claimCard(mEcs, Laser);
	claimCard(mEcs, Laser);
	claimCard(mEcs, Block);
	claimCard(mEcs, Block);
	claimCard(mEcs, Block);

}

void Deck::claimCard(Coordinator* mEcs, int nameIndex) {

	// Create a new card and add it to mBuildDeck vector

	mBuildDeck.push_back(createCard(mEcs, nameIndex));
	

}

Entity Deck::createCard(Coordinator* mEcs, int nameIndex) {

	Entity tmpCard;

	tmpCard = mEcs->CreateEntity();

	// add components to the entity
										  // x  y
	mEcs->AddComponent(tmpCard, Position{ 400, 200 });


	switch (nameIndex) {

	case Move:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 0, 512, 64, 90, false, 0, 0, 0, 0, 10 }); // cards are on layer 10-20
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Move", Move, Skill, Common, 1, false, false, 3, 0, 0 });

		break;

	case Laser:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 256, 512, 64, 90, false, 0, 0, 0, 0, 10 });
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Laser", Laser, Skill, Common, 1, false, false, 3, 5, 0 });

		break;
	case Block:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 320, 512, 64, 90, false, 0, 0, 0, 0, 10 });
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Block", Block, Skill, Common, 1, false, false, 3, 0, 5 });

		break;

	}

	std::cout << "Card: " << tmpCard  << " created!" << std::endl;

	return tmpCard;

}

void Deck::removeCards(Coordinator* mEcs) {

	for (Entity e : mAllCards) {
		if (e != Unassigned) {
			mEcs->DestroyEntity(e);
			std::cout << "Card removed from All-list." << std::endl;
		}
	}

	for (Entity e : mBuildDeck) {
		mEcs->DestroyEntity(e);
		std::cout << "Card removed from BuildDeck-list." << std::endl;
	}

}
