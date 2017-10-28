#include <iostream>
#include <time.h>
#include "Deck.cpp"
#include "Player.cpp"

#define PLUS 1
#define MINUS 2

using namespace std;

void run_Blackjack(int, int);
void startRound(Player**, Deck*, int, BetNode**);
//void hitPlayers(Player**, Deck*, int, int*, bool*);
//bool hitDealer(Player*, Deck*);
void dealerBlackjack(Player**, int, BetNode**);
void determineWinners(Player**, int, bool, BetNode**);
bool checkValidPlayers(Player**, int);

void run_Blackjack(int numOfPlayers, int numOfAI)
{
	int totalPlayers = numOfPlayers+numOfAI+1;
    printf("Game Start: %i Players\n", totalPlayers);
    Deck *D = new Deck(6, time(NULL)); // (Number of decks, seeding)
	//int* revealedCards = new int[6*52]; // Keeps track of revealed cards
										  // Future options
    Player **players = new Player*[totalPlayers];
	int i = 0;
    for(i = 0; i < numOfPlayers; ++i)
        players[i] = new Player();
    for(int j = 0; j < numOfAI; ++j)
        players[i+j] = new AI(1000);
    players[totalPlayers-1] = new Dealer(9999);
    // last player is the dealer
    BetNode **bets = new BetNode*[totalPlayers-1]; // Bets on the table
	for ( i = 0; i < totalPlayers; ++i) bets[i] = new BetNode(0);
    bool dealerBust = false, valid;
    char cont = 'y';
    do {
		printf("\n--New Round--\n");
        startRound(players, D, totalPlayers, bets);
		if (players[totalPlayers-1]->blackjack()) {
			dealerBlackjack(players, totalPlayers, bets);
		} else {
			for( i = 0; i < totalPlayers - 1; ++i) {
				printf("\n--Player %i Turn--\n", i+1);
				players[i]->hit(D, bets[i], players[totalPlayers-1]->getRevealedCard()); // NOTE! Don't pass the deck in. Pass a card
			}
			dealerBust = players[totalPlayers-1]->hit(D, NULL);
			determineWinners(players, totalPlayers, dealerBust, bets);
			valid = checkValidPlayers(players, totalPlayers);
		}
		if (valid)
			do {
				printf("Continue? (y/n) ");
				cin.clear();
				fflush(stdin);
				cin >> cont;
			} while (cont != 'y' && cont != 'n');
    } while (cont == 'y' && valid);
    if(!valid) printf("All players are out of money!\n");
    printf("Game End\n");
	
	for ( i = 0; i < totalPlayers - 1; ++i) {
		printf("Player %i: $%i\n", i+1, players[i]->changeDollars(0, 0));
	}
}

/* Sets up the beginning of each round
 */
void startRound(Player **players, Deck *D, int totalPlayers, BetNode** bets)
{
    if( D->lessThan(20) ) D->shuffle(time(NULL));
    for(int i = 0; i < totalPlayers - 1; ++i) {
        players[i]->clear(); // Clear player's hand
		// Clear Player's bets
		BetNode* delBetNode = bets[i];
		BetNode* betPtr = delBetNode->next;
		while (betPtr) {
			delBetNode = betPtr;
			betPtr = betPtr->next;
			delete delBetNode;
		}
		bets[i]->next = NULL;
		
        if(!players[i]->out()) {
            do {
                cin.clear();
                fflush(stdin);
                printf("Player %i: $%i: Place your bet: $", i+1, 
                       players[i]->changeDollars(0, 0));
                players[i]->placeBet(bets[i]);
            } while (bets[i]->bet < 1 || bets[i]->bet > players[i]->changeDollars(0, 0));
            players[i]->changeDollars(bets[i]->bet, MINUS);
        }
    }
    players[totalPlayers - 1]->clear(); // Clear dealer's hand as well
    for(int j = 0; j < 2; ++j)
        for(int i = 0; i < totalPlayers; ++i) {
            if(!players[i]->out()) 
                players[i]->deal(D->pop()) && players[i]->splitMoney(bets[i]->bet);
        }
    for(int i = 0; i < totalPlayers; ++i) {
        if(!players[i]->out()) {
            printf("Player %i: $%i: ", i+1, players[i]->changeDollars(0, 0));
            players[i]->displayHand(false);
        }
    }
}

/* Results are determined
 * BUST - Lose bet
 * Lost - Lose bet
 * Draw - Return bet
 * Win - Double bet
 * Push - Return bet
 * Blackjack - 2.5 * bet
 */ 

void dealerBlackjack(Player **players, int totalPlayers, BetNode** bets) {
	struct totalSum playerSum, dealerSum = players[totalPlayers - 1]->getTotalSum(0);
	BetNode* betsPtr = NULL;
	printf("DEALER BLACKJACK! All players without Blackjack lose!\n");
        for(int i = 0; i < totalPlayers - 1; ++i) {
            betsPtr = bets[i];
			if(!players[i]->out()) {
                for(int j = 0; players[i]->handExists(j); ++j, betsPtr = betsPtr->next) {
                    playerSum = players[i]->getTotalSum(j);
                    if(players[i]->blackjack())
                        printf("Player %i: Hand %i: BLACKJACK PUSH; $%i\n", i+1, j+1, 
                               players[i]->changeDollars(betsPtr->bet, PLUS));
                    else
                        printf("Player %i: Hand %i: Lost; $%i\n", i+1, j+1, 
                               players[i]->changeDollars(0, 0));
                }
            }
            if ( players[i]->changeDollars(0, 0) <= 0 ) players[i]->out(true);
        }
} 

void determineWinners(Player **players, int totalPlayers, bool dealerBust, BetNode** bets) {
    struct totalSum playerSum, dealerSum = players[totalPlayers - 1]->getTotalSum(0);
	BetNode* betsPtr = NULL;
    if(dealerBust) {
        printf("Dealer Busted! All left in the game players win!\n");
        for(int i = 0; i < totalPlayers - 1; ++i) {
			betsPtr = bets[i];
            if(!players[i]->out()) {
                for(int j = 0; players[i]->handExists(j); ++j, betsPtr = betsPtr->next) {
					playerSum = players[i]->getTotalSum(j);
					if(players[i]->blackjack())
						printf("Player %i: Hand %i: Blackjack! $%i\n", i+1, j+1, 
							   players[i]->changeDollars( (5 * betsPtr->bet / 2), PLUS));
					else if(playerSum.total > 21)
						printf("Player %i: Hand %i: Sum: %i BUST; $%i\n", i+1, j+1, 
							   playerSum.total, players[i]->changeDollars(0, 0));
					else 
						printf("Player %i: Hand %i: Sum: %i Won; $%i\n", i+1, j+1, 
							   playerSum.total, players[i]->changeDollars(betsPtr->bet * 2, PLUS));
                }
            }
            if ( players[i]->changeDollars(0, 0) <= 0 ) players[i]->out(true);
        }
    } else {
        printf("Dealer's Hand is %i\n", dealerSum);
        for(int i = 0; i < totalPlayers - 1; ++i) {
			betsPtr = bets[i];
            if(!players[i]->out()) {
                for(int j = 0; players[i]->handExists(j); ++j, betsPtr = betsPtr->next) {
                    playerSum = players[i]->getTotalSum(j);
                    if(players[i]->blackjack())
                        printf("Player %i: Hand %i: Blackjack! $%i\n", i+1, j+1, 
                               players[i]->changeDollars( (5 * betsPtr->bet / 2), PLUS));
                    else if(playerSum.total > 21)
                        printf("Player %i: Hand %i: Sum: %i BUST; $%i\n", i+1, j+1, 
                               playerSum.total, players[i]->changeDollars(0, 0));
                    else if(playerSum.total < dealerSum.total)
                        printf("Player %i: Hand %i: Sum: %i Lost; $%i\n", i+1, j+1, 
                               playerSum.total, players[i]->changeDollars(0, 0));
                    else if(playerSum.total == dealerSum.total)
                        printf("Player %i: Hand %i: Sum: %i Draw; $%i\n", i+1, j+1, 
                               playerSum.total, players[i]->changeDollars(betsPtr->bet, PLUS));
                    else if(playerSum.total > dealerSum.total)
                        printf("Player %i: Hand %i: Sum: %i Won; $%i\n", i+1, j+1, 
                               playerSum.total, players[i]->changeDollars(betsPtr->bet * 2, PLUS));
                }
            }
            if ( players[i]->changeDollars(0, 0) <= 0 ) players[i]->out(true);
        }
    }
}

bool checkValidPlayers(Player **players, int totalPlayers) {
    bool valid = false;
    for(int i = 0; i < totalPlayers-1 && !valid; ++i)
        if ( !players[i]->out() ) valid = true;
    return valid;
}