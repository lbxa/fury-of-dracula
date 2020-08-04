////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "dracula.h"

#include <assert.h>
#include <time.h>

#include "DraculaView.h"
#include "Game.h"

PlaceId resolveDraculaMove(DraculaView view, PlaceId move) {
  int round = DvGetRound(view);
  if (placeIsReal(move) || move == CITY_UNKNOWN || move == SEA_UNKNOWN) {
    return move;
  }
  int locationCount = 0;
  bool canFree;
  PlaceId *draculaLocationHistory = GvGetLocationHistory(
      DvGetGameView(view), PLAYER_DRACULA, &locationCount, &canFree);

  switch (move) {
    case TELEPORT:
      return CASTLE_DRACULA;
    case HIDE:
      return draculaLocationHistory[round - 1];
    case DOUBLE_BACK_1:
      return draculaLocationHistory[round - 1];
    case DOUBLE_BACK_2:
      return draculaLocationHistory[round - 2];
    case DOUBLE_BACK_3:
      return draculaLocationHistory[round - 3];
    case DOUBLE_BACK_4:
      return draculaLocationHistory[round - 4];
    case DOUBLE_BACK_5:
      return draculaLocationHistory[round - 5];
    default:
      assert(0);  // impossible
  }
}

void DFirst(DraculaView dv) {
  registerBestPlay(PLACES[VENICE].abbrev, "132");
}

void DSimpleBestMove(DraculaView dv) {
  int numMoves = 0;
  PlaceId *moves = DvGetValidMoves(dv, &numMoves);
  for (int i = 0; i < numMoves; ++i) {
    PlaceId resolved = resolveDraculaMove(dv, moves[i]);
    if (!placeIsSea(resolved) || i == numMoves - 1) {
      registerBestPlay(placeIdToAbbrev(moves[i]), "132");
    }
  }
  if (numMoves == 0) {
    registerBestPlay(placeIdToAbbrev(TELEPORT), "132");
  }
}

void decideDraculaMove(DraculaView dv) {
  srand(time(NULL));
  if (DvGetRound(dv) == 0) {
    DFirst(dv);
  } else {
    DSimpleBestMove(dv);
  }
}
