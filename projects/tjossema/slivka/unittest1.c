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

//unit test 1 which tests the discard card function
int main() {
	int i, j, l, difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Discard function****\n\n");
	//test discard with 1-10 cards in hand
	for(i = 1; i < 11; i++) {
		//test at every position in the hand
		for(handPos=0; handPos < i;handPos++) {
			//test with 0-2 cards in discard pile
			for(j=0;j<3;j++) {
				initializeGame(2,k,10,&G);
				//set hand size
				G.handCount[player] = i;
				//fill hand to hand size
				for(l=5;l<i;l++) {
					G.hand[player][l] = l;
				}
				//set card at hand position to be a unique card (for validation purposes)
				G.hand[player][handPos] = 20;
				//fill discard pile to correct size
				G.discardCount[player] = j;
				G.discard[player][j] = -1;
				//create previously discarded cards
				if(j>0) {
					G.discard[player][0] == 3;
				}
				if(j==2) {
					G.discard[player][1] == 4;
				}
				memcpy(&preG,&G,sizeof(struct gameState));
				discardCard(handPos,player,&G,0);

				//compare current hand count - should be 1 less
				if(!asserttrue(G.handCount[player] == preG.handCount[player]-1, &works)) {
					printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]-1);
					printf("iteration: #HandSize: %d #HandPosition: %d #DiscardSize: %d \n\n",i,handPos,j);
				}
				//compare deck counts - should be same
				if(!asserttrue(G.deckCount[player] == preG.deckCount[player], &works)) {
					printf("Deck count error: Has %d cards in deck, should have %d\n",G.deckCount[player],preG.deckCount[player]);
					printf("iteration: #HandSize: %d #HandPosition: %d #DiscardSize: %d\n\n",i,handPos,j);
		
				}
				//Card at HandPos  should be in discard pile
				if(!asserttrue(G.discard[player][G.discardCount[player]-1] == preG.hand[player][handPos],&works)) {
					printf("Card Discard Error - Card from hand not at expected discard location\n");
					printf("iteration: #HandSize: %d #HandPosition: %d #DiscardSize: %d\n\n",i,handPos,j);
				}
				//Card at HandPos should not be at HandPos
				if(!asserttrue(G.hand[player][handPos] != 20,&works)) {
					printf("Card Position Error - Card at hand position is same card (was not discarded)\n");
					printf("iteration: #Handsize: %d #HandPosition: %d #DiscardSize: %d\n\n",i,handPos,j);
				}
				//discard pile should have 1 more card
				if(!asserttrue(G.discardCount[player] == preG.discardCount[player] +1, &works)) {
					printf("Discard Error: Discard pile has %d should have %d\n",G.discardCount[player],preG.discardCount[player]+1);
					printf("iteration: #Handsize: %d #HandPosition: %d #DiscardSize: %d\n\n",i,handPos,j);
				}
				//discard pile should be same with 1 additional card on top
				for(l=0;l<j;l++) {
					if(!asserttrue(G.discard[player][l] == preG.discard[player][preG.discardCount[l]],&works)) {
						printf("Discard pile not correct cards / order\n");
						break;
					}
				}
				//other player had no status change
				if(!asserttrue(G.handCount[player+1] == preG.handCount[player+1],&works)) {
					printf("Error in other player gamestate: Hand Count\n");
				}
				if(!asserttrue(G.deckCount[player+1] == preG.deckCount[player+1],&works)) {
					printf("Error in other player gamestate: Deck Count\n");
				}
				//no change in supply cards (victory, treasure, curse, or otherwise)
				for(l=adventurer; l <= treasure_map;l++) {
					if(!asserttrue(G.supplyCount[l] == preG.supplyCount[l],&works)) {
						printf("Error in supply cards state: Card position %d\n",l);
					}
				}
			}
		}
	}
	if(works == 1) {
		printf("NO ERRORS IN TESTING: ALL TESTS PASSED\n");
	}
	return 0;
}
