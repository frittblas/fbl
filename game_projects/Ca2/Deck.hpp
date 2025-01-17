/*
*
*	Charming Alarming 2: Reasonable Robots
*
*	Deck.hpp
*
*	Deck class header, handles cards, piles and decks.
*
*	Hans Strömquist 2022
*
*/

#pragma once

#include <vector>
#include <queue>
#include <stack>
#include "Ecs/Types.hpp"

class Coordinator;
class Game;

class Deck {

public:

    enum CardTypes{
        Attack,
        Skill,
        Ongoing,
        Junk,
        NumCardTypes
    };

    enum CardRarities {
        Common,
        Rare,
        Epic
    };

    enum CardNames {
        Move,
        Laser,
        Block,
        NumCards
    };

    // position of the piles
    const int cDrawPileX = 32;
    const int cDrawPileY = 418;
    const int cDiscardPileX = 864;
	const int cDiscardPileY = 418;
	const int cBurnPileX = 864;
	const int cBurnPileY = 296;

	const uint32_t Unassigned = 9999;   // used for empty slots in the collected card-arrays.

	Entity mAllCards[NumCards];         // all the cards you can use in the game, as an array
    Entity mCollectedCards[NumCards];   // all the cards you have collected, as an array
	std::vector<Entity> mBuildDeck;     // the deck of cards you are building, as vector
    std::queue<Entity> mDrawPile;       // the draw pile, as queue (gets copied from mBuildDeck at start of combat)
	std::stack<Entity> mDiscardPile;    // the discard pile, as stack
    std::stack<Entity> mBurnPile;       // the burn pile, as stack
	std::vector<Entity> mCurrentHand;   // the cards you're currently holding

	float mCardTweenXId, mCardTweenYId; // used for tweening the cards

    Deck();
    ~Deck();

    void setupCards(Coordinator* mEcs);
    void createStartingDeck(Coordinator* mEcs);
	void claimCard(Coordinator* mEcs, int nameIndex);      // call this to "own" a copy of a card
    Entity createCard(Coordinator* mEcs, int nameIndex);
    void removeCards(Coordinator* mEcs);
	void hideCards(Coordinator* mEcs);
    void copyDeckToDrawpile(Coordinator* mEcs);
    void drawCard(Coordinator* mEcs, int amount);
    void prepPiles(Coordinator* mEcs);
    void clearPiles(Coordinator* mEcs);

private:
	uint16_t mDrawPileCardBackId;
	uint16_t mDiscardPileCardBackId;

};
