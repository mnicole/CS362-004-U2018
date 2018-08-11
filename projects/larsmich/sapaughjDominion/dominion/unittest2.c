/*
 * unittest2.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNC "isGameOver"

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
    int gameOver;

    int seed = 1000;
    int numPlayers = 2;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Function: %s ----------------\n", TESTFUNC);

    // ----------- TEST 1: game ends if no province cards left --------------
    printf("TEST 1: game ends if no province cards left\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Set province cards to 0
    testG.supplyCount[province] = 0;

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 1);
    failures += assertTrue(gameOver, 1);

    // ----------- TEST 2: game does not end if there are province cards left --------------
    printf("\nTEST 2: game does not end if there are province cards left\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Set province cards to 10 (num > 0)
    testG.supplyCount[province] = 10;

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 0);
    failures += assertTrue(gameOver, 0);

    // ----------- TEST 3: game ends if 3 supply cards are at 0 --------------
    printf("\nTEST 3: game ends if 3 supply cards are at 0\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    for (i = 0; i < 25; i++) {
        testG.supplyCount[i] = 1;
    }

    testG.supplyCount[1] = 0;
    testG.supplyCount[6] = 0;
    testG.supplyCount[19] = 0;

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 1);
    failures += assertTrue(gameOver, 1);

    // ----------- TEST 4: game doesn't ends if supply cards are all != 0 --------------
    printf("\nTEST 4: game doesn't ends if supply cards are all != 0\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    for (i = 0; i < 25; i++) {
        testG.supplyCount[i] = 1;
    }

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 0);
    failures += assertTrue(gameOver, 0);

    // ----------- TEST 5: game doesn't ends if 1 supplyCard = 0 --------------
    printf("\nTEST 5: game doesn't ends if 1 supplyCard = 0\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    for (i = 1; i < 24; i++) {
        testG.supplyCount[i] = 1;
    }

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 0);
    failures += assertTrue(gameOver, 0);

    // ----------- TEST 6: game doesn't ends if 2 supply cards = 0 --------------
    printf("\nTEST 6: game doesn't ends if 2 supply cards = 0\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    for (i = 0; i < 23; i++) {
        testG.supplyCount[i] = 1;
    }

    gameOver = isGameOver(&testG);

    printf("gameOver = %d, expected = %d\n", gameOver, 0);
    failures += assertTrue(gameOver, 0);

    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTFUNC, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTFUNC);
    }

    return 0;
}
