/*
 * unittest4.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNC "whoseTurn"

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
    int playerTurn;

    int seed = 1000;
    int numPlayers = 5;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Function: %s ----------------\n", TESTFUNC);

    // ----------- TEST 1: game recognizes correct player's turn (1) --------------
    printf("TEST 1: game recognizes correct player's turn\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Set player turn
    testG.whoseTurn = 1;

    playerTurn = whoseTurn(&testG);

    printf("playerTurn = %d, expected = %d\n", playerTurn, 1);
    failures += assertTrue(playerTurn, 1);


    // ----------- TEST 2: game recognizes correct player's turn (4) --------------
    printf("TEST 2: game recognizes correct player's turn (4)\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Set player turn
    testG.whoseTurn = 4;

    playerTurn = whoseTurn(&testG);

    printf("playerTurn = %d, expected = %d\n", playerTurn, 4);
    failures += assertTrue(playerTurn, 4);


    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTFUNC, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTFUNC);
    }

    return 0;
}
