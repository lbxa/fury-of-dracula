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
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "PathFinding.h"
#include "Utilities.h"
#include "minimax.h"

int LocationDistanceScore(int numberMoves) {
  if (numberMoves > 10) return 10;
  return (int)(30 * log(numberMoves == 0 ? 0.00001f : (float)numberMoves) - 74);
}

void DvMakeFirstMove(DraculaView dv) {
  // Find best starting location
  GameView state = DvGetGameView(dv);
  Map map = GvGetMap(state);
  int round = GvGetRound(state);
  PlaceId bestLocation = 0;
  double bestLocationScore = 0.0f;

  Path *distanceLookup[4];
  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    distanceLookup[player] = GetPathLookupTableFrom(
        state, map, player, PLACES[DvGetPlayerLocation(dv, player)], true, true,
        true, round, true, false);
  }

  for (int place = 0; place < NUM_REAL_PLACES; ++place) {
    double locationScore = 0.0f;
    if (place == HOSPITAL_PLACE) continue;

    for (int player = 0; player < PLAYER_DRACULA; ++player) {
      int numMoves = distanceLookup[player][place]->distance;
      locationScore += LocationDistanceScore(numMoves);
    }
    if (placeIsSea(place)) locationScore *= 0.8;
    if (locationScore > bestLocationScore) {
      bestLocation = place;
      bestLocationScore = locationScore;
    }
  }

  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    for (int i = 0; i < NUM_REAL_PLACES; ++i) {
      FreePathNode(distanceLookup[player][i]);
    }
  }

  //  int place = rand() % 23;
  registerBestPlay((char *)PLACES[bestLocation].abbrev,
                   "Have we nothing Toulouse?");
}

void DvMakeRandomMove(DraculaView hv) {
  int numMoves = 0;
  PlaceId *possibleMoves = DvGetValidMoves(hv, &numMoves);
  if (numMoves == 0) {
    registerBestPlay((char *)placeIdToAbbrev(TELEPORT), "");
  } else {
    int moveIndex = rand() % numMoves;
    int placeId = possibleMoves[moveIndex];
    registerBestPlay((char *)placeIdToAbbrev(placeId),
                     "Have we nothing Toulouse?");
  };
}

void MakeMinimaxMove(DraculaView dv) {
  GameView state = DvGetGameView(dv);
  Map map = GvGetMap(state);
  PlaceId currentLocation = DvGetPlayerLocation(dv, PLAYER_DRACULA);
  int numReturnedMoves = 0;
  int depth = 16;
  PlaceId *possibleMoves =
      GetPossibleMoves(state, map, PLAYER_DRACULA, currentLocation, true, false,
                       true, GvGetRound(state), &numReturnedMoves, false, true);
  if (numReturnedMoves == 0) {
    registerBestPlay(placeIdToAbbrev(TELEPORT), "");
  }
  PlaceId bestMove = 0;
  int bestEval = INT_MIN;

  Path **pathLookup = malloc(sizeof(Path *) * NUM_REAL_PLACES);

  for (int i = 0; i < NUM_REAL_PLACES; i++) {
    pathLookup[i] = NULL;
  }

  for (int i = 0; i < numReturnedMoves; ++i) {
    printf("Evaluating -> %s\n", placeIdToName(possibleMoves[i]));
    GameView newState = GvClone(state);
    char *play = GetPastPlayStringForMove(
        state, (char *)placeIdToAbbrev(possibleMoves[i]), PLAYER_DRACULA,
        GvGetTurnNumber(state));
    GvProcessMoves(newState, play, NULL);
    int eval = 0;
    eval = MiniMax(newState, pathLookup, depth, INT_MIN, INT_MAX);
    if (eval > bestEval) {
      bestEval = eval;
      bestMove = possibleMoves[i];
      printf("Best move updated: %d %s\n", bestMove, placeIdToName(bestMove));
      registerBestPlay(placeIdToAbbrev(bestMove), "");
    }
    GvFree(newState);
    free(play);
  }

  // Free memory
  //  for (int i = 0; i < NUM_REAL_PLACES; ++i) {
  //    if (pathLookup[i] != NULL) {
  //
  //    }
  //  }
}

void decideDraculaMove(DraculaView dv) {
  DraculaView view = (DraculaView)dv;
  srand(time(NULL));

  if (DvGetRound(view) == 0) {
    DvMakeFirstMove(view);
  } else {
    //    if (view) DvMakeRandomMove(view);
    clock_t start;
    start = clock();
    MakeMinimaxMove(view);
    double time_taken = ((double)clock() - start) / CLOCKS_PER_SEC;
    printf("Turn time taken: %lf\n", time_taken);
  }
}
