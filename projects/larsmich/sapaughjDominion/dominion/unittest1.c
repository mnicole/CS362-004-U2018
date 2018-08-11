/*
 * unittest1.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNC "updateCoins"

int assertTrue(int input1, int input2) {
    if (input1 == input2) {
        printf("TEST PASSED!\n");
        return 0;
    } else {
        printf("TEST FAILED!\n");
        return 1;
    }
}

int main() {
    int failures = 0;
    int i;
    int totalCoins;

    int bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Function: %s ----------------\n", TESTFUNC);

    // ----------- TEST 1: calculates correct coins based on player's hand --------------
    printf("TEST 1: calculates correct coins based on player's hand\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Find the coins in the hand
    totalCoins = 0;
    for (i = 0; i < testG.handCount[thisPlayer]; i++) {
        if (testG.hand[thisPlayer][i] == copper) {
            totalCoins += 1;
        } else if (testG.hand[thisPlayer][i] == silver) {
           totalCoins += 2;
        } else if (testG.hand[thisPlayer][i] == gold) {
           totalCoins += 3;
        }
    }

    // Reset coins to start from same base
    testG.coins = 0;
    G.coins = 0;

    updateCoins(thisPlayer, &testG, bonus);

    printf("coins = %d, expected = %d\n", testG.coins, G.coins + totalCoins + bonus);
    failures += assertTrue(testG.coins, G.coins + totalCoins + bonus);


    // ----------- TEST 2: adds appropriate bonus to coins --------------
    printf("\nTEST 2: adds appropriate bonus to coins\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Find the coins in the hand
    totalCoins = 0;
    for (i = 0; i < testG.handCount[thisPlayer]; i++) {
        if (testG.hand[thisPlayer][i] == copper) {
            totalCoins += 1;
        } else if (testG.hand[thisPlayer][i] == silver) {
           totalCoins += 2;
        } else if (testG.hand[thisPlayer][i] == gold) {
           totalCoins += 3;
        }
    }

    bonus = 5;

    // Reset coins to start from same base
    testG.coins = 0;
    G.coins = 0;

    updateCoins(thisPlayer, &testG, bonus);

    printf("coins = %d, expected = %d\n", testG.coins, G.coins + totalCoins + bonus);
    failures += assertTrue(testG.coins, G.coins + totalCoins + bonus);


    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTFUNC, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTFUNC);
    }

    return 0;
}
