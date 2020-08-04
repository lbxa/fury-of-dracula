//
// Created by eric on 29/7/20.
//

#ifndef FURY_OF_DRACULA_HUNT_UTILITIES_H
#define FURY_OF_DRACULA_HUNT_UTILITIES_H

#include "GameView.h"
/**
 * Wrapper for checking malloced memory. This helps us avoid re-writing
 * this block code over and over again.
 * @param mem - the pointer to whatever is being malloced.
 * @param errorMessage - Msg to appear when error in evoked.
 *
 */
void CheckMallocSuccess(void *mem, char *errorMessage);

/**
 * Returns the minimum of a and b
 * @param a
 * @param b
 * @return
 */
int min(int a, int b);

/**
 * Returns the maximum of a and b
 * @param a
 * @param b
 * @return
 */
int max(int a, int b);

/**
 * Adds a given move to the past plays string
 * @param state
 * @param pastPlays
 * @param newPlay
 * @param currentPlayer
 * @param turnNumber
 */
char *GetPastPlayStringForMove(GameView state, char *newPlay,
                              Player currentPlayer, int turnNumber);

#endif  // FURY_OF_DRACULA_HUNT_UTILITIES_H
