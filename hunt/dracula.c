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

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "PathFinding.h"
#include "Utilities.h"
// #include "minimax.h"

void DvMakeFirstMove(DraculaView hv) {
  int place = rand() % 23;
  printf("First: %s\n", placeIdToName(place));
  registerBestPlay((char *)PLACES[place].abbrev, "Have we nothing Toulouse?");
}

void DvMakeRandomMove(DraculaView hv) {
  int numMoves = 0;
  PlaceId *possibleMoves = DvGetValidMoves(hv, &numMoves);
  if (numMoves == 0) {
    FILE *draculaLog = fopen("dracula.log", "a");
    fprintf(draculaLog, "Move (TELEPORT) (%d): %s\n", DvGetRound(hv),
            placeIdToName(TELEPORT));
    fclose(draculaLog);
    registerBestPlay((char *)placeIdToAbbrev(TELEPORT), "");
  } else {
    int moveIndex = rand() % numMoves;
    int placeId = possibleMoves[moveIndex];
    FILE *draculaLog = fopen("dracula.log", "a");
    fprintf(draculaLog, "Move (%d): %s\n", DvGetRound(hv),
            placeIdToName(placeId));
    fclose(draculaLog);

    registerBestPlay((char *)placeIdToAbbrev(placeId),
                     "Have we nothing Toulouse?");
  };
}

// void MakeMinimaxMove(DraculaView dv) {
//   GameView state = DvGetGameView(dv);
//   Map map = GetMap(state);
//   PlaceId currentLocation = DvGetPlayerLocation(dv, PLAYER_DRACULA);
//   int numReturnedMoves = 0;
//   int depth = 1;
//   PlaceId *possibleMoves = GetPossibleMoves(
//       state, map, PLAYER_DRACULA, currentLocation, true, false, true,
//       GvGetRound(state), &numReturnedMoves, false, true);
//   PlaceId bestMove = 0;
//   int bestEval = INT_MIN;
//   for (int i = 0; i < numReturnedMoves; ++i) {
//     printf("Evaluating -> %s\n", placeIdToName(possibleMoves[i]));
//     GameView newState = GvClone(state);
//     char *play = GetPastPlayStringForMove(state, (char *)placeIdToAbbrev(possibleMoves[i]),
//                                           PLAYER_DRACULA, GvGetTurnNumber(state));
//     GvProcessMoves(newState, play, NULL);
//     int eval = minimax(newState, depth, INT_MIN, INT_MAX);
//     if (eval > bestEval) {
//       bestEval = eval;
//       bestMove = possibleMoves[i];
//       registerBestPlay((char *)placeIdToAbbrev(bestMove), NULL);
//     }
//   }
// }

void decideDraculaMove(void *dv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  DraculaView view = (DraculaView)dv;
  FILE *draculaLog = fopen("dracula.log", "a");
  fprintf(draculaLog, "\nDracula Move (%d)\n", DvGetRound(dv));
  fclose(draculaLog);

  FILE *turnLog = fopen("turns.log", "a");
  fprintf(turnLog, "\nDracula Move (%d)\n", DvGetRound(dv));
  fclose(turnLog);

  srand(time(NULL));

  if (DvGetRound(view) == 0) {
    DvMakeFirstMove(view);
  } else {
    DvMakeRandomMove(view);
    //    if (view) DvMakeRandomMove(view);
    //MakeMinimaxMove(view);
  }
}
