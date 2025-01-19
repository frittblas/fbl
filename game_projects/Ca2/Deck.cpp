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

	Entity tmpCard = mEcs->CreateEntity();

	// add components to the entity
										  // x  y
	mEcs->AddComponent(tmpCard, Position{ 400, 200 });


	switch (nameIndex) {

	case Move:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 0, 512, 64, 90, false, 1, 0, 0, 0, 10 }); // cards are on layer 10-20
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Move", Move, Skill, Common, 1, false, false, 3, 0, 0 });

		break;

	case Laser:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 256, 512, 64, 90, false, 1, 0, 0, 0, 10 });
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Laser", Laser, Skill, Common, 1, false, false, 3, 5, 0 });

		break;
	case Block:
										// id id id id num tx ty   w   h   anim fr spd dir dirLast layer
		mEcs->AddComponent(tmpCard, Sprite{ 0, 0, 0, 0, 1, 320, 512, 64, 90, false, 1, 0, 0, 0, 10 });
										// name  nameid type  rarity mana burn  upgrd prc atk blk
		mEcs->AddComponent(tmpCard, Card{ "Block", Block, Skill, Common, 1, false, false, 3, 0, 5 });

		break;

	}

	std::cout << "Card: " << tmpCard << " created!" << std::endl;

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

	mBuildDeck.clear();

}

void Deck::hideCards(Coordinator* mEcs) {

	// Note: think about maybe hide different piles individually?

	for (Entity e : mAllCards) {
		if (e != Unassigned) {
			auto& spr = mEcs->GetComponent<Sprite>(e);
			fbl_set_sprite_active(spr.id[0], false);
		}
	}

	for (Entity e : mBuildDeck) {
		auto& spr = mEcs->GetComponent<Sprite>(e);
		fbl_set_sprite_active(spr.id[0], false);
	}

}

void Deck::copyDeckToDrawpile(Coordinator* mEcs) {

	// loop through mBuildDeck and create a copy of each card in mDrawPile
	for (Entity card : mBuildDeck) {
		auto& crd = mEcs->GetComponent<Card>(card);
		Entity newCard = createCard(mEcs, crd.nameIndex);
		mDrawPile.push(newCard);
		// create a light component for the cards in the draw pile? Or for every card..
		mEcs->AddComponent(newCard, Light{ 0, 384, 512, 64, 90, 1.0 });
	}

	std::cout << "Copied " << mBuildDeck.size() << " cards to draw pile." << std::endl;
}

void Deck::drawCard(Coordinator* mEcs, int amount) {

	// draw a card from the draw pile to the hand

	for (int i = 0; i < amount; i++) {

		if (mDrawPile.size() > 0) {

			Entity e = mDrawPile.front();
			mDrawPile.pop();
			mCurrentHand.push_back(e);

			// set the card to visible by changing pos
			auto& pos = mEcs->GetComponent<Position>(e);
			pos.x = cDrawPileX + 128 + (mCurrentHand.size() * 32);
			pos.y = cDrawPileY;

			auto& card = mEcs->GetComponent<Card>(e);
			auto& spr = mEcs->GetComponent<Sprite>(e);

			fbl_set_sprite_layer(spr.id[0], mCurrentHand.size() + 10); // 10-20

			std::cout << "Card: " << card.name << " drawn. Sprite layer: " << fbl_get_sprite_layer(spr.id[0]) << std::endl;

		}
		else {
			std::cout << "No more cards in draw pile." << std::endl;
		}

	}

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

void Deck::prepPiles(Coordinator* mEcs) {

	// create the sprites for cardbacks (layer 21) then place cards from the draw pile under them (still visible)

	mDrawPileCardBackId = fbl_create_sprite(448, 512, 64, 90, 0);
	fbl_set_sprite_xy(mDrawPileCardBackId, cDrawPileX, cDrawPileY);
	fbl_set_sprite_layer(mDrawPileCardBackId, 21);

	mDiscardPileCardBackId = fbl_create_sprite(448, 512, 64, 90, 0);
	fbl_set_sprite_xy(mDiscardPileCardBackId, cDiscardPileX, cDiscardPileY);
	fbl_set_sprite_layer(mDiscardPileCardBackId, 21);

	// put all cards face up under the cardback draw pile sprite
	std::queue<Entity> tempQueue;
	while (!mDrawPile.empty()) {
		Entity e = mDrawPile.front();
		mDrawPile.pop();

		auto& pos = mEcs->GetComponent<Position>(e);
		pos.x = cDrawPileX;
		pos.y = cDrawPileY;

		tempQueue.push(e);
	}
	mDrawPile = tempQueue;

	fbl_sort_sprites(FBL_SORT_BY_LAYER);

}

void Deck::clearPiles(Coordinator* mEcs) {

	// Clear the draw pile (queue)
	while (!mDrawPile.empty()) {
		Entity tmpCard = mDrawPile.front();
		mEcs->DestroyEntity(tmpCard);
		mDrawPile.pop();
		std::cout << "Card removed from draw pile." << std::endl;
	}

	// Clear the discard pile (stack)
	while (!mDiscardPile.empty()) {
		Entity tmpCard = mDiscardPile.top();
		mEcs->DestroyEntity(tmpCard);
		mDiscardPile.pop();
		std::cout << "Card removed from discard pile." << std::endl;
	}

	// Clear the burn pile (stack)
	while (!mBurnPile.empty()) {
		Entity tmpCard = mBurnPile.top();
		mEcs->DestroyEntity(tmpCard);
		mBurnPile.pop();
		std::cout << "Card removed from burn pile." << std::endl;
	}

	// Clear the hand
	for (Entity e : mCurrentHand) {
		mEcs->DestroyEntity(e);
		std::cout << "Card removed from hand." << std::endl;
	}

	mCurrentHand.clear();

}
