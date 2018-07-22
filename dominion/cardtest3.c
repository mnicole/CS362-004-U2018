/*
 * cardtest3.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"
// Look for treasure cards until you reveal 2. Put those in your hand and discard other revealed cards.

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
    int numToDiscard = 0;
    int discarded = 1;

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

    // ----------- TEST 1: playing adventurer gives you +2 treasure cards when the first 2 cards are treasure cards --------------
    printf("TEST 1: playing adventurer gives you +2 treasure cards when the first 2 cards are treasure cards\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
    newCards = 2;

    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);


    // ----------- TEST 2: playing adventurer gives you +2 treasure cards when the first 2 cards are not treasure cards --------------
    printf("\nTEST 2: playing adventurer gives you +2 treasure cards when the first 2 cards are not treasure cards\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    shuffle(thisPlayer, &testG);
    shuffle(thisPlayer, &testG);

    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

    newCards = 2;

    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);


    // ----------- TEST 3: playing adventurer discards the correct cards when the first 2 cards are not treasure cards --------------
    printf("\nTEST 3: playing adventurer discards the correct cards when the first 2 cards are not treasure cards\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    shuffle(thisPlayer, &testG);
    shuffle(thisPlayer, &testG);
    shuffle(thisPlayer, &testG);
    shuffle(thisPlayer, &testG);

    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

    numToDiscard = 2;

    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numToDiscard);
    failures += assertTrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numToDiscard);


    // ----------- TEST 4: playing adventurer discards card at end --------------
    printf("\nTEST 4: discard played adventurer card\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

    printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);

    failures += assertTrue(testG.playedCardCount, G.playedCardCount + discarded);


    // ----------- SUMMARY --------------
    if (failures > 0) {
        printf("\n >>>>> Testing %s complete, %d asserts failed <<<<<\n\n", TESTCARD, failures);
    } else {
        printf("\n >>>>> SUCCESS: Testing %s complete <<<<<\n\n", TESTCARD);
    }

    return 0;
}
