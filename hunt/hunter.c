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

<<<<<<< HEAD
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

  /**
   * Prioritise land locations
   * Predictions what areas he may be in, by looking at last known location,
   * predictions involving sea moves
   * If known vampire location go to it
   * hunter stay in position as possible move
   * Research turn
   */
  int player = HvGetPlayer(hv);
  PlaceId playerLocation = HvGetPlayerLocation(hv, player);
  int currentRound = HvGetRound(hv);
  if (currentRound == 0) {
    HFirst(player);
  } else {
    Round lastKnownRound = -1;
    PlaceId lastKnown = HvGetLastKnownDraculaLocation(hv, &lastKnownRound);

    if (lastKnown == playerLocation && currentRound - lastKnownRound <= 1) {
      // Stay at current as dracula is there
      registerBestPlay(placeIdToAbbrev(playerLocation), "132");
    } else if (lastKnown != NOWHERE && currentRound - lastKnownRound <= 3) {
      int pathLength = 0;
      PlaceId* path = HvGetShortestPathTo(hv, player, lastKnown, &pathLength);
      if (pathLength > 0 && placeIsReal(path[0])) {
        registerBestPlay(placeIdToAbbrev(path[0]), "132");
=======
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

  if (HvGetRound(view) == 0) {
    HvMakeFirstMove(view);
  } else {
    HvMakeRandomMove(view);
    //    if (view) DvMakeRandomMove(view);
    //MakeMinimaxMove(view);
  }
}

//Function to find if dracula is near from hunter's current location
// static PlaceId draculaLocation (PlaceId nearby[], int numLoc, PlaceId drac) {

//   int x = 0;
//   while (x < numLoc) {
//     if (nearby[x] == drac) {
//       return 0;
//     }
//     x++;
//   }
//   return 1;
// }
/*
void decideHunterMove(void *hv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//  srand ( GvGetPlayer(1) );
  
  HunterView view = (HunterView)hv;
  FILE *turnLog = fopen("turns.log", "a");
  fprintf(turnLog, "\nHunter Move (%d)\n", HvGetRound(view));
  fclose(turnLog);

  Message array;
  int numLoc = 0;
  char *forward = "";
  char *backward = "abcde";

  //Player moves
  if (HvGetRound(view) == 0) {

    if (HvGetPlayer(view) == PLAYER_LORD_GODALMING) {

      registerBestPlay("CD", forward);  

    } else if (HvGetPlayer(view) == PLAYER_DR_SEWARD) {

      registerBestPlay("AT", forward); 

    } else if (HvGetPlayer(view) == PLAYER_VAN_HELSING) {

      registerBestPlay("VR", forward);

    } else {

      registerBestPlay("CN", forward);

    }

  //In this case if dracula is in same location as Hunter, Hunter will stay in that location.
  } else if (HvGetPlayerLocation(view, PLAYER_DRACULA) == HvGetPlayerLocation(view, HvGetPlayer(view))) {

      registerBestPlay(placeIdToAbbrev(HvGetPlayerLocation(view, PLAYER_DRACULA)), (char *)array);
  
  //In this case, if dracula is lost, Hunter goes back to his/her path
  } else if (strcmp((char *)array, forward) != 0 && strcmp((char *)array, backward) != 0) {

      if (draculaLocation(HvWhereCanIGoByType(view, true, true, false, &numLoc), numLoc, placeAbbrevToId((char *)array)) == 1) {        
          registerBestPlay(placeIdToAbbrev(HvGetPlayerLocation(view, HvGetPlayer(view))), (char *)array);
      } else {
          registerBestPlay((char *)array, forward);
      }

  // All players case
  } else if (HvGetPlayer(view) == PLAYER_LORD_GODALMING) {

      //get message for Lord Godalming - implement

      //player taken to hospital
      if (HvGetPlayerLocation(view, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARY){ 
          registerBestPlay("SZ", forward);
      //player's way returning from hospital
      } else if (HvGetPlayerLocation(view, PLAYER_LORD_GODALMING) == SZEGED) {
          registerBestPlay("KL",forward);
      } else if(HvGetPlayerLocation(view, PLAYER_DRACULA) == CASTLE_DRACULA) {
          registerBestPlay("CD", forward);
      } else if(HvGetHealth(view, PLAYER_LORD_GODALMING) < GAME_START_HUNTER_LIFE_POINTS && HvGetPlayerLocation(view, PLAYER_DRACULA) != CASTLE_DRACULA) {
          registerBestPlay("GA", forward);  
>>>>>>> 1191eba3817e7c5758b881f4d9454feda305af75
      } else {
        HMakeRandomMove(hv);
      }
    } else {
      HMakeRandomMove(hv);
    }
  }
}
<<<<<<< HEAD
=======
*/
>>>>>>> 1191eba3817e7c5758b881f4d9454feda305af75
