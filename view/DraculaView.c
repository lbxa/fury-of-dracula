////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "path_finding.h"
// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct draculaView {
    GameView gameView;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[]) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DraculaView new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Couldn't allocate DraculaView\n");
        exit(EXIT_FAILURE);
    }

    GameView gameView = GvNew(pastPlays, messages);
    new->gameView = gameView;

    return new;
}

void DvFree(DraculaView dv) {
    GvFree(dv->gameView);
    free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv) {
    return GvGetRound(dv->gameView);
}

int DvGetScore(DraculaView dv) {
    return GvGetScore(dv->gameView);
}

int DvGetHealth(DraculaView dv, Player player) {
    return GvGetHealth(dv->gameView, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player) {
    return GvGetPlayerLocation(dv->gameView, player);
}

PlaceId DvGetVampireLocation(DraculaView dv) {
    return GvGetVampireLocation(dv->gameView);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numTraps = 0;
    return GvGetTrapLocations(dv->gameView, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves) {

}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs) {
    *numReturnedLocs = 0;
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
    return GvGetReachable(dv->gameView, PLAYER_DRACULA, -1, currentLocation, numReturnedLocs);
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs) {
    *numReturnedLocs = 0;
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
    return GvGetReachableByType(dv->gameView, PLAYER_DRACULA, -1, currentLocation, road, false, boat, numReturnedLocs);
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs) {
    *numReturnedLocs = 0;
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
    Round round = GvGetRound(dv->gameView);
    return GvGetReachable(dv->gameView, player, round, currentLocation, numReturnedLocs);
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedLocs = 0;
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
    Round round = GvGetRound(dv->gameView);
    return GvGetReachableByType(dv->gameView, player, round, currentLocation, road, rail, boat, numReturnedLocs);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
