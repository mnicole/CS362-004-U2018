/*
 * cardtest2.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "smithy"
// Gives you +3 cards

int assertTrue(int input1, int input2) {
    if (input1 == input2) {
        printf("TEST PASSED!\n");
        return 0;
    } else {
        printf("TEST FAILED!\n");
        return 1;
    }
}

int randomGenerator(int min, int max) {
    return min + rand() % (max + 1 - min);
}

int main() {
    // Seed rand
    srand(time(NULL));

    int failures = 0;
    int newCards = 0;
    int discarded = 1;
    int shuffledCards = 0;

    int handpos = 0, choice1 = -1, choice2 = -1, choice3 = -1, bonus = 0;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};

    int numTimesToTest = randomGenerator(1, 10);
    int seed = 0;
    int numPlayers = 2;

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    // ----------- TEST 1: playing smithy gives you +3 cards --------------
    printf("TEST 1: playing smithy gives you +3 cards\n");

    for (int i = 0; i < numTimesToTest; i++) {
        seed = randomGenerator(0, 1000);
        numPlayers = randomGenerator(2, 4);
        // initialize a game state and player cards
        initializeGame(numPlayers, k, seed, &G);

        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

        newCards = 3;
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);

        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
        failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
    }

    // ----------- TEST 2: playing smithy discards card at end --------------
    printf("\nTEST 2: discard played smithy card\n");

    for (int i = 0; i < numTimesToTest; i++) {
        seed = randomGenerator(0, 1000);
        numPlayers = randomGenerator(2, 4);
        // initialize a game state and player cards
        initializeGame(numPlayers, k, seed, &G);
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

        printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);

        failures += assertTrue(testG.playedCardCount, G.playedCardCount + discarded);
    }

    // ----------- TEST 3: playing smithy does not affect other players' hands --------------
    printf("\nTEST 3: no changes to other players' hands\n");

    for (int i = 0; i < numTimesToTest; i++) {
        seed = randomGenerator(0, 1000);
        numPlayers = randomGenerator(2, 4);
        // initialize a game state and player cards
        initializeGame(numPlayers, k, seed, &G);
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

        // check other players' hands
        for (int i = 1; i < numPlayers; i++) {
            printf("player %d: hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i]);
            failures += assertTrue(testG.handCount[i], G.handCount[i]);

            printf("player %d: deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i]);
            failures += assertTrue(testG.deckCount[i], G.deckCount[i]);
        }
    }


    // ----------- TEST 4: playing smithy does not affect victory card piles --------------
    printf("\nTEST 4: playing smithy does not affect victory card piles\n");

    for (int i = 0; i < numTimesToTest; i++) {
        seed = randomGenerator(0, 1000);
        numPlayers = randomGenerator(2, 4);
        // initialize a game state and player cards
        initializeGame(numPlayers, k, seed, &G);
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

        printf("estate count = %d, expected = %d\n", testG.supplyCount[estate], G.supplyCount[estate]);
        failures += assertTrue(testG.supplyCount[estate], G.supplyCount[estate]);

        printf("duchy count = %d, expected = %d\n", testG.supplyCount[duchy], G.supplyCount[duchy]);
        failures += assertTrue(testG.supplyCount[duchy], G.supplyCount[duchy]);

        printf("province count = %d, expected = %d\n", testG.supplyCount[province], G.supplyCount[province]);
        failures += assertTrue(testG.supplyCount[province], G.supplyCount[province]);
    }

    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTCARD, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTCARD);
    }

    return 0;
}
