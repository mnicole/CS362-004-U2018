/*
 * cardtest4.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "council_room"
// Gives you +4 cards
// +1 Buy
// Each other player draws a card
// put played card in played card pile

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


    // ----------- TEST 1: playing council_room gives you +4 cards --------------
    printf("TEST 1: playing council_room gives you +4 cards\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

    newCards = 4;
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);
    failures += assertTrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards);

    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
    failures += assertTrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);


    // ----------- TEST 2: playing council_room discards played card at end --------------
    printf("\nTEST 2: discard played council_room card\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

    printf("played card count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + discarded);

    failures += assertTrue(testG.playedCardCount, G.playedCardCount + discarded);


    // ----------- TEST 3: playing council_room gives you +1 buy --------------
    printf("\nTEST 3: playing council_room gives you +1 buy\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

    printf("num buys = %d, expected = %d\n", testG.numBuys, G.numBuys + 1);
    failures += assertTrue(testG.numBuys, G.numBuys + 1);


    // ----------- TEST 4: playing council_room adds a card to other players' hands --------------
    printf("\nTEST 4: playing council_room adds a card to other players' hands\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

    // check other players' hands
    for (int i = 1; i < numPlayers; i++) {
        printf("player %d: hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i] + 1);
        failures += assertTrue(testG.handCount[i], G.handCount[i] + 1);

        printf("player %d: deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i] - 1);
        failures += assertTrue(testG.deckCount[i], G.deckCount[i] - 1);
    }


    // ----------- TEST 5: playing council_room does not affect victory card piles --------------
    printf("\nTEST 5: playing council_room does not affect victory card piles\n");

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

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
