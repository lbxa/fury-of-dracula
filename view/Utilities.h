//
// Created by eric on 29/7/20.
//

#ifndef FURY_OF_DRACULA_HUNT_UTILITIES_H
#define FURY_OF_DRACULA_HUNT_UTILITIES_H

/**
 * Checks if malloc failed to allocate and if it has
 * it exits the program and prints error message
 * @param mem
 * @param errorMessage
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

#endif  // FURY_OF_DRACULA_HUNT_UTILITIES_H
