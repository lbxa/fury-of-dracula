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
#include "PathFinding.h"
#include "Utilities.h"
// #include "minimax.h"

void HvMakeValidMove(PlaceId place, PlaceId *adjacent_places, int size) {
  for (int i = 0; i < 0; i ++) {
    if (adjacent_places[i] == place) {
      return TRUE;
    }
  } 
  return FALSE;
}

PlaceId HvGetDraculaLocation(Hunerview hv, int *numTurns) {

  PlaceId *dracula_trail[TRAIL_SIZE];
  GvGetMoveHistory(view, PLAYER_DRACULA, &dracula_trail, &canFree);
  for (int j = 0; j < TRAIL_SIZE; j++) {
    if (dracula_trail[j] < MapNumPlaces && dracula_trail[j] >= 0) {
      *numTurns = j;
      return dracula_trail[j];
    // Drac's most recent location will be the one if he db to a place in his trail
    } else if (dracula_trail[j] >= DOUBLE_BACK_1 && dracula_trail[j] <= DOUBLE_BACK_5) {
      int doub_back = j + (trail[j] - (DOUBLE_BACK_1 - 1));
      
      if (doub_back < TRAIL_SIZE && dracula_trail[doub_back] >= 0 && dracula_trail[doub_back] < MapNumPlaces) {
        *numTurns = j;
        return dracula_trail[doub_back];
      }
    }
  }
  *numTurns = -1;
  return UNKNOWN_PLACE;
}

void HvMakeFirstMove(HunterView hv) {
  int place = rand() % 23;
  printf("First: %s\n", placeIdToName(place));
  registerBestPlay((char *)PLACES[place].abbrev, "Have we nothing Toulouse?");
} 

void HvMakeRandomMove(HunterView hv) {
  int numMoves = 0;
  PlaceId *possibleMoves = HvWhereCanIGo(hv, &numMoves);
  if (numMoves == 0) {
    FILE *hunterLog = fopen("dracula.log", "a");
    fprintf(hunterLog, "Move (TELEPORT) (%d): %s\n", HvGetRound(hv),
            placeIdToName(TELEPORT));
    fclose(hunterLog);
    registerBestPlay((char *)placeIdToAbbrev(TELEPORT), "");
  } else {
    int moveIndex = rand() % numMoves;
    int placeId = possibleMoves[moveIndex];
    FILE *hunterLog = fopen("dracula.log", "a");
    fprintf(hunterLog, "Move (%d): %s\n", HvGetRound(hv),
            placeIdToName(placeId));
    fclose(hunterLog);

    registerBestPlay((char *)placeIdToAbbrev(placeId),
                     "Have we nothing Toulouse?");
  };
}

void decideHunterMove(void *hv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  HunterView view = (HunterView)hv;
  FILE *draculaLog = fopen("dracula.log", "a");
  fprintf(draculaLog, "\nDracula Move (%d)\n", HvGetRound(hv));
  fclose(draculaLog);

  FILE *turnLog = fopen("turns.log", "a");
  fprintf(turnLog, "\nDracula Move (%d)\n", HvGetRound(hv));
  fclose(turnLog);

  srand(time(NULL));
  
  PlaceId *adjacent_places;
  int size = 0;
  int trail_size = 0;
  PlaceId *dracula_trail = HvGetDraculaLocation(view, &trail_size);

  if (HvGetRound(view) == 0) {
    HvMakeFirstMove(view);
  } else {
    adjacent_places = GvGetReachableByType(view, HvGetPlayer(view), HvGetRound(view), HvGetPlayerLocation(view), true, true, true, &size);

    if (trail_size == 0 && HvMakeValidMove(dracula_trail, adjacent_places, size)) {
      registerBestPlay((char *)placeIdToAbbrev(dracula_trail), "Go to Drac's place");
      return;
    } else if (dracula_trail == UNKNOWN_PLACE && HvGetHealth(view, HvGetPlayer(view)) < GAME_START_HUNTER_LIFE_POINTS) {
      registerBestPlay((char *)placeIdToAbbrev(HvGetPlayerLocation(view, HvGetPlayer(view))), "Hunter is at rest");
      return;
    }
    if (HvGetPlayer(view) == 0) {
      if (HvGetPlayerLocation(view, HvGetPlayer(view)) == GALATZ) {
        registerBestPlay((char *)placeIdToAbbrev(KLAUSENBURG), "Go to Klausenburg");
      } else if(HvGetPlayerLocation(view, HvGetPlayer(view)) == KLAUSENBURG) {
        registerBestPlay((char *)placeIdToAbbrev(GALATZ), "Go to Galatz");
      } else {
        registerBestPlay((char *)placeIdToAbbrev(HvGetPlayerLocation(view, HvGetPlayer(view))), "At rest");
        HvGetShortestPathTo(view, HvGetPlayer(view), dracula_trail, size);
      }
    }
  }
}
