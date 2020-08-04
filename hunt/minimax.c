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

#define LIFE_FACTOR 10
#define SCORE_FACTOR 2
#define DISTANCE_FACTOR 4

int distanceScore(int numberMoves) {
  return (int)log(numberMoves == 0 ? 0.00001f : (float)numberMoves);
}

int lifePointScore(int hunterLife, int draculaLife) {
  if (draculaLife <= LIFE_LOSS_HUNTER_ENCOUNTER) return -1;
  if (hunterLife == 0) return 1;
  return draculaLife / hunterLife;
}

int scoreFactor(int score) { return 1 - (score / GAME_START_SCORE); }

int evaluateGameState(GameView state, HashTable *distanceLookup) {
  int eval = 0;
  Map map = GvGetMap(state);
  int round = GvGetRound(state);
  // Loop through all hunters
  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
    PlaceId hunterLocation = GvGetPlayerLocation(state, player);
    if (placeIsReal(draculaLocation)) {
      HashTable pathLookup;
      if (distanceLookup[hunterLocation] == NULL) {
        pathLookup =
            GetPathLookupTableFrom(state, map, player, PLACES[hunterLocation],
                                   true, false, true, round, true, true);
        distanceLookup[hunterLocation] = pathLookup;
      } else {
        pathLookup = distanceLookup[hunterLocation];
      }
      Path path = (Path)HashGet(pathLookup, placeIdToAbbrev(hunterLocation));
      eval += distanceScore(path->distance) * DISTANCE_FACTOR;
    }
    eval += lifePointScore(GvGetHealth(state, player),
                           GvGetHealth(state, PLAYER_DRACULA)) *
            LIFE_FACTOR;
  }
  eval += scoreFactor(GvGetScore(state)) * SCORE_FACTOR;
  return eval;
}

bool isGameOver(GameView gameView) {
  return GvGetScore(gameView) <= 0 ||
         GvGetHealth(gameView, PLAYER_DRACULA) <= 0;
}

int minimax(GameView state, HashTable *distanceLookup, int depth, int alpha,
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
      int eval = minimax(newState, distanceLookup, depth - 1, alpha, beta);
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);
      free(play);
      GvFree(newState);
      if (beta <= alpha) break;
    }
    free(possibleMoves);

    return maxEval;
  } else {
    int minEval = INT_MAX;
    int numReturnedMoves = 0;
    Map map = GvGetMap(state);
    PlaceId currentLocation = GvGetPlayerLocation(state, player);
    PlaceId *possibleMoves =
        GetPossibleMoves(state, map, player, currentLocation, true, true, true,
                         GvGetRound(state), &numReturnedMoves, false, false);
    for (int i = 0; i < numReturnedMoves; ++i) {
      GameView newState = GvClone(state);
      char *play = GetPastPlayStringForMove(
          state, (char *)placeIdToAbbrev(possibleMoves[i]), player, turnNumber);
      GvProcessMoves(newState, play, NULL);
      int eval = minimax(newState, distanceLookup, depth - 1, alpha, beta);
      minEval = min(minEval, eval);
      alpha = min(beta, eval);
      free(play);
      GvFree(newState);
      if (beta <= alpha) break;
    }
    free(possibleMoves);
    return minEval;
  }
}