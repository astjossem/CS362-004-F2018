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

//card test 1 which tests the great_hall card
int main() {
	int i, j, difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	initializeGame(2,k,10,&G);
	printf("\n\n****Testing Great Hall Card****\n\n");
	//add extra cards to deck
	for(i = 0; i < 5; i++) {
		G.deck[player][G.deckCount[player]] = i;
		G.deckCount[player]++;
	}
	//test cardEffect with from position 1-5 cards in player's hand
	for(i = 0; i < 10; i++) {
		G.hand[player][i] = great_hall;
		memcpy(&preG,&G,sizeof(struct gameState));
		cardEffect(great_hall,choice1,choice2,choice3,&G,i,&bonus);
	
		//compare current hand count - should be the same (1 drawn, 1 discarded)
		if(!asserttrue(G.handCount[player] == preG.handCount[player], &works)) {
			printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]);
			printf("iteration: %d\n\n",i);
		}
		//compare deck counts - should have 1 fewer
		if(!asserttrue(G.deckCount[player] == preG.deckCount[player] - 1, &works)) {
			printf("Deck count error: Has %d cards in deck, should have %d\n",G.deckCount[player],preG.deckCount[player]);
			printf("iteration: %d\n\n",i);
		
		}
		//perform 1 actions (G->numActions + 1)
		//check actions are +1
		if(!asserttrue(G.numActions == preG.numActions +1,&works)) {
			printf("Increase Action by 1 error: Has %d actions when should have %d\n",G.numActions,preG.numActions + 2);
			printf("iteration: %d\n\n",i);
		}
		//great_hall should be in discard pile
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
