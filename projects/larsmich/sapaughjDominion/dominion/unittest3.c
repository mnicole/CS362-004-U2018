/*
 * unittest3.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNC "gainCard"

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
    int toFlag = 0;
    int gainCardRes = 0;

    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};
    int supplyPos = k[0];


    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Function: %s ----------------\n", TESTFUNC);


    // ----------- TEST 1: does not gain card if supplyPile is empty or card not used in game --------------
    printf("TEST 1: game ends if no province cards left\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Set supplyPos = 0
    testG.supplyCount[supplyPos] = 0;

    gainCardRes = gainCard(supplyPos, &testG, toFlag, thisPlayer);

    printf("gainCard Result = %d, expected = %d\n", gainCardRes, -1);
    failures += assertTrue(gainCardRes, -1);


    // ----------- TEST 2: gain card in discard if toFlag = 0 --------------
    printf("\nTEST 2: gain card in discard if toFlag = 0\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    toFlag = 0;

    gainCardRes = gainCard(supplyPos, &testG, toFlag, thisPlayer);

    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + 1);
    failures += assertTrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + 1);

    printf("discard card = %d, expected = %d\n", testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 1], supplyPos);
    failures += assertTrue(testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 1], supplyPos);

    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer]);

    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

    printf("gainCard Result = %d, expected = %d\n", gainCardRes, 0);


    // ----------- TEST 3: gain card in deck if toFlag = 1 --------------
    printf("\nTEST 3: gain card in deck if toFlag = 1\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    toFlag = 1;

    gainCardRes = gainCard(supplyPos, &testG, toFlag, thisPlayer);

    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] + 1);
    failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] + 1);

    printf("deck card = %d, expected = %d\n", testG.deck[thisPlayer][testG.deckCount[thisPlayer] - 1], supplyPos);
    failures += assertTrue(testG.deck[thisPlayer][testG.deckCount[thisPlayer] - 1], supplyPos);

    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
    failures += assertTrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);

    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer]);

    printf("gainCard Result = %d, expected = %d\n", gainCardRes, 0);


    // ----------- TEST 4: gain card in hand if toFlag = 2 --------------
    printf("\nTEST 4: gain card in hand if toFlag = 2\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    toFlag = 2;

    gainCardRes = gainCard(supplyPos, &testG, toFlag, thisPlayer);

    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);

    printf("hand card = %d, expected = %d\n", testG.hand[thisPlayer][testG.handCount[thisPlayer] - 1], supplyPos);
    failures += assertTrue(testG.hand[thisPlayer][testG.handCount[thisPlayer] - 1], supplyPos);

    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
    failures += assertTrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);

    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

    printf("gainCard Result = %d, expected = %d\n", gainCardRes, 0);


    // ----------- TEST 5: decrease supply pile --------------
    printf("\nTEST 5: decrease supply pile\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    toFlag = 0;

    gainCardRes = gainCard(supplyPos, &testG, toFlag, thisPlayer);

    printf("supply count = %d, expected = %d\n", testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1);
    failures += assertTrue(testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1);

    printf("gainCard Result = %d, expected = %d\n", gainCardRes, 0);


    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTFUNC, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTFUNC);
    }

    return 0;
}
