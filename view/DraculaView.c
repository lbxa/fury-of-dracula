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
#include "PathFinding.h"



struct draculaView {
    GameView gameView;
};

// Code written by:
// Eric | Lucas | Stephen | Debbie - (20T2)

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor


DraculaView DvNew(char *pastPlays, Message messages[]) {

    DraculaView new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Couldn't allocate DraculaView\n");
        exit(EXIT_FAILURE);
    }

    GameView gameView = GvNew(pastPlays, messages);
    new->gameView = gameView;

    return new;
}

// Frees all memory allocated for `dv`.
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
    *numTraps = 0;
    return GvGetTrapLocations(dv->gameView, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves) {
    *numReturnedMoves = 0;
    Map map = GetMap(dv->gameView);
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
    return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
            true, false, true, 0, numReturnedMoves, false, true);
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs) {
    *numReturnedLocs = 0;
    Map map = GetMap(dv->gameView);
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
    return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                            true, false, true, 0, numReturnedLocs, false, true);
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs) {
    *numReturnedLocs = 0;
    Map map = GetMap(dv->gameView);
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
    return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                            road, false, boat, 0, numReturnedLocs, true, true);
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs) {
    *numReturnedLocs = 0;
    Map map = GetMap(dv->gameView);
    int round = GvGetRound(dv->gameView);
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
    return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                            true, true, true, round, numReturnedLocs, true, true);
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs) {
    *numReturnedLocs = 0;
    Map map = GetMap(dv->gameView);
    int round = GvGetRound(dv->gameView);
    PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
    return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                            road, rail, boat, round, numReturnedLocs, true, true);
}


////////////////////////////////////////////////////////////////////////
