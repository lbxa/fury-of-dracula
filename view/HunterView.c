////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "HunterView.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HashTable.h"
#include "Map.h"
#include "PathFinding.h"
#include "Places.h"
#include "Utilities.h"

/**
 * Dracula view struct only contains GameView as everything required is
 * there
 */
struct hunterView {
  GameView gameView;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[]) {
  // Allocate memory for hunterView
  HunterView new = malloc(sizeof(*new));
  if (new == NULL) {
    fprintf(stderr, "Couldn't allocate HunterView!\n");
    exit(EXIT_FAILURE);
  }
  // Construct the gameView
  GameView gameView = GvNew(pastPlays, messages);
  new->gameView = gameView;

  // TODO: Add helper function to copy messages into messageList

  return new;
}

/**
 * Frees all memory allocated for `dv`.
 * @param hv
 */
void HvFree(HunterView hv) {
  // Free the gameView and then the rest
  GvFree(hv->gameView);
  free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv) { return GvGetRound(hv->gameView); }

Player HvGetPlayer(HunterView hv) { return GvGetPlayer(hv->gameView); }

int HvGetScore(HunterView hv) { return GvGetScore(hv->gameView); }

int HvGetHealth(HunterView hv, Player player) {
  return GvGetHealth(hv->gameView, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player) {
  return GvGetPlayerLocation(hv->gameView, player);
}

PlaceId HvGetVampireLocation(HunterView hv) {
  return GvGetVampireLocation(hv->gameView);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round) {
  // Initialise variable
  int numReturnedMoves = 0;
  bool canModify = true;
  // Get Dracula's full move history
  PlaceId *draculaHistory = GvGetMoveHistory(hv->gameView, PLAYER_DRACULA,
                                             &numReturnedMoves, &canModify);
  // Loop through his moves, starting at most recent
  // Return any that is a real location
  for (int i = numReturnedMoves - 1; i >= 0; i--) {
    PlaceId currentMove = draculaHistory[i];
    if (currentMove >= MIN_REAL_PLACE && currentMove <= MAX_REAL_PLACE) {
      *round = i;
      PlaceId location = draculaHistory[i];
      free(draculaHistory);
      return location;
    }
  }
  free(draculaHistory);
  // Return if no real moves discovered from his history
  *round = 0;
  return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength) {
  // Getting map
  Map map = GetMap(hv->gameView);
  // Getting data for Place
  *pathLength = 0;
  PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, hunter);
  // Return nothing for start == dest
  if (currentLocation == dest) return NULL;
  // Allocate memory for place
  Place *from = malloc(sizeof(struct place));
  CheckMallocSuccess(from, "Couldn't allocate Place!\n");

  int round = GvGetRound(hv->gameView);

  // Create a lookup table for all paths from current location
  HashTable pathLookup =
      GetPathLookupTableFrom(hv->gameView, map, hunter, PLACES[currentLocation],
                             true, true, true, round, true, false);

  // Get the path to the destination
  Path path = (Path)HashGet(pathLookup, placeIdToAbbrev(dest))->value;

  // Allocate memory for the PlaceId *array that will be returned
  // Loop through the HashNode to get the path and use charAbbrevToId to convert
  // char *key into PlaceId Update length of the array with each node added to
  // the path and return the full array

  *pathLength = path->distance;
  PlaceId *orderedPath = GetOrderedPlaceIds(path);

  // Free memory
  free(from);
  HashTableDestroy(pathLookup, FreePathNode);

  return orderedPath;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs) {
  // Initialise variables to call get possible moves
  *numReturnedLocs = 0;
  Map map = GetMap(hv->gameView);
  Player currentPlayer = GvGetPlayer(hv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, currentPlayer);
  int currentRound = GvGetRound(hv->gameView);

  // Finds legal moves from a given place, see PathFinding.c
  return GetPossibleMoves(hv->gameView, map, currentPlayer, currentLocation,
                          true, true, true, currentRound, numReturnedLocs, true,
                          false);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail, bool boat,
                             int *numReturnedLocs) {
  // Initialise variables to call get possible moves
  *numReturnedLocs = 0;
  Map map = GetMap(hv->gameView);
  Player currentPlayer = GvGetPlayer(hv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, currentPlayer);
  int currentRound = GvGetRound(hv->gameView);

  // Finds legal moves from a given place, see PathFinding.c
  return GetPossibleMoves(hv->gameView, map, currentPlayer, currentLocation,
                          road, rail, boat, currentRound, numReturnedLocs, true,
                          false);
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player, int *numReturnedLocs) {
  // Initialise variables to call get possible moves
  *numReturnedLocs = 0;
  Map map = GetMap(hv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, player);
  int currentRound = GvGetRound(hv->gameView);

  // Finds legal moves from a given place, see PathFinding.c
  return GetPossibleMoves(hv->gameView, map, player, currentLocation, true,
                          true, true, currentRound, numReturnedLocs, true,
                          false);
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player, bool road,
                                bool rail, bool boat, int *numReturnedLocs) {
  // Initialise variables to call get possible moves
  *numReturnedLocs = 0;
  bool canFree = true;
  int numReturnedMoves;
  Map map = GetMap(hv->gameView);
  PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, player);

  // Current round is more complicated -> must check if the player has already
  // made a move this round
  int currentRound = GvGetRound(hv->gameView);
  PlaceId *history =
      GvGetMoveHistory(hv->gameView, player, &numReturnedMoves, &canFree);
  // If player has made a move this turn, check using next turns round number
  // Important for rail moves
  if (history[currentRound] == currentLocation) {
    currentRound += 1;
  }
  free(history);
  // Finds legal moves from a given place, see PathFinding.c
  return GetPossibleMoves(hv->gameView, map, player, currentLocation, road,
                          rail, boat, currentRound, numReturnedLocs, true,
                          false);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
