#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

//New functions after refactoring
void playAdventurer(struct gameState *state, int drawntreasure, int z, int currentPlayer, int cardDrawn, int *temphand);
void playCouncilRoom(struct gameState *state, int handPos, int currentPlayer);
void playFeast(struct gameState *state, int choice1, int *temphand, int currentPlayer);
void playSmithy(struct gameState *state, int handPos, int currentPlayer);
void playSteward(struct gameState *state, int choice1, int choice2, int choice3, int handPos, int currentPlayer);

#endif
