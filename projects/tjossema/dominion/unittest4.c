#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int asserttrue(int a, int* works) {
	if(!a) {
		*works = 0;
	}
	return a;
}

//unit test 4 which tests the update coins function
int main() {
	int i, j, l, counter,difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Update Coins****\n\n");
	//test update with 0-10 cards in hand
	for(i = 0; i < 11; i++) {
		//tests with bonus between 0-5
		for(bonus=0; bonus < 5; bonus++) {
			initializeGame(2,k,10,&G);
			counter =  bonus;
			//eliminate player 1 and 2's treasure cards
			for(l=0;l<5;l++) {
				G.hand[player+1][l] = 1;
				G.hand[player][l] = 1;
			}
			G.handCount[player] = i;
			//test with 0-2 copper
			if(i>1) {
				counter++;
				G.hand[player][1] = copper;
				if(i>2) {
					counter++;
					G.hand[player][2] = copper;
				}
			}
			//test with 0-2 gold
			if(i>5) {
				counter += 3;
				G.hand[player][0] = gold;
				if(i>7) {
					counter += 3;
					G.hand[player][7] = gold;
				}
			}
			//test with 0-2 silver
			if(i>8) {
				counter += 2;
				G.hand[player][4] = silver;
				if(i>10) {
					counter += 2;
					G.hand[player][10] = silver;
				}
			}

			memcpy(&preG,&G,sizeof(struct gameState));
			updateCoins(player,&G,bonus);

			//player 1 coins are correct
			if(!asserttrue(G.coins == counter,&works)) {
				printf("ERROR: Coin count incorrect: Has: %d Expected: %d\n",G.coins,counter);
				printf("iteration: #Handsize: %d #Bonus: %d\n\n",i,bonus); 
			}
			//player 2 cards don't effect count
			//add 5 copper to player 2 hand
			for(l=0;l<5;l++) {
				G.hand[player+1][i] = copper;
			}
			memcpy(&preG,&G,sizeof(struct gameState));
			updateCoins(player,&G,bonus);
			if(!asserttrue(G.coins == preG.coins,&works)) {
				printf("ERROR: player2 coins affected player1 count: Has: %d Expected: %d\n",G.coins,counter);
			}
		}
	}
	if(works == 1) {
		printf("NO ERRORS IN TESTING: ALL TESTS PASSED\n");
	}
	return 0;
}
