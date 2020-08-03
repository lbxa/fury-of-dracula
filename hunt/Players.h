//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_PLAYERS_H
#define FURY_OF_DRACULA_PLAYERS_H

#include <stdio.h>

#include "Game.h"
#include "Places.h"

/**
 * Player details struct containing fields containing all information regarding
 * a players state in the game
 */
typedef struct playerDetails {
  Player player;
  int moveCount;
  bool isDead;
  PlaceId moves[GAME_START_SCORE];
  PlaceId resolvedMoves[GAME_START_SCORE];
  PlaceId lastResolvedLocation;
  int playerHealth;
} * PlayerDetails;

/**
 * Creates a PlayerDetails struct for the given player enum with the given
 * starting health
 * @param player
 * @param initialHealth
 * @return
 */
PlayerDetails CreatePlayer(Player player, int initialHealth);

/**
 * Frees all memory associated with given PlayerDetails struct
 * @param player
 */
void FreePlayer(PlayerDetails player);

/**
 * Prints a given player to stdout
 * @param player
 */
void PrintPlayer(PlayerDetails player);

#endif  // FURY_OF_DRACULA_PLAYERS_H
