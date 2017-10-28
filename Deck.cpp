#include "Deck.h"
#include <stdlib.h>
#include <stdio.h>
#include "Cards.h"

Deck::Deck(int numOfDecks, int rand = 0)
{
	this->numOfDecks = numOfDecks;
    this->numOfCards = 0;
    this->topCard = NULL;
	shuffle(rand);
}

Deck::~Deck()
{
	CardNode *ptr;
	CardNode *delPtr;
	for ( delPtr = this->topCard; delPtr; delPtr = ptr )
	{
		ptr = delPtr->next;
		delete delPtr;
	}
}

int Deck::shuffle(int randNum = 0)
{
    CardNode *ptr;
    int pos;
    srand(randNum);
	this->topCard = new CardNode(0, this->topCard); // Placeholding
    for ( int j = 0; j < this->numOfDecks; ++j) {
        for ( int i = 1; i < 52; ++i, ++this->numOfCards ) {
            pos = ( rand() % ( this->numOfCards + 1 ) ) + 1; // Random % ( number of cards ) + 1 offset
            int j = 1;
            for( ptr = this->topCard; j < pos; ptr = ptr->next ) {++j;} // Find index of new card
            ptr->next = new CardNode( cardList[i], ptr->next ); // Insert card
        }
    }
	ptr = this->topCard; // Temporary pointer
	this->topCard = ptr->next; // Move topCard ptr to real start
	delete ptr;
	return randNum;
}

int Deck::pop()
{
    int ret = this->topCard->card;
    CardNode *del = this->topCard;
    this->topCard = this->topCard->next;
    delete del;
    --this->numOfCards;
    return ret;
}

bool Deck::lessThan(int cardNum)
{
    return this->numOfCards < cardNum;
}

void Deck::reveal()
{
    for ( CardNode *ptr = this->topCard; ptr; ptr = ptr->next ) {
        printf("%s ", cardTag(ptr->card));
    }
    printf("\n");
}

