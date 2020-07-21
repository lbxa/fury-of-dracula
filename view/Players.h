//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_PLAYERS_H
#define FURY_OF_DRACULA_PLAYERS_H

#include "Game.h"
#include "Places.h"

typedef struct playerDetails {
    Player player;
    int moveCount;
    PlaceId *moves;
    PlaceId *resolvedMoves;
    int playerHealth;
} *PlayerDetails;

PlayerDetails CreatePlayer(Player player, int initialHealth) {
    PlayerDetails playerDetails = malloc(sizeof(*playerDetails));
    if (playerDetails == NULL) {
        fprintf(stderr, "Couldn't allocate PlayerDetails!\n");
        exit(EXIT_FAILURE);
    }
    playerDetails->moveCount = 0;
    playerDetails->moves = NULL;
    playerDetails->resolvedMoves = NULL;
    playerDetails->playerHealth = initialHealth;
    playerDetails->player = player;
    return playerDetails;
}

void FreePlayer(PlayerDetails player) {
    free(player->moves);
    free(player);
}


#endif //FURY_OF_DRACULA_PLAYERS_H
