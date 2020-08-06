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
#include <stdbool.h>

#include "Game.h"
#include "HunterView.h"

#define PLAYER_MSG1 "nah bro, don't think so"

void HMakeRandomMove(HunterView view) {
  int numMoves = 0;
  PlaceId* possibleMoves = HvWhereCanIGo(view, &numMoves);
  PlaceId move = possibleMoves[rand() % numMoves];
  registerBestPlay((char*)placeIdToAbbrev(move), "132?");

  // remove resting & test for health checks
  // if dracula is near hospital area don't bother with rest
  // don't use go backwards unless its the only valid move.
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

  /**
   * (1) Prioritise land locations
   * (2) Predictions what areas he may be in, by looking at last known location,
   * (3) Predictions involving sea moves
   * (4) If known vampire location go to it
   * hunter stay in position as possible move
   * Research turn
   */


  int player = HvGetPlayer(hv);
  PlaceId playerLocation = HvGetPlayerLocation(hv, player);
  int currentRound = HvGetRound(hv);
  if (currentRound == 0) {
    HFirst(player);
  } else if (currentRound == 1) {
    // research move
    registerBestPlay(placeIdToAbbrev(playerLocation), "132");
  } else {
    PlaceId vampireLocation = HvGetVampireLocation(hv);
    
    Round lastKnownRound = -1;
    PlaceId lastKnown = HvGetLastKnownDraculaLocation(hv, &lastKnownRound);

    if (lastKnownRound > 10) {
      // research turn if the last known location was only 10 rounds ago
      registerBestPlay(placeIdToAbbrev(playerLocation), PLAYER_MSG1);
    }    

    if (lastKnown == playerLocation && currentRound - lastKnownRound <= 1) {
      // Stay at current as dracula is there
      registerBestPlay(placeIdToAbbrev(playerLocation), "132");
    } else if (lastKnown != NOWHERE && currentRound - lastKnownRound <= 3) {
      int pathLength = 0;
      PlaceId* path = HvGetShortestPathTo(hv, player, lastKnown, &pathLength);
      PlaceId lastKnownVampire = HvGetVampireLocation(hv);
      
      if (pathLength > 0 && placeIsReal(path[0])) {
        // find out if Dracula went to sea (which he did a lot in the simulations)
        for (int i = 0; i < pathLength; i++) {
          if (placeIsSea(path[i]) == true) {
            // tell the spread the hunters out on the map
            // finish this off... 
          }

          // if there is a vampire -- then go to it!
          if (lastKnownVampire == path[i]) {
            // send hunters toward the location of the vampire.
          }
          
        }

        registerBestPlay(placeIdToAbbrev(path[0]), "132");
      } else {
        HMakeRandomMove(hv);
      }
    } else if (vampireLocation != NOWHERE) {
      int pathLength = 0;
      PlaceId *pathToVampire = HvGetShortestPathTo(hv, player, vampireLocation, &pathLength);
      int roundsToMature = currentRound - HvGetRoundVampirePlaced(hv);
      if (pathLength < roundsToMature && pathLength > 0 && placeIsReal(pathToVampire[0])) {
        registerBestPlay(placeIdToAbbrev(pathToVampire[0]) "132");
      }
    } else {
      HMakeRandomMove(hv);
    }
  }
}