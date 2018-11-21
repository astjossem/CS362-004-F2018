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

//unit test 2 which tests the draw card function
int main() {
	int i, j, l, m,difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Draw Card****\n\n");
	//test draw with 0-5 cards in hand
	for(i = 1; i < 6; i++) {
		//test draw with 0-5 cards in deck
		for(j=0; j < 6;j++) {
			//test with 1-5 cards in discard pile
			for(m=1;m<6;m++) {
				initializeGame(2,k,10,&G);
				//set hand & deck
				G.handCount[player] = i;
				G.deckCount[player] = j;
				//remove initialized cards in deck that are not used
				for(l=j;l<5;l++) {
					G.deck[player][l] = -1;
				}
				//fill hand to hand size
				for(l=0;l<i;l++) {
					G.hand[player][l] = l;
				}
				//remove initialized cards in hand that are not used
				for(l=i;l<5;l++) {
					G.hand[player][l] = -1;
				}
				//fill discard pile to correct size
				G.discardCount[player] = m;
				G.discard[player][j] = -1;
				//create previously discarded cards
				for(l=0;l<m;l++) {
					G.discard[player][l] = 1;
				}
				memcpy(&preG,&G,sizeof(struct gameState));
				drawCard(player,&G);

				//compare current hand count - should be 1 more
				if(!asserttrue(G.handCount[player] == preG.handCount[player]+1, &works)) {
					printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]+1);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d \n\n",i,j,m);
				}
				//compare deck counts - should have 1 less (unless shuffled) - then one less of discard
				if(preG.deckCount[player] != 0) {
					if(!asserttrue(G.deckCount[player] == preG.deckCount[player]-1, &works)) {
						printf("Not Shuffled: Deck count error: Has %d cards in deck, should have %d\n",
										G.deckCount[player],preG.deckCount[player]-1);
						printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);	
					}
				}
				else {
					if(!asserttrue(G.deckCount[player] == preG.discardCount[player] - 1,&works)) {
						printf("Shuffled: Deck count error: Has %d cards in deck, should have %d\n",
										G.deckCount[player],preG.discardCount[player]-1);
						printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
					}
				}
				//Test that shuffle only occurs when deck size = 0 (Check by looking at discard piles - should be same)
				if(preG.deckCount[player] != 0) {
					if(!asserttrue(G.discardCount[player] == preG.discardCount[player],&works)) {
						printf("Shuffle Error: Shuffled when deck size wasn't 0\n");
						printf("iteration: #Handsize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
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
