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

	Entity tmpCard;

	// init to unassigned
	for (int i = 0; i < NumCards; i++) {
		mAllCards[i] = Unassigned;
		mCollectedCards[i] = Unassigned;
	}

	// create all the card entities
	for (int i = 0; i < NumCards; i++) {

		tmpCard = mEcs->CreateEntity();

		// add components to the entity
											  // x  y
		mEcs->AddComponent(tmpCard, Position{ 400, 200 });


		switch (i) {

		case Move:
											// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
			mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 0, 512, 64, 90, false, 0, 0, 0, 0, 10 });	// cards are on layer 10-20
											// name   type   rarity mana burn  upgrd price
			mEcs->AddComponent(tmpCard, Card{ "Move", Skill, Common, 1, false, false, 3 });

			break;

		}

		// add the entity to the array containing all cards
		mAllCards[i] = tmpCard;

	}

	claimCard(Move);


}

void Deck::claimCard(int nameIndex) {

	

}