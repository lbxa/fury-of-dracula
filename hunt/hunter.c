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
#include "GameView.h"
#include "PathFinding.h"

#define PLAYER_MSG1 "nah bro, don't think so"

PlaceId PredictDracula(HunterView view, Player player,
                        PlaceId LastKnown, int roundLastSeen) {
    PlaceId currentLocation = LastKnown;
    PlaceId currentLoc;
    int currentRound = HvGetRound(view);
    int roundsSinceSeen = currentRound - roundLastSeen;
    
    int checkLastKnownPathLength = 0;
    PlaceId *LastKnownPath = HvGetShortestPathToNoRail(view, player,
                            LastKnown, &checkLastKnownPathLength);
    if (roundsSinceSeen < 3 && checkLastKnownPathLength < 2) {
        return LastKnown;
    }
    free(LastKnownPath);
        
    for (int i = 0; i < roundsSinceSeen; i++) {
        GameView state = HvGetGameView(view);
        Map map = GvGetMap(state);
        int numReturnedMoves = 0;
        int maxPathLength = -1;
        printf("1");
        PlaceId *whereCanHeGo = GetPossibleMoves(state, map, PLAYER_DRACULA,
                        currentLocation, true, false, true,  currentRound,
                        &numReturnedMoves, true, false);
        
        for (int j = 0; j < numReturnedMoves; j++) {
            currentLoc = whereCanHeGo[j];
            printf("2");
            int pathLength = 0;
            PlaceId *path = HvGetShortestPathToNoRail(view, player,
                            currentLoc, &pathLength);
            free(path);
            if (pathLength > maxPathLength && placeIsSea(currentLoc) != true) {
                currentLocation =  currentLoc;
                maxPathLength = pathLength;
            }
        }
        
    }


    return currentLocation;
}

void HMakeRandomMove(HunterView view) {
  int numMoves = 0;
  PlaceId* possibleMoves = HvWhereCanIGo(view, &numMoves);
  PlaceId move = possibleMoves[rand() % numMoves];
                                
  int currentRound = HvGetRound(view);
  if (currentRound > 4 && numMoves > 3) {
    GameView gameView = HvGetGameView(view);
    int player = HvGetPlayer(view);
    int numReturnedMoves = 0;
    bool canFree = true;
    PlaceId *moveHistory = GvGetMoveHistory(gameView, player, 
                                &numReturnedMoves, &canFree);
    // removes moves that were done in the last 3 moves, reduces chance for 
    // random move to go backwards
    
    for (int i = 0; i < numMoves - 1; i++) {
        for (int j = 0; j < 3; j++) {
            if (possibleMoves[i] == moveHistory[j]) {
                possibleMoves[i] = NOWHERE;
            }
        }
    }
    // Gets a new random move from only the list of forward moves
    int countForwardMoves = 0;
    PlaceId possibleForwardMoves[numMoves];
    for (int i = 0; i < numMoves; i++) {
        if (possibleMoves[i] >= 0 && 
                possibleMoves[i] < 100) {
            possibleForwardMoves[countForwardMoves] = possibleMoves[i];
            countForwardMoves++;
        }
    }
    
    // Check if near the hospital, if not near the hospital and low on health
    // consider resting as one random move
    int pathLength = 0;
    PlaceId* path = HvGetShortestPathTo(view, player, 
                        HOSPITAL_PLACE, &pathLength);
    free(path);
                        
    if (pathLength > 6 && HvGetHealth(view, player) < 3) {
        PlaceId currentLocation = HvGetPlayerLocation(view, player);
        possibleForwardMoves[countForwardMoves] = currentLocation;
        countForwardMoves++;
    } 
    
    
    move = possibleForwardMoves[rand() % countForwardMoves];
  }
  
  
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
    Round lastKnownRound = -1;
    PlaceId lastKnown = HvGetLastKnownDraculaLocation(hv, &lastKnownRound);

    if (currentRound - lastKnownRound > 6) {
      // research turn if the last known location was only 10 rounds ago
      registerBestPlay(placeIdToAbbrev(playerLocation), PLAYER_MSG1);
    }    

    if (lastKnown == playerLocation && currentRound - lastKnownRound <= 1) {
      // Stay at current as dracula is there
      registerBestPlay(placeIdToAbbrev(playerLocation), "132");
    } else if (lastKnown != NOWHERE && currentRound - lastKnownRound <= 5) {
      printf("1");
      int pathLength = 0;
      printf("1");
      PlaceId predictedLocation = PredictDracula(hv, player,
                        lastKnown, lastKnownRound);
      PlaceId* path = HvGetShortestPathTo(hv, player, 
                        predictedLocation, &pathLength);
      PlaceId lastKnownVampire = HvGetVampireLocation(hv);
      
      if (pathLength > 0 && placeIsReal(path[0])) {
        registerBestPlay(placeIdToAbbrev(path[0]), "132");
      } else  if (lastKnownVampire >= 0 && lastKnownVampire < 100) {
        int roundVampirePlaced = HvGetRoundVampirePlaced(hv);
        if (roundVampirePlaced >= 0) {
            int roundsToMature = currentRound - roundVampirePlaced;
            int lengthToVampire;
            PlaceId *pathToVampire = HvGetShortestPathTo(hv, player, 
                                    lastKnownVampire, &lengthToVampire);
            if (lengthToVampire < roundsToMature) {
                registerBestPlay(placeIdToAbbrev(pathToVampire[0]), "Hunting!");
            }
        free(pathToVampire);
        }
      } else {
        HMakeRandomMove(hv);
      }
    } else {
      HMakeRandomMove(hv);
    }
  }
}
