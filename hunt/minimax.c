//
// Created by eric on 28/7/20.
//

#include "minimax.h"

#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "GameView.h"
#include "PathFinding.h"
#include "Utilities.h"

#define LIFE_FACTOR 80
#define SCORE_FACTOR 50
#define DISTANCE_FACTOR 60

double distanceScore(int numberMoves, int draculaHealth) {
  double weight = 1;
  if (draculaHealth < 10)
    weight = 10;
  else if (draculaHealth < 20)
    weight = 1;
  else if (draculaHealth > 40)
    weight = 0.6;
  else if (draculaHealth >= 50)
    weight = 0.3;
  else if (draculaHealth >= 60)
    weight = 0.1;

  if (numberMoves == 0) return -100 * weight;
  if (numberMoves == 1) return -50 * weight;
  if (numberMoves == 2) return -10.0f * weight;
  if (numberMoves == 3) return -5.0f * weight;
  if (numberMoves == 4) return -3.0f * weight;
  if (numberMoves == 5) return -2.0f * weight;
  if (numberMoves == 6) return 1.0f * weight;
  if (numberMoves == 7) return 2.0f * weight;
  return 15 * weight;
}

double scoreFactor(int score) {
  return 1.0f - ((double)score / (double)GAME_START_SCORE);
}

int evaluateGameState(GameView state, Path **distanceLookup) {
  int eval = 0;
  Map map = GvGetMap(state);
  int round = GvGetRound(state);
  // Loop through all hunters
  int smallestDistance = INT_MAX;
  PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
  int draculaHealth = GvGetHealth(state, PLAYER_DRACULA);
  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    PlaceId hunterLocation = GvGetPlayerLocation(state, player);
    if (placeIsReal(draculaLocation)) {
      Path *pathLookup;
      if (distanceLookup[hunterLocation] == NULL) {
        pathLookup =
            GetPathLookupTableFrom(state, map, player, PLACES[hunterLocation],
                                   true, true, true, round, true, false);
        distanceLookup[hunterLocation] = pathLookup;
      } else {
        pathLookup = distanceLookup[hunterLocation];
      }
      Path path = pathLookup[draculaLocation];
      smallestDistance = min(smallestDistance, path->distance);
      smallestDistance += path->distance;
      //      eval += distanceScore(path->distance, draculaHealth) *
      //      DISTANCE_FACTOR;
    }
  }
  eval += distanceScore(smallestDistance, draculaHealth) * DISTANCE_FACTOR;
  //  eval += (draculaHealth - GAME_START_BLOOD_POINTS) * LIFE_FACTOR;
  if (draculaHealth <= 0) eval -= 99999999;
  if (draculaHealth <= 2) {
    if (placeIsSea(draculaLocation)) {
      eval -= 9999999;
    }
    eval += -10000 * LIFE_FACTOR;
  } else if (draculaHealth <= 5) {
    eval += -1000 * LIFE_FACTOR;
  } else if (draculaHealth <= 10) {
    eval += -800 * LIFE_FACTOR;
  } else if (draculaHealth <= 15) {
    eval += -500 * LIFE_FACTOR;
  } else if (draculaHealth <= 20) {
    eval += -300 * LIFE_FACTOR;
  } else if (draculaHealth <= 30) {
    eval += -100 * LIFE_FACTOR;
  } else {
    if (draculaHealth <= 40) {
      eval += (draculaHealth - GAME_START_BLOOD_POINTS) * LIFE_FACTOR;
    }
  }
  eval += (int)(scoreFactor(GvGetScore(state)) * SCORE_FACTOR);
  if (GvGetScore(state) == 0) eval += 99999;
  return eval;
}

bool isGameOver(GameView gameView) {
  return GvGetScore(gameView) <= 0 ||
         GvGetHealth(gameView, PLAYER_DRACULA) <= 0;
}

int MiniMax(GameView state, Path **distanceLookup, int depth, int alpha,
            int beta) {
  if (depth == 0 || isGameOver(state))
    return evaluateGameState(state, distanceLookup);
  int turnNumber = GvGetTurnNumber(state);
  Player player = turnNumber % NUM_PLAYERS;
  bool maximisingPlayer = player == PLAYER_DRACULA;
  if (maximisingPlayer) {
    int maxEval = INT_MIN;
    int numReturnedMoves = 0;
    Map map = GvGetMap(state);
    PlaceId currentLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
    PlaceId *possibleMoves =
        GetPossibleMoves(state, map, PLAYER_DRACULA, currentLocation, true,
                         false, true, 0, &numReturnedMoves, false, true);
    for (int i = 0; i < numReturnedMoves; ++i) {
      GameView newState = GvClone(state);
      char *play = GetPastPlayStringForMove(
          state, (char *)placeIdToAbbrev(possibleMoves[i]), player, turnNumber);
      GvProcessMoves(newState, play, NULL);
      int eval = MiniMax(newState, distanceLookup, depth - 1, alpha, beta);
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);
      free(play);
      GvFree(newState);
      if (beta <= alpha) break;
    }
    free(possibleMoves);

    return maxEval;
  } else {
    //    int minEval = INT_MAX;
    //    int numReturnedMoves = 0;
    Map map = GvGetMap(state);
    PlaceId currentLocation = GvGetPlayerLocation(state, player);

    char *bestMove = NULL;
    PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);

    Path *pathLookup;
    if (distanceLookup[currentLocation] == NULL) {
      pathLookup = GetPathLookupTableFrom(state, map, player,
                                          PLACES[currentLocation], true, true,
                                          true, GvGetRound(state), true, false);
      distanceLookup[currentLocation] = pathLookup;
    } else {
      pathLookup = distanceLookup[currentLocation];
    }
    Path path = pathLookup[draculaLocation];
    Path cur = path;
    Path last = NULL;
    while (cur && cur->predecessor) {
      last = cur;
      cur = cur->predecessor;
    }

    if (last != NULL) {
      bestMove = last->place;
    } else {
      bestMove = placeIdToAbbrev(currentLocation);
    }

    //    printf("%s -> %s\n", placeIdToAbbrev(currentLocation), bestMove);
    //    printf("%s\n", placeIdToAbbrev(draculaLocation));

    GameView newState = GvClone(state);
    char *play = GetPastPlayStringForMove(state, bestMove, player, turnNumber);
    GvProcessMoves(newState, play, NULL);
    int eval = MiniMax(newState, distanceLookup, depth - 1, alpha, beta);
    beta = min(beta, eval);
    GvFree(newState);
    free(play);
    return eval;
  }
}