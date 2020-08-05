//
// Created by eric on 28/7/20.
//

#include "Game.h"

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "GameView.h"
#include "Players.h"
#include "dracula.h"
#include "hunter.h"

char *pastPlays = NULL;
Message *messages = NULL;
char *bestPlay = NULL;
Message lastMessage = "";
GameView state;
Player currentPlayer = 0;
int turnNumber = 0;

void registerBestPlay(char *play, Message message) {
  bestPlay = play;
  //    strcpy(lastMessage, lastMessage);
}

char GameGetPlayerChar(Player player) {
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

void GameGetEncounterStr(PlaceId playerLoc, char *playStr) {
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
  free(trapLocations);
}

void DraculaEncounterString(PlaceId playerLoc, char *playStr) {
  int round = turnNumber / 5;
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

void addBestMoveToPastPlays() {
  char *playStr = malloc(sizeof(char) * 8);
  playStr[0] = GameGetPlayerChar(currentPlayer);
  if (currentPlayer == PLAYER_DRACULA) assert(bestPlay != NULL);
  if (bestPlay != NULL) {
    playStr[1] = bestPlay[0];
    playStr[2] = bestPlay[1];
    PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
    PlaceId playerLocation =
        ResolveLocation(state, player, placeAbbrevToId(bestPlay));
    if (currentPlayer == PLAYER_DRACULA) {
      DraculaEncounterString(playerLocation, playStr);
    } else {
      GameGetEncounterStr(playerLocation, playStr);
    }
  } else {
    // Stay at location
    PlayerDetails player = GetPlayerDetailsArray(state)[currentPlayer];
    const char *abbrev = placeIdToAbbrev(player->lastResolvedLocation);
    playStr[1] = abbrev[0];
    playStr[2] = abbrev[1];
    if (currentPlayer == PLAYER_DRACULA) {
      DraculaEncounterString(player->lastResolvedLocation, playStr);
    } else {
      GameGetEncounterStr(player->lastResolvedLocation, playStr);
    }
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
  // Reset
  free(playStr);
  bestPlay = NULL;
}

int main(void) {
  for (int i = 0; i < 1; i++) {
    pastPlays = NULL;
    messages = NULL;
    bestPlay = NULL;
    state = NULL;
    currentPlayer = 0;
    turnNumber = 0;
    pastPlays = malloc(1);
    pastPlays[0] = '\0';
    state = GvNew(pastPlays, messages);
    while (GvGetScore(state) > 0 && GvGetHealth(state, PLAYER_DRACULA) > 0) {
      printf("Turn: %d\n", turnNumber);
      printf("Plays: %s\n", pastPlays);
      currentPlayer = turnNumber % NUM_PLAYERS;
      char *move = NULL;
      if (currentPlayer == PLAYER_DRACULA) {
        DraculaView dv = DvNew(pastPlays, messages);
        decideDraculaMove(dv);
        free(dv);
      } else {
        HunterView hv = HvNew(pastPlays, messages);
        decideHunterMove(hv);
        free(hv);
      }
      addBestMoveToPastPlays();
      turnNumber++;
      free(state);
      state = GvNew(pastPlays, messages);
    }

    printf("Plays: %s\n", pastPlays);
    printf("Score: %d\n", GvGetScore(state));
    printf("Dracula Health: %d\n", GvGetHealth(state, PLAYER_DRACULA));

    FILE *outcomes = fopen("outcomes.txt", "a");
    fprintf(outcomes,"Score: %d\n", GvGetScore(state));
    fprintf(outcomes,"Dracula Health: %d\n\n", GvGetHealth(state, PLAYER_DRACULA));
    fclose(outcomes);

    if (GvGetScore(state) <= 0) {
      printf("Dracula won\n");
    } else {
      printf("Hunters won\n");
    }

    FILE *f = fopen("result.log", "a");
    if (f == NULL) {
      perror("Failed to save result to log: ");
      exit(EXIT_FAILURE);
    }
    fprintf(f, "%s\n\n", pastPlays);
    fclose(f);
    GvFree(state);
  }

  return 0;
}