#ifndef CARDS_H
#define CARDS_H
#include <stdio.h>
const int CA = 0, C2 = 1, C3 = 2, C4 = 3, C5 = 4, C6 = 5, C7 = 6, C8 = 7, C9 = 8, C10 = 9, CJ = 10, 
        CQ = 11, CK = 12, DA = 13, D2 = 14, D3 = 15, D4 = 16, D5 = 17, D6 = 18, D7 = 19, D8 = 20, D9 = 21, 
        D10 = 22, DJ = 23, DQ = 24, DK = 25, HA = 26, H2 = 27, H3 = 28, H4 = 29, H5 = 30, H6 = 31, H7 = 32, 
        H8 = 33, H9 = 34, H10 = 35, HJ = 36, HQ = 37, HK = 38, SA = 39, S2 = 40, S3 = 41, S4 = 42, S5 = 43, 
        S6 = 44, S7 = 45, S8 = 46, S9 = 47, S10 = 48, SJ = 49, SQ = 50, SK = 51;

const int cardList[] = {CA, C2, C3, C4, C5, C6, C7, C8, C9, C10, CJ, CQ, CK,
                        DA, D2, D3, D4, D5, D6, D7, D8, D9, D10, DJ, DQ, DK,
                        HA, H2, H3, H4, H5, H6, H7, H8, H9, H10, HJ, HQ, HK,
                        SA, S2, S3, S4, S5, S6, S7, S8, S9, S10, SJ, SQ, SK};
/*
const int cardList[] = {CA, CA, CA, CA, CA, CA, CA, CA, CA, CA, CA, CA, CA,
                        DA, DA, DA, DA, DA, DA, DA, DA, DA, DA, DA, DA, DA,
                        HA, HA, HA, HA, HA, HA, HA, HA, HA, HA, HA, HA, HA,
                        SA, SA, SA, SA, SA, SA, SA, SA, SA, SA, SA, SA, SA};
*/
char* cardTag( int cardNum )
{
    char *str = new char[4];
    str[2] = '\0';
    if( cardNum < 13 ) str[0] = 'C';
    else if( cardNum >= 13 && cardNum < 26 ) str[0] = 'D';
    else if( cardNum >= 26 && cardNum < 39 ) str[0] = 'H';
    else str[0] = 'S';
    switch(cardNum)
    {
        case 0:
        case 13:
        case 26:
        case 39:
            str[1] = 'A'; break;
        case 1:
        case 14:
        case 27:
        case 40:
            str[1] = '2'; break;
        case 2:
        case 15:
        case 28:
        case 41:
            str[1] = '3'; break;
        case 3:
        case 16:
        case 29:
        case 42:
            str[1] = '4'; break;
        case 4:
        case 17:
        case 30:
        case 43:
            str[1] = '5'; break;
        case 5:
        case 18:
        case 31:
        case 44:
            str[1] = '6'; break;
        case 6:
        case 19:
        case 32:
        case 45:
            str[1] = '7'; break;
        case 7:
        case 20:
        case 33:
        case 46:
            str[1] = '8'; break;
        case 8:
        case 21:
        case 34:
        case 47:
            str[1] = '9'; break;
        case 9:
        case 22:
        case 35:
        case 48:
            str[1] = '1';
            str[2] = '0';
            str[3] = '\0';
            break;
        case 10:
        case 23:
        case 36:
        case 49:
            str[1] = 'J'; break;
        case 11:
        case 24:
        case 37:
        case 50:
            str[1] = 'Q'; break;
        case 12:
        case 25:
        case 38:
        case 51:
            str[1] = 'K'; break;
        default:
            str[1] = '\0';
    }
    return str;
}

int cardVal( int cardNum )
{
    int val;
    switch(cardNum)
    {
        case 0:
        case 13:
        case 26:
        case 39:
            val = 1; break;
        case 1:
        case 14:
        case 27:
        case 40:
            val = 2; break;
        case 2:
        case 15:
        case 28:
        case 41:
            val = 3; break;
        case 3:
        case 16:
        case 29:
        case 42:
            val = 4; break;
        case 4:
        case 17:
        case 30:
        case 43:
            val = 5; break;
        case 5:
        case 18:
        case 31:
        case 44:
            val = 6; break;
        case 6:
        case 19:
        case 32:
        case 45:
            val = 7; break;
        case 7:
        case 20:
        case 33:
        case 46:
            val = 8; break;
        case 8:
        case 21:
        case 34:
        case 47:
            val = 9; break;
        case 9:
        case 22:
        case 35:
        case 48:
        
        case 10:
        case 23:
        case 36:
        case 49:
        
        case 11:
        case 24:
        case 37:
        case 50:
        
        case 12:
        case 25:
        case 38:
        case 51:
            val = 10; break;
        default:
            val = 0;
    }
    return val;
}

bool isAnAce( int cardNum )
{
    switch( cardNum ) {
        case CA:
        case DA:
        case HA:
        case SA:
            return true;
        default:
            break;
    }
    return false;
}
//const int cardList[] = {A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K};

#endif