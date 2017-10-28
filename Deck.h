#ifndef DECK_H
#define DECK_H
#include <stdio.h>

struct CardNode {
    int card;
    CardNode *next;
    CardNode( int card, CardNode *next=NULL) : card(card), next(next) {}
};

class Deck
{
	public:
		Deck(int, int);
		~Deck();
		int shuffle(int);
        int pop();
        bool lessThan(int);
        void reveal();
		
	private:
		CardNode *topCard;
		int numOfCards;
		int numOfDecks;
};



#endif