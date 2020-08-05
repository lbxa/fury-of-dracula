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

#define LIFE_FACTOR {}
#define SCORE_FACTOR {}
#define DISTANCE_FACTOR {}

int distanceScore(int numberMoves) {
  if (numberMoves > 8) return 10;
//    return (int)(2 * log(numberMoves == 0 ? 0.00001f : (float)numberMoves) - 4);
    return (int)(23 * log(numberMoves == 0 ? 0.00001f : (float)numberMoves) - 55);
}

int lifePointScore(int hunterLife, int draculaLife) {
  if (draculaLife <= LIFE_LOSS_HUNTER_ENCOUNTER) return -1;
  if (hunterLife == 0) return 1;
  return draculaLife / hunterLife;
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
  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
    PlaceId hunterLocation = GvGetPlayerLocation(state, player);
    if (placeIsReal(draculaLocation)) {
      Path *pathLookup;
      if (distanceLookup[hunterLocation] == NULL) {
        pathLookup =
            GetPathLookupTableFrom(state, map, player, PLACES[hunterLocation],
                                   true, false, true, round, true, true);
        distanceLookup[hunterLocation] = pathLookup;
      } else {
        pathLookup = distanceLookup[hunterLocation];
      }
      Path path = pathLookup[draculaLocation];
      smallestDistance = min(smallestDistance, path->distance);
    }
  }
  eval += distanceScore(smallestDistance) * DISTANCE_FACTOR;
  int draculaHealth = GvGetHealth(state, PLAYER_DRACULA);
  if (draculaHealth <= 10) {
    eval += -100 * LIFE_FACTOR;
  } else {
    eval += draculaHealth * LIFE_FACTOR;
  }

  eval += (int)(scoreFactor(GvGetScore(state)) * SCORE_FACTOR);
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
      pathLookup =
          GetPathLookupTableFrom(state, map, player, PLACES[currentLocation],
                                 true, false, true, GvGetRound(state), true, true);
      distanceLookup[currentLocation] = pathLookup;
    } else {
      pathLookup = distanceLookup[currentLocation];
    }
    Path path = pathLookup[draculaLocation];
    Path cur = path;
    while (cur && cur->predecessor) {
      cur = cur->predecessor;
    }

    if (cur == NULL) {
      bestMove = cur->place;
    } else {
      bestMove = placeIdToAbbrev(currentLocation);
    }

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