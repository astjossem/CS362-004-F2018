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

//card test 1 which tests the smithy card
int main() {
	int i, j, difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Smithy Card****\n\n");
	//initialize game and make Deck large enough to cycle through
	initializeGame(2,k,10,&G);
	for(i=0; i < 25;i++) {
		G.deck[player][G.deckCount[player]] = i;
		G.deckCount[player]++;
	}

	//test cardEffect with from position 1-5 cards in player's hand
	for(i = 0; i < 10; i++) {
		if(i < 5) {
			G.hand[player][i] = smithy;
		}
		else {
			G.hand[player][i-5] = smithy;
		}
		memcpy(&preG,&G,sizeof(struct gameState));
		cardEffect(smithy,choice1,choice2,choice3,&G,i,&bonus);
		
		//compare current hand count - should have 2 more (3 drawn, 1 discarded)
		if(!asserttrue(G.handCount[player] == preG.handCount[player]+2, &works)) {
			printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]+2);
			printf("iteration: %d\n\n",i);
		}
		//compare deck counts - should have 3 fewer
		if(!asserttrue(G.deckCount[player] == preG.deckCount[player] - 3, &works)) {
			printf("Deck count error: Has %d cards in deck, should have %d\n",G.deckCount[player],preG.deckCount[player]-3);
			printf("iteration: %d\n\n",i);
			
		}
		//smithy should be in played pile
		if(!asserttrue(G.playedCardCount == preG.playedCardCount + 1,&works)) {
			printf("played card count not corred: has %d, expected %d\n", G.playedCardCount, preG.playedCardCount + 1);
			printf("iteration: %d\n\n",i);
		}
		//other player had no status change
		if(!asserttrue(G.handCount[player+1] == preG.handCount[player+1],&works)) {
			printf("Error in other player gamestate: Hand Count\n");
		}
		if(!asserttrue(G.deckCount[player+1] == preG.deckCount[player+1],&works)) {
			printf("Error in other player gamestate: Deck Count\n");
		}
		//no change in supply cards (victory, treasure, curse, or otherwise)
		for(j=adventurer; j <= treasure_map;j++) {
			if(!asserttrue(G.supplyCount[j] == preG.supplyCount[j],&works)) {
				printf("Error in supply cards state: Card position %d\n",j);
			}
		}
	}
	if(works == 1) {
		printf("NO ERRORS IN TESTING: ALL TESTS PASSED\n");
	}
	return 0;
}
