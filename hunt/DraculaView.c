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

#include "DraculaView.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "PathFinding.h"
#include "Utilities.h"

// Code written by:
// Eric | Lucas | Stephen | Debbie - (20T2)

/**
 * Dracula view struct only contains GameView as everything required is
 * there. This singular approach reduced the amount of code that needed
 * to be copied between HunterView and DraculaView.
 */
struct draculaView {
  GameView gameView;
};

// Code written by:
// Eric | Lucas | Stephen | Debbie - (20T2)

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[]) {
  DraculaView new = malloc(sizeof(*new));
  CheckMallocSuccess(new, "Couldn't allocate DraculaView\n");

  // Creates new GameView and puts it in DraculaView struct
  GameView gameView = GvNew(pastPlays, messages);
  new->gameView = gameView;

  return new;
}

/**
 * Frees all memory allocated for `dv`.
 * @param dv
 */
void DvFree(DraculaView dv) {
  GvFree(dv->gameView);
  free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv) {
  assert(dv != NULL);
  return GvGetRound(dv->gameView);
}

int DvGetScore(DraculaView dv) {
  assert(dv != NULL);
  return GvGetScore(dv->gameView);
}

int DvGetHealth(DraculaView dv, Player player) {
  assert(dv != NULL);
  return GvGetHealth(dv->gameView, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player) {
  assert(dv != NULL);
  return GvGetPlayerLocation(dv->gameView, player);
}

PlaceId DvGetVampireLocation(DraculaView dv) {
  assert(dv != NULL);
  return GvGetVampireLocation(dv->gameView);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps) {
  assert(dv != NULL);
  *numTraps = 0;
  return GvGetTrapLocations(dv->gameView, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves) {
  *numReturnedMoves = 0;
  Map map = GetMap(dv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
  // Gets valid moves for dracula applying his movement restrictions
  return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                          true, false, true, 0, numReturnedMoves, false, true);
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs) {
  *numReturnedLocs = 0;
  Map map = GetMap(dv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
  // Resolves valid moves for dracula into locations and applying his movement
  // restrictions
  return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                          true, false, true, 0, numReturnedLocs, true, true);
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs) {
  *numReturnedLocs = 0;
  Map map = GetMap(dv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, PLAYER_DRACULA);
  // Resolves valid moves for dracula into locations and applying his movement
  // restrictions as well as doing type restrictions
  return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                          road, false, boat, 0, numReturnedLocs, true, true);
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player, int *numReturnedLocs) {
  *numReturnedLocs = 0;
  Map map = GetMap(dv->gameView);
  int round = GvGetRound(dv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
  // Resolves valid moves for given players into locations and applying relevant
  // movement restrictions
  return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                          true, true, true, round, numReturnedLocs, true, true);
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player, bool road,
                                bool rail, bool boat, int *numReturnedLocs) {
  *numReturnedLocs = 0;
  Map map = GetMap(dv->gameView);
  int round = GvGetRound(dv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(dv->gameView, player);
  // Resolves valid moves for given players into locations and applying relevant
  // movement restrictions as well as doing type restrictions
  return GetPossibleMoves(dv->gameView, map, PLAYER_DRACULA, currentLocation,
                          road, rail, boat, round, numReturnedLocs, true, true);
}

////////////////////////////////////////////////////////////////////////

GameView DvGetGameView(DraculaView view) {
  return view->gameView;
}