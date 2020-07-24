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
    PlaceId moves[GAME_START_SCORE];
    PlaceId resolvedMoves[GAME_START_SCORE];
    PlaceId lastResolvedLocation;
    int playerHealth;
} *PlayerDetails;

PlayerDetails CreatePlayer(Player player, int initialHealth) {
    PlayerDetails playerDetails = malloc(sizeof(*playerDetails));
    if (playerDetails == NULL) {
        fprintf(stderr, "Couldn't allocate PlayerDetails!\n");
        exit(EXIT_FAILURE);
    }
    playerDetails->moveCount = 0;
    playerDetails->playerHealth = initialHealth;
    playerDetails->player = player;
    playerDetails->lastResolvedLocation = NOWHERE;
    return playerDetails;
}

void FreePlayer(PlayerDetails player) {
    free(player);
}

void PrintPlayer(PlayerDetails player) {
    char* names[NUM_PLAYERS];
    names[PLAYER_LORD_GODALMING] = "PLAYER_LORD_GODALMING";
    names[PLAYER_DR_SEWARD] = "PLAYER_DR_SEWARD";
    names[PLAYER_MINA_HARKER] = "PLAYER_MINA_HARKER";
    names[PLAYER_VAN_HELSING] = "PLAYER_VAN_HELSING";
    names[PLAYER_DRACULA] = "PLAYER_DRACULA";

    printf("Player: %s\n", names[player->player]);
    printf("\t- Health: %d\n", player->playerHealth);
    printf("\t- Last Location: %s\n", placeIdToName(player->lastResolvedLocation));
}


#endif //FURY_OF_DRACULA_PLAYERS_H
