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
        Acc,
        Torch,
        Flame,
        Laser,
        Block,
        NumCards
    };

	const uint32_t Unassigned = 9999;   // used for empty slots in the collected card-arrays.

	Entity mAllCards[NumCards];         // all the cards you can use in the game, as an array
    Entity mCollectedCards[NumCards];   // all the cards you have collected, as an array
	std::vector<Entity> mBuildDeck;     // the deck of cards you are building, as vector
    std::queue<Entity> mDrawPile;       // the draw pile, as queue (gets copied from mBuildDeck at start of combat)
	std::stack<Entity> mDiscardPile;    // the discard pile, as stack
    std::stack<Entity> mBurnPile;       // the burn pile, as stack
	std::vector<Entity> mCurrentHand;   // the cards you're currently holding

    Deck();
    ~Deck();

    void setupCards(Coordinator* mEcs);
    void createStartingDeck(Coordinator* mEcs);
	void claimCard(Coordinator* mEcs, int nameIndex);      // call this to "own" a copy of a card
    Entity createCard(Coordinator* mEcs, int nameIndex);
    void removeCards(Coordinator* mEcs);

};
