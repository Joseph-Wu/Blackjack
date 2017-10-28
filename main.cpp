#include "Blackjack.cpp"
//#include "Blackjack_Sim.cpp"
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    int AINum,
		cycles,
		runs;
    printf("How many computers? ");
    cin >> AINum;
    printf("How many cycles? ");
    cin >> cycles;
    printf("How many runs? ");
    cin >> runs;
    printf("\n");
	for (int i = 0; i < runs; ++i) {
		run_Blackjack(1, AINum);
		Sleep(1000);
	}
    
    Sleep(2000);
    return 0;
}