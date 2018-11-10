#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int asserttrue(int a, int* works, struct gameState* state) {
	if(!a) {
		*works = 0;
		printf("Error:\nPlayers: %d\nOutpostPlayed: %d\nOutpostTurn: %d\nWhoseTurn: %d\nNumActions: %d\nCoins: %d\n"
			"numBuys: %d\nHandCount: %d\nDeckCount: %d\nDiscardCount: %d\nPlayedCardCount: %d\n",
			state->numPlayers,state->outpostPlayed,state->outpostTurn,state->whoseTurn,state->numActions,
			state->coins,state->numBuys,state->handCount[state->whoseTurn],state->deckCount[state->whoseTurn],
			state->discardCount[state->whoseTurn],state->playedCardCount);
	}
	return a;
}
//random function which takes a max and min value and returns pseudo random result
int randomNum(int max, int min) {
	return rand() % (max - min + 1) + min;
}

//random card test 1 which tests the adventurer card
int main() {
	//seed the rand function
	srand(time(0));
	int i, j,l, loc,t1=-1,t2=-1,extras=0,difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player,works=1,totalDeck,newT,oldT;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Adventurer Card****\n\n");
	//test cardEffect with from position 1-5 cards in player's hand
	for(i = 0; i < 100000; i++) {
		//max players are 4 so randomize 2-4 plaeyrs
		G.numPlayers = randomNum(4,2);
		//randomize supply count and embargo tokens- max is 100 to give high count examples, min is 1 because certain cards MUST exist
		for(j=0;j<treasure_map+1;j++) {
			G.supplyCount[j] = randomNum(100,1);
			G.embargoTokens[j] = randomNum(100,1);
		}
		//randomize outpostPlayed and outpost turn 0-1 because it is a simple bool value
		G.outpostPlayed = randomNum(1,0);
		G.outpostTurn = randomNum(1,0);
		//randomize whose turn it is - can obviously not go higher than the total number of players playing
		G.whoseTurn = randomNum(G.numPlayers-1,0);
		player = G.whoseTurn;
		//phase must be 0 to play card
		G.phase = 0;
		//randomize num actions - every player gets at least 1 action.
		G.numActions = randomNum(5,1);
		//randomize number of coins - need at least 10
		G.coins = randomNum(100,10);
		//randomize number of buys - at least 1 per turn
		G.numBuys = randomNum(5,1);
		//for each player
		//randomize hand size between 100 and 1 - need at least 1 card in hand so can play said card
		//do this for each player and then randomize cards in hand
		//randomize deck count and deck cards between 100 and 2 (must be 2 treasure cards to draw)
		//randomize discard count and cards between 100 and 0 (can have 0 cards in discard)
		for(j=0;j<G.numPlayers;j++) {
			G.handCount[j] = randomNum(100,1);
			G.deckCount[j] = randomNum(100,3);
			G.discardCount[j] = randomNum(100,0);
			//1 card MUST be adventurer - in order to be played
			loc = randomNum(G.handCount[j]-1,0);
			for(l=0;l<G.handCount[j];l++) {
				//26 cards to randomize
				//if location of adventurer place adventurer
				if(loc == l) {
					G.hand[j][l] = adventurer;
				}
				else {
					G.hand[j][l] = randomNum(26,0);
				}
			}
			for(l=0;l<G.deckCount[j];l++) {
				G.deck[j][l] = randomNum(26,0);
			}
			for(l=0;l<G.discardCount[j];l++) {
				G.discard[j][l] = randomNum(26,0);
			}
		}

		//randomize played cards
		G.playedCardCount = randomNum(100,0);
		for(j=0;j<G.playedCardCount;j++) {
			G.playedCards[j] = randomNum(26,0);
		}
		//calculate how many cards **Should** be discarded by adventurer because they are not treasure
		//printf("CARDS IN HAND (TOP TO BOTTOM)\n");
		t1 = -1;
		t2 = -1;
		//add non treasure card to hand to avoid duplicate deck values
		for(j=G.deckCount[player]-1;j>=0;j--) {
			
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
		if(t1 == -1) {
			G.deck[player][2] = 4;
		}
		if(t2 == -1) {
			G.deck[player][1] = 4;
		}
		//printf("Treasure at position %d, and %d\n",t1,t2);
		extras = (G.deckCount[player]-t1) + (t1-t2-1);
		//printf("Cards to be discarded: %d\n\n",extras);		
		memcpy(&preG,&G,sizeof(struct gameState));
		cardEffect(adventurer,choice1,choice2,choice3,&G,loc,&bonus);
		//draw 2 card
		//compare current hand count - should be the 1 more (2 drawn, 1 discarded)
		if(!asserttrue(G.handCount[player] == preG.handCount[player]+1, &works,&preG)) {
			printf("Draw card error: Has %d cards when should have %d cards\n", G.handCount[player], preG.handCount[player]+1);
		}
		//Should have 2 additional treasure cards in hand
		newT = 0;
		oldT = 0;
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
		if(!asserttrue(newT == oldT+2,&works,&preG)) {
			printf("2 New Treasure Error: Old Total: %d New Total: %d\n",oldT,newT);
		}
		//discard should have adventurer as well as every drawn non treasure card (extras)
		if(!asserttrue(G.discardCount[player] == preG.discardCount[player] + 1 + extras,&works,&preG)) {
			printf("discard card error: Have: %d, Should Have: %d\n",G.discardCount[player],preG.discardCount[player] + 1 + extras);
		}
		for(j=0;j<G.discardCount[player];j++) {
			if(!asserttrue(G.discard[player][j] != -1,&works,&preG)) {
				printf("Error in discard pile - null result\n");
			} 
		}
		
		//other player had no status change
		if(!asserttrue(G.handCount[player+1] == preG.handCount[player+1],&works,&preG)) {
			printf("Error in other player gamestate: Hand Count\n");
		}
		if(!asserttrue(G.deckCount[player+1] == preG.deckCount[player+1],&works,&preG)) {
			printf("Error in other player gamestate: Deck Count\n");
		}
		//no change in supply cards (victory, treasure, curse, or otherwise)
		for(j=adventurer; j <= treasure_map;j++) {
			if(!asserttrue(G.supplyCount[j] == preG.supplyCount[j],&works,&preG)) {
				printf("Error in supply cards state: Card position %d\n",j);
			}
		}
	
	}
	if(works == 1) {
		printf("NO ERRORS IN TESTING: ALL TESTS PASSED\n");
	}
	return 0;
}
