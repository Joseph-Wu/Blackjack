#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include "Choices.h"

struct HandNode {
	int* hand;
	int cardsInHand;
	bool pass;
	HandNode* next;
	HandNode( bool pass = false, HandNode* next = NULL) : pass(pass), next(next) {}
};

struct BetNode {
	int bet;
	BetNode* next;
	BetNode ( int bet, BetNode* next = NULL) : bet(bet), next(next) {}
};

class Player {
    public:
        Player(int, bool(*)(Player*, Deck*));
		~Player();
		virtual void placeBet(BetNode*);
        bool deal(int);
        void split(BetNode*, int);
        virtual bool hit(Deck*, BetNode*, int);
        int stay(int);
        virtual bool blackjack();
		bool canSplit(int);
        bool splitMoney(int);
        bool handExists(int);
        bool out(bool);
        struct totalSum getTotalSum(int);
        int changeDollars(int, int);
        virtual void displayHand(bool);
		virtual int getRevealedCard();
        void clear();
    protected:
        HandNode* hands;
        int dollars;
		bool end;
};

class Dealer : public Player {
    public:
        Dealer(int, bool(*)(Player*, Deck*));
        bool hit(Deck*, BetNode*, int);
        void displayHand(bool);
        bool blackjack();
		int getRevealedCard();
    private:
        bool (*algr)(Player*, Deck*);
};

class AI : public Player {
    public:
		AI(int, bool(*)(Player*, Deck*));
		void placeBet(BetNode*);
        bool hit(Deck*, BetNode*, int);
    private:
        bool (*algr)(Player*, Deck*);
		bool doSplit(int, int, BetNode*);
		char hitChoice(int, int);
};

#endif