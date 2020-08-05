#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "GameView.h"
#include "Places.h"

void CheckMallocSuccess(void *mem, char *errorMessage) {
  if (mem == NULL) {
    fprintf(stderr, "%s", errorMessage);
    exit(EXIT_FAILURE);
  }
}

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

char GameUtilGetPlayerChar(Player player) {
  if (player == PLAYER_LORD_GODALMING) {
    return 'G';
  } else if (player == PLAYER_DR_SEWARD) {
    return 'S';
  } else if (player == PLAYER_VAN_HELSING) {
    return 'H';
  } else if (player == PLAYER_MINA_HARKER) {
    return 'M';
  }
  return 'D';
}

void GameUtilGetEncounterStr(GameView state, Player currentPlayer,
                             PlaceId playerLoc, char *playStr) {
  int numTraps = 0;
  PlaceId *trapLocations = GvGetTrapLocations(state, &numTraps);
  int numEncounteredTraps = 0;

  int encounterIndex = 3;

  for (int i = 0; i < numTraps; ++i) {
    if (trapLocations[i] == playerLoc && !placeIsSea(playerLoc)) {
      numEncounteredTraps++;
      playStr[encounterIndex] = 'T';
      encounterIndex++;
    }
  }
  if (GvGetVampireLocation(state) == playerLoc && !placeIsSea(playerLoc)) {
    playStr[encounterIndex] = 'V';
    encounterIndex++;
  }
  if (currentPlayer != PLAYER_DRACULA && !placeIsSea(playerLoc) &&
      GvGetPlayerLocation(state, PLAYER_DRACULA) == playerLoc) {
    playStr[encounterIndex] = 'D';
    encounterIndex++;
  }
  for (int i = encounterIndex; i < 7; i++) {
    playStr[i] = '.';
  }
}

void DraculaUtilEncounterString(GameView state, Player currentPlayer,
                                PlaceId playerLoc, char *playStr) {
  int round = GvGetRound(state);
  if (!placeIsSea(playerLoc)) {
    if (round % 13 == 0) {
      playStr[3] = '.';
      playStr[4] = 'V';
    } else {
      playStr[3] = 'T';
      playStr[4] = '.';
    }
  } else {
    playStr[3] = '.';
    playStr[4] = '.';
  }

  if (GvIsVampireMaturing(state)) {
    playStr[5] = 'V';
  } else if (GvGetExpiringTrap(state) != NOWHERE) {
    playStr[5] = 'M';
  } else {
    playStr[5] = '.';
  }

  playStr[6] = '.';
}

char *GetPastPlayStringForMove(GameView state, char *newPlay,
                              Player currentPlayer, int turnNumber) {
  char *playStr = malloc(sizeof(char) * 8);
  playStr[0] = GameUtilGetPlayerChar(currentPlayer);
  if (newPlay != NULL) {
    playStr[1] = newPlay[0];
    playStr[2] = newPlay[1];
    PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
<<<<<<< HEAD
    PlaceId playerLocation =
        ResolveLocation(state, player, placeAbbrevToId(newPlay));
    if (currentPlayer == PLAYER_DRACULA) {
      DraculaUtilEncounterString(state, currentPlayer, playerLocation, playStr);
    } else {
      GameUtilGetEncounterStr(state, currentPlayer, playerLocation, playStr);
    }
=======
    // PlaceId playerLocation = ResolveLocation(state, player, placeAbbrevToId(newPlay));
    getEncounterStr(state, currentPlayer, player->lastResolvedLocation, playStr);
>>>>>>> 1191eba3817e7c5758b881f4d9454feda305af75
  } else {
    // Stay at location
    PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
    const char *abbrev = placeIdToAbbrev(player->lastResolvedLocation);
    playStr[1] = abbrev[0];
    playStr[2] = abbrev[1];
    if (currentPlayer == PLAYER_DRACULA) {
      DraculaUtilEncounterString(state, currentPlayer,
                                 player->lastResolvedLocation, playStr);
    } else {
      GameUtilGetEncounterStr(state, currentPlayer,
                              player->lastResolvedLocation, playStr);
    }
  }
  playStr[7] = '\0';
  return playStr;
}