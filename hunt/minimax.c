//
// Created by eric on 28/7/20.
//

#include <view/GameView.h>
#include "minimax.h"
#include <limits.h>
#include <view/path_finding.h>
#include <string.h>
#include <math.h>

char getPlayerChar(Player player) {
    if (player == PLAYER_LORD_GODALMING) {
        return 'G';
    } else if (player == PLAYER_DR_SEWARD) {
        return 'S';
    } else if (player == PLAYER_VAN_HELSING) {
        return 'V';
    } else if (player == PLAYER_MINA_HARKER) {
        return 'M';
    }
    return 'D';
}

void getEncounterStr(GameView state, Player currentPlayer, PlaceId playerLoc, char* playStr) {
    int numTraps = 0;
    PlaceId *trapLocations = GvGetTrapLocations(state, &numTraps);
    int numEncounteredTraps = 0;

    int encounterIndex = 3;

    for (int i = 0; i < numTraps; ++i) {
        if (trapLocations[i] == playerLoc) {
            numEncounteredTraps++;
            playStr[encounterIndex] = 'T';
            encounterIndex++;
        }
    }
    if (GvGetVampireLocation(state) == playerLoc) {
        playStr[encounterIndex] = 'V';
        encounterIndex++;
    }
    if (currentPlayer != PLAYER_DRACULA &&
        GvGetPlayerLocation(state, PLAYER_DRACULA) == playerLoc) {
        playStr[encounterIndex] = 'D';
        encounterIndex++;
    }
    for (int i = encounterIndex; i < 7; i++) {
        playStr[i] = '.';
    }
}

void addMoveToPastPlays(GameView state, char *pastPlays, char *newPlay, Player currentPlayer, int turnNumber) {
    char *playStr = malloc(sizeof(char) * 8);
    playStr[0] = getPlayerChar(currentPlayer);
    if (newPlay != NULL) {
        playStr[1] = newPlay[0];
        playStr[2] = newPlay[1];
        PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
        PlaceId playerLocation = ResolveLocation(state, player, placeAbbrevToId(newPlay));
        getEncounterStr(state, currentPlayer, player->lastResolvedLocation, playStr);
    } else {
        // Stay at location
        PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
        const char *abbrev = placeIdToAbbrev(player->lastResolvedLocation);
        playStr[1] = abbrev[0];
        playStr[2] = abbrev[1];
        getEncounterStr(state, currentPlayer, player->lastResolvedLocation, playStr);
    }

    playStr[7] = ' ';
    int length = ((turnNumber + 1) * 8);
    if (length > 8) {
        pastPlays[length - 9] = ' ';
    }
    pastPlays = realloc(pastPlays, length);
    int offset = turnNumber * 8;
    for (int i = 0; i < 8; i++) {
        pastPlays[i + offset] = playStr[i];
    }
    pastPlays[length - 1] = '\0';
}

int distanceScore(int numberMoves) {
    return (int) log(numberMoves == 0 ? 0.00001f : (float)numberMoves);
}

int lifePointScore(int hunterLife, int draculaLife) {
    if (draculaLife <= LIFE_LOSS_HUNTER_ENCOUNTER) return -1;
    return draculaLife / hunterLife;
}

int scoreFactor(int score) {
    return 1 - (score / GAME_START_SCORE);
}

int evaluateGameState(GameView state) {
    int eval = 0;

    // Loop through all hunters
    for (int player = 0; player < PLAYER_DRACULA; ++player) {
        PlaceId draculaLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
        if (placeIsReal(draculaLocation)) {
            int distanceApart = 0;
            GetShortestPathTo(state, player, draculaLocation, &distanceApart);
            eval += distanceScore(distanceApart);
        }
        eval += lifePointScore(GvGetHealth(state, player), GvGetHealth(state, PLAYER_DRACULA));
    }
    eval += scoreFactor(GvGetScore(state));
    return eval;
}

bool isGameOver(GameView gameView) {
    return GvGetScore(gameView) <= 0 || GvGetHealth(gameView, PLAYER_DRACULA) <= 0;
}

int m_max(int a, int b) {
    return (a > b) ? a : b;
}

int m_min(int a, int b) {
    return (a < b) ? a : b;
}

int minimax(GameView state, char *pastPlays, int depth, int alpha, int beta) {
    if (depth == 0 || isGameOver(state)) return evaluateGameState(state);
    int turnNumber = GvGetTurnNumber(state);
    Player player = turnNumber % NUM_PLAYERS;
    bool maximisingPlayer = player == PLAYER_DRACULA;
    if (maximisingPlayer) {
        int maxEval = INT_MIN;
        int numReturnedMoves = 0;
        Map map = GetMap(state);
        PlaceId currentLocation = GvGetPlayerLocation(state, PLAYER_DRACULA);
        PlaceId *possibleMoves =  GetPossibleMoves(state, map, PLAYER_DRACULA, currentLocation,
                                true, false, true, 0, &numReturnedMoves, false, true);
        for (int i = 0; i < numReturnedMoves; ++i) {
            char *newPlayString = malloc(sizeof(strlen(pastPlays)));
            strcpy(newPlayString, pastPlays);
            addMoveToPastPlays(state, newPlayString, (char*) placeIdToAbbrev(possibleMoves[i]), player, turnNumber);
            GameView newState = GvNew(newPlayString, NULL);
            int eval = minimax(newState, newPlayString, depth - 1, alpha, beta);
            maxEval = m_max(maxEval, eval);
            alpha = m_max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        int numReturnedMoves = 0;
        Map map = GetMap(state);
        PlaceId currentLocation = GvGetPlayerLocation(state, player);
        PlaceId *possibleMoves =  GetPossibleMoves(state, map, player, currentLocation,
                                                   true, true, true, GvGetRound(state), &numReturnedMoves, false, false);
        for (int i = 0; i < numReturnedMoves; ++i) {
            char *newPlayString = malloc(sizeof(strlen(pastPlays)));
            strcpy(newPlayString, pastPlays);
            addMoveToPastPlays(state, newPlayString, (char*) placeIdToAbbrev(possibleMoves[i]), player, turnNumber);
            GameView newState = GvNew(newPlayString, NULL);
            int eval = minimax(newState, newPlayString, depth - 1, alpha, beta);
            minEval = m_min(minEval, eval);
            alpha = m_min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}