////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "GameView.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "Map.h"
#include "PathFinding.h"
#include "Places.h"
#include "Utilities.h"

struct gameView {
  Map map;
  PlayerDetails players[NUM_PLAYERS];
  int gameScore;
  int turnNumber;
  int numberTraps;
  PlaceId trapLocations[TRAIL_SIZE];
  PlaceId vampireLocation;
  int roundVampirePlaced;
};

// Code written by:
// Eric | Lucas | Stephen | Debbie - (20T2)

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

/**
 * Creates a GameView and sets all of the initial game parameters to
 * their according values.
 * @return - a newly generated GameView struct
 */
GameView ConstructGameView() {
  GameView new = malloc(sizeof(*new));
  CheckMallocSuccess(new, "Couldn't allocate space for GameView!\n");

  new->map = MapNew();
  new->gameScore = GAME_START_SCORE;
  new->turnNumber = 0;
  new->numberTraps = 0;
  new->roundVampirePlaced = -1;
  new->vampireLocation = NOWHERE;

  for (int i = 0; i < TRAIL_SIZE; ++i) {
    new->trapLocations[i] = NOWHERE;
  }

  // Create hunters
  for (int i = 0; i < NUM_PLAYERS - 1; ++i) {
    new->players[i] = CreatePlayer(i, GAME_START_HUNTER_LIFE_POINTS);
  }
  // Create dracula
  new->players[PLAYER_DRACULA] =
      CreatePlayer(PLAYER_DRACULA, GAME_START_BLOOD_POINTS);

  return new;
}

/**
 * Handle the process of clearing traps and mature vampires
 * @param gameView
 */
void ProcessTraps(GameView gameView) {
  int roundNumber = gameView->turnNumber / 5;

  // Remove trap
  gameView->trapLocations[roundNumber % TRAIL_SIZE] = NOWHERE;

  // Handle maturing vampire
  if (gameView->vampireLocation != NOWHERE) {
    if (roundNumber - gameView->roundVampirePlaced == TRAIL_SIZE) {
      gameView->gameScore -= SCORE_LOSS_VAMPIRE_MATURES;
      gameView->vampireLocation = NOWHERE;
      gameView->roundVampirePlaced = -1;
    }
  }
}

/**
 * Handle and process encounter events
 * @param encounter - type of encounter encoded into a single char.
 * @return - value represents if the player killed in encounter
 */
bool ProcessEncounter(GameView gameView, Player player, char encounter) {
  /**
   * Clear traps/vampire
   */
  if (encounter == '.') return false;
  if (encounter == TRAP_ENCOUNTER) {
    gameView->players[player]->playerHealth -= LIFE_LOSS_TRAP_ENCOUNTER;
    PlaceId currentPlayerLocation =
        gameView->players[player]->lastResolvedLocation;
    for (int i = 0; i < TRAIL_SIZE; ++i) {
      if (gameView->trapLocations[i] == currentPlayerLocation) {
        gameView->trapLocations[i] = NOWHERE;
        break;
      }
    }
  } else if (encounter == VAMPIRE_ENCOUNTER) {
    gameView->roundVampirePlaced = -1;
    gameView->vampireLocation = NOWHERE;
  } else {  // Dracula Encounter
    gameView->players[PLAYER_DRACULA]->playerHealth -=
        LIFE_LOSS_HUNTER_ENCOUNTER;
    gameView->players[player]->playerHealth -= LIFE_LOSS_DRACULA_ENCOUNTER;
  }
  return gameView->players[player]->playerHealth <= 0;
}

PlaceId ResolveLocation(GameView gameView, PlayerDetails player,
                        PlaceId unresolvedLocation) {
  if (player->player != PLAYER_DRACULA) return unresolvedLocation;
  if (unresolvedLocation >= DOUBLE_BACK_1 &&
      unresolvedLocation <= DOUBLE_BACK_5) {
    int resolvedIndex =
        player->moveCount - 1 - (unresolvedLocation - DOUBLE_BACK_1);
    return player->resolvedMoves[resolvedIndex];
  } else if (unresolvedLocation == TELEPORT) {
    return CASTLE_DRACULA;
  } else if (unresolvedLocation == HIDE) {
    return player->resolvedMoves[player->moveCount - 1];
  }
  return unresolvedLocation;
}

/**
 * Process the current location of the player
 * @param gameView
 * @param placeAbbrev[3] - placeAbbrev[0] - first character of location
 *                         placeAbbrev[1] - second character of location
 *                         placeAbbrev[2] - null terminator
 */
void ProcessLocation(GameView gameView, Player player, char placeAbbrev[3]) {
  PlaceId placeId = placeAbbrevToId(placeAbbrev);

  PlayerDetails playerDetails = gameView->players[player];
  int moveCount = playerDetails->moveCount;

  // Resolve special cases to find actual location
  PlaceId resolvedId = ResolveLocation(gameView, playerDetails, placeId);

  int roundNumber = gameView->turnNumber / 5;

  /**
   * Find player type; handle sea damage for dracula
   * Handle castle dracula health gain +10
   *
   * Hunter rest -> gain +3 health
   *
   * Placement of traps (every city unknown) + vampires
   */

  if (player == PLAYER_DRACULA) {
    if (placeIsSea(resolvedId)) {
      playerDetails->playerHealth -= LIFE_LOSS_SEA;
    } else {
      // Else must be city
      if (roundNumber % VAMPIRE_PLACE_ROUNDS == 0) {
        // Place immature vampire
        gameView->vampireLocation = resolvedId;
        gameView->roundVampirePlaced = roundNumber;
      } else {
        // Place trap
        gameView->trapLocations[roundNumber % TRAIL_SIZE] = resolvedId;
      }

      if (resolvedId == CASTLE_DRACULA) {
        playerDetails->playerHealth += LIFE_GAIN_CASTLE_DRACULA;
      }
    }
  } else {
    // Avoid branching by evaluation the conditional only
    bool hasRested = playerDetails->lastResolvedLocation ==
                     playerDetails->resolvedMoves[playerDetails->moveCount - 2];
    playerDetails->playerHealth =
        min(GAME_START_HUNTER_LIFE_POINTS,
            playerDetails->playerHealth + LIFE_GAIN_REST * hasRested);
  }

  // Update player histories
  playerDetails->moves[moveCount] = placeId;
  playerDetails->resolvedMoves[moveCount] = resolvedId;
  playerDetails->lastResolvedLocation = resolvedId;
  playerDetails->moveCount++;
}

GameView GvProcessMoves(GameView gameView, char *pastPlays, Message messages[]) {
  char placeAbbrev[3] = {0};
  char cur = pastPlays[0];

  int playStringTurnNumber = 0;

  while (cur != '\0') {
    Player player = gameView->turnNumber % NUM_PLAYERS;

    if (player == PLAYER_DRACULA) {
      // On Dracula's turn
      ProcessTraps(gameView);
    } else if (gameView->players[player]->isDead) {
      gameView->players[player]->playerHealth = GAME_START_HUNTER_LIFE_POINTS;
      gameView->players[player]->isDead = false;
    }

    int charIndex = playStringTurnNumber * (PLAY_STR_LENGTH + 1);

    /** Offsets from charIndex are:
      0 -> player
      1-2 -> place
      3-6 -> encounters
    */
    placeAbbrev[0] = pastPlays[charIndex + 1];
    placeAbbrev[1] = pastPlays[charIndex + 2];
    ProcessLocation(gameView, player, placeAbbrev);

    // Process turn encounters
    if (player != PLAYER_DRACULA && placeIsLand(placeAbbrevToId(placeAbbrev))) {
      int encounterIndex = charIndex + 3;
      while (encounterIndex < charIndex + PLAY_STR_LENGTH) {
        if (ProcessEncounter(gameView, player, pastPlays[encounterIndex])) {
          // Hunter has died
          gameView->players[player]->playerHealth = 0;
          gameView->players[player]->isDead = true;
          gameView->players[player]->lastResolvedLocation = HOSPITAL_PLACE;
          gameView->gameScore -= SCORE_LOSS_HUNTER_HOSPITAL;
          break;
        }
        ++encounterIndex;
      }
    }

    gameView->gameScore -= (player == PLAYER_DRACULA) * 1;
    playStringTurnNumber++;
    gameView->turnNumber++;
    cur = pastPlays[playStringTurnNumber * 8 - 1];
  }
  return gameView;
}

GameView GvNew(char *pastPlays, Message messages[]) {
  GameView gameView = ConstructGameView();
  GvProcessMoves(gameView, pastPlays, messages);
  return gameView;
}

void GvFree(GameView gameView) {
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    FreePlayer(gameView->players[i]);
  }
  MapFree(gameView->map);
  free(gameView);
}

////////////////////////////////////////////////////////////////////////
// Game State Information
//
// The code for hese functions is quite straight-forward. They only
// act as 'getters'.

Round GvGetRound(GameView gv) {
  assert(gv != NULL);
  return gv->turnNumber / 5;
}

Player GvGetPlayer(GameView gv) {
  assert(gv != NULL);
  return gv->turnNumber % NUM_PLAYERS;
}

int GvGetScore(GameView gv) {
  assert(gv != NULL);
  assert(gv->gameScore <= GAME_START_SCORE);
  return gv->gameScore;
}

int GvGetHealth(GameView gv, Player player) {
  assert(gv != NULL);
  return gv->players[player]->playerHealth;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player) {
  assert(gv != NULL);
  return gv->players[player]->lastResolvedLocation;
}

PlaceId GvGetVampireLocation(GameView gv) {
  assert(gv != NULL);
  return gv->vampireLocation;
}

// loop through gv trap locations and look for all the values in the array
// without a value of NOWHERE
PlaceId *GvGetTrapLocations(GameView gv, int *numTraps) {
  *numTraps = 0;
  for (int i = 0; i < TRAIL_SIZE; i++) {
    if (gv->trapLocations[i] != NOWHERE) {
      (*numTraps)++;
    }
  }
  int count = 0;
  PlaceId *traps = malloc(sizeof(PlaceId) * (*numTraps));
  CheckMallocSuccess(traps, "Couldn't allocate space for traps!\n");
  for (int i = 0; i < TRAIL_SIZE; i++) {
    if (gv->trapLocations[i] != NOWHERE) {
      traps[count] = gv->trapLocations[i];
      count++;
    }
  }
  return traps;
}

////////////////////////////////////////////////////////////////////////
// Game History
PlaceId *GvGetMoveHistory(GameView gv, Player player, int *numReturnedMoves,
                          bool *canFree) {
  PlayerDetails currPlayer = gv->players[player];
  *numReturnedMoves = currPlayer->moveCount;
  // should be set to true if using a copy of the original array.
  *canFree = true;

  PlaceId *moveHistory = malloc(sizeof(PlaceId) * currPlayer->moveCount);
  CheckMallocSuccess(moveHistory,
                     "Couldn't allocate space for move history!\n");
  memcpy(moveHistory, currPlayer->moves,
         sizeof(PlaceId) * currPlayer->moveCount);

  return moveHistory;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree) {
  PlayerDetails currPlayer = gv->players[player];
  *numReturnedMoves = currPlayer->moveCount;
  *canFree = false;

  if (numMoves > currPlayer->moveCount) {
    numMoves = currPlayer->moveCount;
  }

  *numReturnedMoves = numMoves;

  PlaceId *nLastMoves = malloc(sizeof(PlaceId) * numMoves);
  CheckMallocSuccess(nLastMoves, "Couldn't allocate space for move history!\n");

    for (int i = 0; i < numMoves; i++) {
        nLastMoves[i] = currPlayer->moves[currPlayer->moveCount - i - 1];
    }

  return nLastMoves;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player, int *numReturnedLocs,
                              bool *canFree) {
  PlayerDetails currPlayer = gv->players[player];
  *numReturnedLocs = currPlayer->moveCount;
  // should be set to true if using a copy of the original array.
  *canFree = true;

  PlaceId *locationHistory = malloc(sizeof(PlaceId *) * currPlayer->moveCount);
  CheckMallocSuccess(locationHistory,
                     "Couldn't allocate for location history!\n");

  memcpy(locationHistory, currPlayer->resolvedMoves,
         sizeof(PlaceId) * currPlayer->moveCount);

  return locationHistory;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree) {
  PlayerDetails currPlayer = gv->players[player];
  *numReturnedLocs = currPlayer->moveCount;
  *canFree = false;

  if (numLocs > currPlayer->moveCount) {
    numLocs = currPlayer->moveCount;
  }

  *numReturnedLocs = numLocs;
  PlaceId *nLastLocations = malloc(sizeof(PlaceId *) * numLocs);
  CheckMallocSuccess(nLastLocations,
                     "Couldn't allocate space for last locations!\n");

  for (int i = 0; i < numLocs; i++) {
    nLastLocations[i] = currPlayer->resolvedMoves[currPlayer->moveCount - i - 1];
  }

  return nLastLocations;
}

////////////////////////////////////////////////////////////////////////
// Making a Move
PlaceId *GvGetReachable(GameView gv, Player player, Round round, PlaceId from,
                        int *numReturnedLocs) {
  *numReturnedLocs = 0;
  return GetPossibleMoves(gv, gv->map, player, from, true, true, true, round,
                          numReturnedLocs, true, false);
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail, bool boat,
                              int *numReturnedLocs) {
  *numReturnedLocs = 0;
  return GetPossibleMoves(gv, gv->map, player, from, road, rail, boat, round,
                          numReturnedLocs, true, false);
}

////////////////////////////////////////////////////////////////////////
// OUR own interface functions
// Eric | Lucas | Stephen | Debbie - (20T2)

Map GvGetMap(GameView gameView) {
  assert(gameView != NULL);
  return gameView->map;
}

PlayerDetails *GetPlayerDetailsArray(GameView gameView) {
  assert(gameView != NULL);
  return gameView->players;
}

int GvGetTurnNumber(GameView gameView) {
  assert(gameView != NULL);
  return gameView->turnNumber;
}

PlaceId GvGetExpiringTrap(GameView gameView) {
  return gameView->trapLocations[(gameView->turnNumber / NUM_PLAYERS) % TRAIL_SIZE];
}

bool GvIsVampireMaturing(GameView gameView) {
  FILE *draculaLog = fopen("dracula.log", "a");
  fprintf(draculaLog, "Round Placed (%d)\n", gameView->roundVampirePlaced);
  fprintf(draculaLog, "GvIsVampireMaturing (%d)\n", (gameView->turnNumber / NUM_PLAYERS) - gameView->roundVampirePlaced);
  fclose(draculaLog);
  if (gameView->roundVampirePlaced == -1) return false;
  return ((gameView->turnNumber / NUM_PLAYERS) - gameView->roundVampirePlaced == TRAIL_SIZE);
}

GameView GvClone(GameView state) {
  GameView clone = ConstructGameView();
  clone->gameScore = state->gameScore;
  clone->roundVampirePlaced = state->roundVampirePlaced;
  clone->vampireLocation = state->vampireLocation;
  clone->turnNumber = state->turnNumber;
  clone->map = MapNew();
  memcpy(clone->trapLocations, state->trapLocations, 6);

  for (int i = 0; i < NUM_PLAYERS; i++) {
    // Clone player
    PlayerDetails player = state->players[i];
    PlayerDetails playerClone = CreatePlayer(i, player->playerHealth);
    playerClone->lastResolvedLocation = player->lastResolvedLocation;
    playerClone->moveCount = player->moveCount;
    playerClone->isDead = player->isDead;
    memcpy(playerClone->resolvedMoves, player->resolvedMoves, GAME_START_SCORE);
    memcpy(playerClone->moves, player->moves, GAME_START_SCORE);
    clone->players[i] = playerClone;
  }
  return clone;
}