#include <stdio.h>
#include "Player.h"
#include "Cards.h"
#define MAX_HAND_SIZE 11
#define PLUS 1
#define MINUS 2

using namespace std;

struct totalSum {
	int total;
	bool soft;
	
	totalSum(int t=0, bool b=false) : total(t), soft(b) {}
	
	totalSum& operator=(const totalSum& r) 
	{
		total = r.total;
		soft = r.soft;
		return *this;
	}
};

Player::Player(int d=100, bool(*UNUSED)(Player*, Deck*) = NULL) {
	hands = new HandNode;
	hands->hand = new int[MAX_HAND_SIZE];
    hands->cardsInHand = 0;
    hands->pass = false;
    dollars = d;
	hands->next = NULL;
	end = false;
}

Player::~Player() {
	delete hands->hand;
	delete hands->next;
	delete hands;
}

void Player::placeBet(BetNode* bets)
{
	cin >> bets->bet;
}

/* Takes a card value and stores it into the hand
 * Returns true if both cards are a pair
 */

bool Player::deal(int card)
{
    hands->hand[hands->cardsInHand++] = card;
    // return true if pair
	return ( hands->cardsInHand == 2 ) && 
	       ( cardVal(hands->hand[0]) == cardVal(hands->hand[1]) );
}

/* Splits the two dealt pair cards into two hands
 * No error-checking
 */
void Player::split(BetNode* bets, int idx)
{
	HandNode* handPtr = this->hands;
	BetNode* betsPtr = bets;
	while (idx-- > 0) 
	{
		handPtr = handPtr->next;
		betsPtr = betsPtr->next;
	}

	handPtr->next = new HandNode(false, handPtr->next); // New HandNode after current
    handPtr->next->hand = new int[MAX_HAND_SIZE]; 
	// Second card of first hand becomes first card of split hand
    handPtr->next->hand[0] = handPtr->hand[1];
    handPtr->hand[1] = 0;
	// Both hands now have 1 card
    handPtr->cardsInHand = handPtr->next->cardsInHand = 1;
	
	betsPtr->next = new BetNode(betsPtr->bet, betsPtr->next);
    this->changeDollars(betsPtr->bet, MINUS);
}
/* Adds the top card of the Deck to the specified hand
 * Returns true if bust, false if not
 */
bool Player::hit(Deck *D, BetNode* bets, int dealerCard=0)
{
	struct totalSum t;
	t = getTotalSum(0);
    char input;
    bool bust = false;
    int cardNum,
	    handIdx = 0;
	HandNode* handPtr = this->hands;
	BetNode* betsPtr = bets;
    if(!out(false)) {
        for(int j = 0; handExists(j); ++j) {
			input = 'y';
			if (handPtr->cardsInHand == 1)
				handPtr->hand[handPtr->cardsInHand++] = D->pop();
			
			while( canSplit(j) && input != 'n' ) {
				do {
					cin.clear();
					fflush(stdin);
					displayHand(true);
					printf("Split? $%i (y/n) ", changeDollars(0, 0));
					cin >> input;
				} while (input != 'y' && input != 'n');
				if (input == 'y') {
					split(bets, j);
				}
			
				if (handPtr->cardsInHand == 1)
					handPtr->hand[handPtr->cardsInHand++] = D->pop();
			}
            do {
                do {
                    displayHand(true);
                    printf("Hand %i: Hit? (y/n)", j+1);
					if (handPtr->cardsInHand == 2) printf(" Double Down? (d)");
					printf(": ");
                    cin.clear();
                    fflush(stdin);
                    cin >> input;
                } while (input != 'y' && input != 'n' && input != 'd');
                if (input == 'y') {
                    cardNum = D->pop();
                    handPtr->hand[handPtr->cardsInHand++] = cardNum;
					t = getTotalSum(j);
                    bust = t.total > 21;
                    if(bust) printf("Hand %i: BUST\n", j+1);
                } else if (input == 'n') {
                    stay(j);
                } else if (input == 'd' && handPtr->cardsInHand == 2) {
					this->changeDollars(betsPtr->bet, MINUS);
					betsPtr->bet = 2*(betsPtr->bet);
                    cardNum = D->pop();
                    handPtr->hand[handPtr->cardsInHand++] = cardNum;
					t = getTotalSum(j);
                    bust = t.total > 21;
                    if(bust) printf("Hand %i: BUST\n", j+1);
					stay(j);
					input = 'n';
				}
            } while (input != 'n' && !bust);
			handPtr = handPtr->next;
			betsPtr = betsPtr->next;
        }
    }
    displayHand(true);
}

/* Sets pass on the specified hand to true
 * Returns the totalSum of the specified hand
 */
int Player::stay(int handNum)
{
	HandNode* handPtr = this->hands;
	while ( handNum-- > 0 ) 
		handPtr = handPtr->next;
    handPtr->pass = true;
    return getTotalSum(handNum).total;
}

bool Player::blackjack()
{
    return hands->cardsInHand == 2 && getTotalSum(0).total == 21 && !hands->next;
}

bool Player::canSplit(int handIdx)
{
	HandNode* handPtr = this->hands;
	while ( handIdx-- > 0 ) 
		handPtr = handPtr->next;
	// return true if pair
	return ( handPtr->cardsInHand == 2 ) && 
	       ( cardVal(handPtr->hand[0]) == cardVal(handPtr->hand[1]) );
}

bool Player::splitMoney(int bet)
{
    return dollars >= bet;
}

bool Player::handExists(int handNum)
{
	HandNode* handPtr = this->hands;
	while ( handNum-- > 0 ) 
		handPtr = handPtr->next;
    return handPtr;
}

bool Player::out(bool set = false)
{
    return end = end || set;
}

struct totalSum Player::getTotalSum(int handNum)
{
	struct totalSum t;
    int total = 0;
    int countAceAsHigh = 0;
	HandNode* handPtr = this->hands;
	while ( handNum-- > 0 ) 
		handPtr = handPtr->next;
    for (int i = 0; i < handPtr->cardsInHand; ++i) {
        if ( isAnAce(handPtr->hand[i]) ) {
            if ( total <= 10 ) {
                total += 11;
                ++countAceAsHigh;
            }
            else total += 1;
        }
        else total += cardVal( handPtr->hand[i] );
        if ( countAceAsHigh > 0 && total > 21 ) {
            --countAceAsHigh;
            total -= 10;
        }
    }
	t.total = total;
	t.soft = countAceAsHigh > 0 ? true : false;
    return t;
}

/* All purpose dollar changing function
 * Modes: 0 - Nothing
 *        PLUS - Add
 *        MINUS - Subtract
 */
int Player::changeDollars(int amt, int mode)
{
    switch(mode)
    {
        case PLUS:
            dollars += amt;
            end = false;
            break;
        case MINUS:
            dollars -= amt;
            break;
        default:
            break;
    }
    return dollars;
}

void Player::displayHand(bool UNUSED)
{
	HandNode* handPtr = this->hands;
    for(int i = 0; handPtr; ++i, handPtr = handPtr->next) {
        printf("Hand %i: ", i+1);
        for(int j = 0; j < handPtr->cardsInHand; ++j) {
            printf("%s ", cardTag(handPtr->hand[j]));
        }
        printf("\n");
    }
}

int Player::getRevealedCard()
{
	return 0;
}

void Player::clear()
{
	HandNode* delHandPtr = this->hands;
	HandNode* handPtr = this->hands->next;
	while(handPtr) {
		delHandPtr = handPtr;
		handPtr = handPtr->next;
		delete delHandPtr->hand;
		delete delHandPtr;
	}
    if(hands->hand) delete hands->hand;
    hands->hand = new int[MAX_HAND_SIZE];
	hands->cardsInHand = 0;
	hands->pass = false;
	hands->next = NULL;
}

Dealer::Dealer(int d=9999, bool(*algr)(Player*, Deck*) = NULL) : Player(d), algr(algr) { }

bool Dealer::hit(Deck* D, BetNode* UNUSED = NULL, int dealerCard=0) 
{
	struct totalSum t;
	t = getTotalSum(0);
    bool bust = false;
    int cardNum;
    printf("\n--Dealer Turn--\n");
    displayHand(true);
    while (( t.total < 17 || ( t.soft && t.total < 18) ) && !bust) {
        cardNum = D->pop();
        hands->hand[hands->cardsInHand++] = cardNum;
		t = getTotalSum(0);
        bust = t.total > 21;
        if(bust) printf("Hand %i: BUST\n", 1);
        displayHand(true);
    }
    return bust;
}

void Dealer::displayHand(bool reveal)
{
    reveal ? printf("Dealer Hand: %s ", cardTag(hands->hand[0])) : 
             printf("Dealer Hand: XX ");
    for(int j = 1; j < hands->cardsInHand; ++j) {
        printf("%s ", cardTag(hands->hand[j]));
    }
    printf("\n");
}

bool Dealer::blackjack()
{
	bool bj = hands->cardsInHand == 2 && getTotalSum(0).total == 21;
	if (bj) displayHand(true);
    return bj;
}

int Dealer::getRevealedCard()
{
	return this->hands->hand[1];
}

AI::AI(int d=100, bool(*algr)(Player*, Deck*) = NULL) : Player(d), algr(algr) { }

void AI::placeBet(BetNode* bets)
{
	bets->bet = 15;
	printf("%i\n", bets->bet);
}

bool AI::hit(Deck* D, BetNode* bets, int dealerCard) 
{
	struct totalSum t;
	t = getTotalSum(0);
    char input = 'y';
    bool bust = false;
    int cardNum;
	HandNode* handPtr = this->hands;
	BetNode* betsPtr = bets;
    if(!out(false)) {
        for(int j = 0; handExists(j); ++j) {
			if (handPtr->cardsInHand == 1)
				handPtr->hand[handPtr->cardsInHand++] = D->pop();
			
			while( canSplit(j) && input != 'n' ) {
				do {
					displayHand(true);
					printf("Split? $%i (y/n) ", changeDollars(0, 0));
					input = doSplit(j, dealerCard, bets) ? 'y' : 'n';
					printf("%c\n", input);
				} while (input != 'y' && input != 'n');
				if (input == 'y') {
					split(bets, j);
				}
				if (handPtr->cardsInHand == 1)
					handPtr->hand[handPtr->cardsInHand++] = D->pop();
			}
            do {
                do {
                    displayHand(true);
                    printf("Hand %i: Hit or Stay? (h/s)", j+1);
					if (handPtr->cardsInHand == 2) printf(" Double Down? (d)");
					printf(": ");
                    input = hitChoice(j, dealerCard);
					printf("%c\n", input);
                } while (input != 'h' && input != 's' && input != 'd');
                if (input == 'h') {
                    cardNum = D->pop();
                    handPtr->hand[handPtr->cardsInHand++] = cardNum;
					t = getTotalSum(j);
                    bust = t.total > 21;
                    if(bust) printf("Hand %i: BUST\n", j+1);
                } else if (input == 's') {
                    stay(j);
                } else if (input == 'd' && handPtr->cardsInHand == 2) {
					this->changeDollars(betsPtr->bet, MINUS);
					betsPtr->bet = 2*(betsPtr->bet);
                    cardNum = D->pop();
                    handPtr->hand[handPtr->cardsInHand++] = cardNum;
					t = getTotalSum(j);
                    bust = t.total > 21;
                    if(bust) printf("Hand %i: BUST\n", j+1);
					stay(j);
					input = 's';
				}
            } while (input != 's' && !bust);
			handPtr = handPtr->next;
			betsPtr = betsPtr->next;
        }
    }
    displayHand(true);
}

bool AI::doSplit (int handNum, int dealerCard, BetNode* bets)
{
	HandNode* handPtr = this->hands;
	BetNode* betPtr = bets;
	while ( handNum-- > 0 ) { 
		handPtr = handPtr->next;
		betPtr = betPtr->next;
	}
	if ( this->dollars > betPtr->bet)
		return pairSplit[10 * (cardVal(handPtr->hand[0]) - 1 ) + cardVal(dealerCard) - 1] == 'y';
	else return false;
}

char AI::hitChoice (int handNum, int dealerCard)
{
	struct totalSum t;
	t = getTotalSum(handNum);
	char choice;
	
	HandNode* handPtr = this->hands;
	while ( handNum-- > 0 ) 
		handPtr = handPtr->next;
	
	int fixedTotal;
	if (t.soft) {
		if (t.total < 13) fixedTotal = 13;
		else if (t.total > 20) fixedTotal = 20;
		else fixedTotal = t.total;
		choice = softHit[ 10 * (fixedTotal - 13) + cardVal(dealerCard) - 1 ];
	} else {
		if (t.total < 8) fixedTotal = 8;
		else if (t.total > 17) fixedTotal = 17;
		else fixedTotal = t.total;
		choice = hardHit[ 10 * (fixedTotal - 8) + cardVal(dealerCard) - 1 ];
	}
	
	if ( choice == 'd' && handPtr->cardsInHand != 2 )
		choice = 'h';
	return choice;
}