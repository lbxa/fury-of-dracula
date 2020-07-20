//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_PLAYERS_H
#define FURY_OF_DRACULA_PLAYERS_H

#include "Game.h"
#include "Places.h"

typedef struct playerDetails {
    Player player;
    int locationHistoryCount;
    Place *locationHistory;
    int playerHealth;
} *PlayerDetails;

PlayerDetails CreatePlayer(Player player, int initialHealth) {
    PlayerDetails playerDetails = malloc(sizeof(*playerDetails));
    if (playerDetails == NULL) {
        fprintf(stderr, "Couldn't allocate PlayerDetails!\n");
        exit(EXIT_FAILURE);
    }
    playerDetails->locationHistoryCount = 0;
    playerDetails->locationHistory = NULL;
    playerDetails->playerHealth = initialHealth;
    playerDetails->player = player;
    return playerDetails;
}


#endif //FURY_OF_DRACULA_PLAYERS_H
