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

//card test 4 which tests the adventurer card
int main() {
	int i, j, t1=-1,t2=-1,extras=0,difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	int cards[12] = {4,4,4,4,4,4,4,4,4,4,4,4};
	printf("\n\n****Testing Adventurer Card****\n\n");
	//test cardEffect with from position 1-5 cards in player's hand
	for(i = 0; i < 10; i++) {
		int newT=0,oldT=0;
		initializeGame(2,k,10,&G);
		if(i < 5) {
			G.hand[player][i] = adventurer;
		}
		else {
			G.hand[player][i-5] = adventurer;
		}
		//calculate how many cards **Should** be discarded by adventurer because they are not treasure
		//printf("CARDS IN HAND (TOP TO BOTTOM)\n");
		t1 = -1;
		t2 = -1;
		if(i > 0) {
			cards[12-i] = 1;
		}
		//add non treasure card to hand to avoid duplicate deck values
		G.deckCount[player] = 12;
		for(j=G.deckCount[player]-1;j>=0;j--) {
			
			G.deck[player][j] = cards[j];
			//printf("%d\n",G.deck[player][j]);
			if(t1 == -1) {
				if(G.deck[player][j] > 3 && G.deck[player][j] < 7) {
					t1 = j;
				}
			}
			else {
				if(t2 == -1) {
					if(G.deck[player][j] > 3 && G.deck[player][j] < 7) {
						t2 = j;
					}
				}
			}	 
		}
		//printf("Treasure at position %d, and %d\n",t1,t2);
		if(t1 != -1 && t2 != -1) {
			extras = (11-t1) + (t1-t2-1);		}
		else {
			extras = -1;
		}
		printf("Cards to be discarded: %d\n\n",extras);
		
		memcpy(&preG,&G,sizeof(struct gameState));
		cardEffect(adventurer,choice1,choice2,choice3,&G,i,&bonus);
		//draw 2 card
		//compare current hand count - should be the 1 more (2 drawn, 1 discarded)
		if(!asserttrue(G.handCount[player] == preG.handCount[player]+1, &works)) {
			printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]+1);
			printf("iteration: %d\n\n",i);
		}
		//Should have 2 additional treasure cards in hand
		for(j=0;j<G.handCount[player];j++) {
			if(G.hand[player][j] == copper)
				newT++;
			if(G.hand[player][j] == silver)
				newT++;
			if(G.hand[player][j] == gold)
				newT++;
		}
		for(j=0;j<preG.handCount[player];j++) {
			if(preG.hand[player][j] == copper)
				oldT++;
			if(preG.hand[player][j] == silver)
				oldT++;
			if(preG.hand[player][j] == gold)
				oldT++;
		}
		if(!asserttrue(newT == oldT+2,&works)) {
			printf("2 New Treasure Error: Old Total: %d New Total: %d\n",oldT,newT);
			printf("iteration: %d\n\n",i);
		}
		//discard should have adventurer as well as every drawn non treasure card (extras)
		if(!asserttrue(G.discardCount[player] == preG.discardCount[player] + 1 + extras,&works)) {
			printf("played card error: Have: %d, Should Have: %d\n",G.discardCount[player],preG.discardCount[player] + 1 + extras);
			printf("iteration: %d\n\n",i);
		}
		for(j=0;j<G.discardCount[player];j++) {
			if(!asserttrue(G.discard[player][j] != -1,&works)) {
				printf("Error in discard pile - null result\n");
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
