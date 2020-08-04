////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "hunter.h"

#include <time.h>

#include "Game.h"
#include "HunterView.h"

void HMakeRandomMove(HunterView view) {
  int numMoves = 0;
  PlaceId* possibleMoves = HvWhereCanIGo(view, &numMoves);
  PlaceId move = possibleMoves[rand() % numMoves];
  registerBestPlay((char*)placeIdToAbbrev(move), "132?");
}

void HFirst(Player player) {
  int placeId = 30;
  // Decide on good starting positions for all hunters
  if (player == 0) {
    placeId = SZEGED;
  } else if (player == 1) {
    placeId = LONDON;
  } else if (player == 2) {
    placeId = SARAGOSSA;
  } else {
    placeId = ZURICH;
  }
  registerBestPlay(PLACES[placeId].abbrev, "132");
}

void decideHunterMove(HunterView hv) {
  srand(time(NULL));
  int player = HvGetPlayer(hv);
  if (HvGetRound(hv) == 0) {
    HFirst(player);
  } else {
    Round lastKnownRound = -1;
    PlaceId lastKnown = HvGetLastKnownDraculaLocation(hv, &lastKnownRound);
    if (lastKnown != NOWHERE && HvGetRound(hv) - lastKnownRound <= 3) {
      int pathLength = 0;
      PlaceId* path = HvGetShortestPathTo(hv, player, lastKnown, &pathLength);
      if (pathLength > 0 && placeIsReal(path[0])) {
        registerBestPlay(placeIdToAbbrev(path[0]), "132");
      } else {
        HMakeRandomMove(hv);
      }
    } else {
      HMakeRandomMove(hv);
    }
  }
}
