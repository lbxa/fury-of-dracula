//
// Created by eric on 28/7/20.
//

#include "minimax.h"

#include <limits.h>
#include <math.h>
#include <string.h>

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

int evaluateGameState(GameView state) {
  int eval = 0;

  // Loop through all hunters
  for (int player = 0; player < PLAYER_DRACULA; ++player) {
    PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
    if (placeIsReal(draculaLocation)) {
      int distanceApart = 0;
      GetShortestPathTo(state, player, draculaLocation, &distanceApart);
      eval += distanceScore(distanceApart) * DISTANCE_FACTOR;
    }
    eval += lifePointScore(GvGetHealth(state, player),
                           GvGetHealth(state, PLAYER_DRACULA)) * LIFE_FACTOR;
  }
  eval += scoreFactor(GvGetScore(state)) * SCORE_FACTOR;
  return eval;
}

bool isGameOver(GameView gameView) {
  return GvGetScore(gameView) <= 0 ||
         GvGetHealth(gameView, PLAYER_DRACULA) <= 0;
}

int minimax(GameView state, int depth, int alpha, int beta) {
  if (depth == 0 || isGameOver(state)) return evaluateGameState(state);
  int turnNumber = GvGetTurnNumber(state);
  Player player = turnNumber % NUM_PLAYERS;
  bool maximisingPlayer = player == PLAYER_DRACULA;
  if (maximisingPlayer) {
    int maxEval = INT_MIN;
    int numReturnedMoves = 0;
    Map map = GetMap(state);
    PlaceId currentLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
    PlaceId *possibleMoves =
        GetPossibleMoves(state, map, PLAYER_DRACULA, currentLocation, true,
                         false, true, 0, &numReturnedMoves, false, true);
    for (int i = 0; i < numReturnedMoves; ++i) {
      GameView newState = GvClone(state);
      printf("Evaluating -> %s\n", placeIdToName(possibleMoves[i]));
      char *play = GetPastPlayStringForMove(state, (char *)placeIdToAbbrev(possibleMoves[i]),
                               player, turnNumber);
      GvProcessMoves(newState, play, NULL);
      int eval = minimax(newState, depth - 1, alpha, beta);
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);
      if (beta <= alpha) break;
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    int numReturnedMoves = 0;
    Map map = GetMap(state);
    PlaceId currentLocation = GvGetPlayerLocation(state, player);
    PlaceId *possibleMoves =
        GetPossibleMoves(state, map, player, currentLocation, true, true, true,
                         GvGetRound(state), &numReturnedMoves, false, false);
    for (int i = 0; i < numReturnedMoves; ++i) {
      GameView newState = GvClone(state);
      char *play = GetPastPlayStringForMove(state, (char *)placeIdToAbbrev(possibleMoves[i]),
                                            player, turnNumber);
      GvProcessMoves(newState, play, NULL);
      int eval = minimax(newState,depth - 1, alpha, beta);
      minEval = min(minEval, eval);
      alpha = min(beta, eval);
      if (beta <= alpha) break;
    }
    return minEval;
  }
}