//
// Created by eric on 29/7/20.
//
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

char getPlayerChar(Player player) {
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
  // TODO: Handle that hunter shouldn't see all the information for dracula
  char *playStr = malloc(sizeof(char) * 8);
  playStr[0] = getPlayerChar(currentPlayer);
  if (newPlay != NULL) {
    playStr[1] = newPlay[0];
    playStr[2] = newPlay[1];
    PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
    // PlaceId playerLocation = ResolveLocation(state, player, placeAbbrevToId(newPlay));
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