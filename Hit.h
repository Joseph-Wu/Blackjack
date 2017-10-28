#ifndef HIT_H
#define HIT_H
#include <stdio.h>
#include "Player.h"

/* Dealer hit algorithm
 *
 */
bool hitDealer(Player *dealer, Deck *D) {
    bool bust = false;
    int cardNum;
    printf("\n--Dealer Turn--\n");
    dealer->displayHand(true);
    while (dealer->getTotalSum(0) < 17 && !bust) {
        cardNum = D->pop();
        dealer->addToHand(cardNum);
        bust = dealer->getTotalSum(0) > 21;
        if(bust) printf("Hand %i: BUST\n", 1);
        dealer->displayHand(true);
    }
    return bust;
}

#endif