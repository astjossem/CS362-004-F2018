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

//unit test 3 which tests the fullDeckCount card function
int main() {
	int i, j, l, m,count,counter,difference =0,choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,player = 0,works=1;
	struct gameState G, preG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	printf("\n\n****Testing Full Deck Count****\n\n");
	//test function with 0-9 cards in hand
	for(i = 0; i < 10; i++) {
		//test draw with 0-9 cards in deck
		for(j=0; j < 10;j++) {
			//test with 0-9 cards in discard pile
			for(m=0;m<9;m++) {
				counter = 0;
				initializeGame(2,k,10,&G);
				//set hand size
				G.handCount[player] = i;
				//remove initialized cards in hand that are not used
				//for(l=0;l<5;l++) {
				//	G.hand[player][l] = -1;
				//}
				//fill hand to hand size
				for(l=0;l<i;l++) {
					G.hand[player][l] = 1;
				}
				//remove initialized cards in deck
				//for(l=0;l<5;l++) {
				//	G.deck[player][l] = -1;
				//}
				//adjust deck cards
				G.deckCount[player] = j;	
				for(l=0;l<j;l++) {
					G.deck[player][l] = 1;
				}
				//fill discard pile to correct size
				G.discardCount[player] = m;
				//G.discard[player][j] = -1;
				//create previously discarded cards
				for(l=0;l<m;l++) {
					G.discard[player][l] = 1;
				}
				memcpy(&preG,&G,sizeof(struct gameState));
				count = fullDeckCount(player,1,&G);

				//compare total count - all the same card
				if(!asserttrue(count == i+j+m, &works)) {
					printf("Basic Count Error: Counted %d Should Have %d\n",count,i+j+m);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d \n\n",i,j,m);
				}
				//compare total count - none of card selected
				count = fullDeckCount(player,2,&G);
				if(!asserttrue(count == 0, &works)) {
					printf("Count 0 Error: Counted: %d Should have %d\n",count,i+j+m);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);	
				}
				//add 1  card to discard - if discard is not empty
				if(G.discardCount[player] != 0) {
					counter++;
					G.discard[player][0] = 2;
					//printf("Added 1st discard\n");
					// add second card to 3rd position
					if(m >= 3) {
						counter++;
						G.discard[player][2] = 2;
						//printf("Added 2nd discard\n");
					}
					count = fullDeckCount(player,2,&G);
					if(!asserttrue(count == counter,&works)) {
						printf("Count Error-Discard: Counted: %d Should have %d\n",count,counter);
						printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
					}
				}
				//add i card to deck - if deck is not empty
				if(G.deckCount[player] != 0) {
					counter++;
					G.deck[player][0] = 2;
					//printf("Added 1st deck\n");
					//add second card to 4th position
					if(j>=4) {
						counter++;
						G.deck[player][3] = 2;
						//printf("Added 2nd deck\n");
					}
					count = fullDeckCount(player,2,&G);
					if(!asserttrue(count == counter, &works)) {
						printf("Count Error-Deck: Counted %d Should have %d\n",count,counter);
						printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
					}
				}
				//add 1 card to hand - if hand not empty
				if(G.handCount[player] != 0) { 
					G.hand[player][0] = 2;
					//printf("Added 1st hand\n");
					counter++;
					//add second card to 5th position
					if(i>=5) {
						counter++;
						//printf("Added 2nd hand\n");
						G.hand[player][4] = 2;
					}
					count = fullDeckCount(player,2,&G);
					if(!asserttrue(count == counter,&works)) {
						printf("Count Error-Hand: Counted %d Should have %d\n",count,counter);
						printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
					}
				}
				//# cards in hand doesn't change
				if(!asserttrue(G.handCount[player] == preG.handCount[player],&works)) {
					printf("Incorect number of cards in hand: Has %d Should have: %d\n",G.handCount[player],preG.handCount[player]);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
				}
				//#cards in deck doesn't change
				if(!asserttrue(G.deckCount[player] == preG.deckCount[player],&works)) {
					printf("Incorect number of cards in deck: Has %d Should have: %d\n",G.deckCount[player],preG.deckCount[player]);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
				}
				//#cards in discard doesn't change
				if(!asserttrue(G.discardCount[player] == preG.discardCount[player],&works)) {
					printf("Incorect number of cards in discard: Has %d Should have: %d\n",G.discardCount[player],preG.discardCount[player]);
					printf("iteration: #HandSize: %d #DeckSize: %d #DiscardSize: %d\n\n",i,j,m);
				}
			}
		}
	}
	if(works == 1) {
		printf("NO ERRORS IN TESTING: ALL TESTS PASSED\n");
	}
	return 0;
}
