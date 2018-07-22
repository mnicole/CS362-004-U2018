/*
 * cardtest1.c
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "village"
// Gives you +1 card and +2 actions
// Add card to played card (discard)

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
    int newCards = 0;
    int newActions = 0;
    int discarded = 1;
    int shuffledCards = 0;

    int handpos = 0, choice1 = -1, choice2 = -1, choice3 = -1, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, remodel,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    // ----------- TEST 1: playing village gives you +1 card --------------
    printf("TEST 1: playing village gives you +1 card\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

    newCards = 1;
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);

    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
    failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);


    // ----------- TEST 2: playing village gives you +2 actions --------------
    printf("\nTEST 2: playing village gives you +2 actions\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

    newActions = 2;
    printf("actions = %d, expected = %d\n", testG.numActions, G.numActions + newActions);
    failures += assertTrue(testG.numActions, G.numActions + newActions);


    // ----------- TEST 3: playing village discards card at end --------------
    printf("\nTEST 3: discard played village card\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

    printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);
    failures += assertTrue(testG.playedCardCount, G.playedCardCount + discarded);


    // ----------- TEST 4: playing village does not affect other players' hands --------------
    printf("\nTEST 4: no changes to other players' hands\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

    // check other players' hands
    for (int i = 1; i < numPlayers; i++) {
        printf("player %d: hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i]);
        failures += assertTrue(testG.handCount[i], G.handCount[i]);

        printf("player %d: deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i]);
        failures += assertTrue(testG.deckCount[i], G.deckCount[i]);
    }


    // ----------- TEST 5: playing village does not affect victory card piles --------------
    printf("\nTEST 5: playing village does not affect victory card piles\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

    printf("estate count = %d, expected = %d\n", testG.supplyCount[estate], G.supplyCount[estate]);
    failures += assertTrue(testG.supplyCount[estate], G.supplyCount[estate]);

    printf("duchy count = %d, expected = %d\n", testG.supplyCount[duchy], G.supplyCount[duchy]);
    failures += assertTrue(testG.supplyCount[duchy], G.supplyCount[duchy]);

    printf("province count = %d, expected = %d\n", testG.supplyCount[province], G.supplyCount[province]);
    failures += assertTrue(testG.supplyCount[province], G.supplyCount[province]);

    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTCARD, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTCARD);
    }

    return 0;
}
