//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_PLAYERS_H
#define FURY_OF_DRACULA_PLAYERS_H

#include <stdio.h>
#include "Game.h"
#include "Places.h"

typedef struct playerDetails {
    Player player;
    int moveCount;
    bool isDead;
    PlaceId moves[GAME_START_SCORE];
    PlaceId resolvedMoves[GAME_START_SCORE];
    PlaceId lastResolvedLocation;
    int playerHealth;
} *PlayerDetails;

PlayerDetails CreatePlayer(Player player, int initialHealth);

void FreePlayer(PlayerDetails player);

void PrintPlayer(PlayerDetails player);


#endif //FURY_OF_DRACULA_PLAYERS_H
