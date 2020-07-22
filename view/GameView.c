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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include "Players.h"

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

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView ConstructGameView() {
    GameView new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Couldn't allocate GameView!\n");
        exit(EXIT_FAILURE);
    }

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
    new->players[PLAYER_DRACULA] = CreatePlayer(PLAYER_DRACULA, GAME_START_BLOOD_POINTS);

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
 * Handle encounter events, guaranteed to be an encounter and not '.'
 * @param gameView
 * @return was player killed in encounter
 */
int ProcessEncounter(GameView gameView, Player player, char encounter) {
    /**
     * Clear traps/vampire
     */
    if (encounter == TRAP_ENCOUNTER) {
        gameView->players[player]->playerHealth -= LIFE_LOSS_TRAP_ENCOUNTER;
        PlaceId currentPlayerLocation = gameView->players[player]->lastResolvedLocation;
        for (int i = 0; i < TRAIL_SIZE; ++i) {
            if (gameView->trapLocations[i] == currentPlayerLocation) {
                gameView->trapLocations[i] = NOWHERE;
                break;
            }
        }
    } else if (encounter == VAMPIRE_ENCOUNTER) {
        gameView->roundVampirePlaced = -1;
        gameView->vampireLocation = NOWHERE;
    } else { // Dracula
        gameView->players[PLAYER_DRACULA]->playerHealth -= LIFE_LOSS_HUNTER_ENCOUNTER;
        gameView->players[player]->playerHealth -= LIFE_LOSS_DRACULA_ENCOUNTER;
    }
    return gameView->players[player] <= 0;
}

PlaceId ResolveLocation(GameView gameView, PlayerDetails player, PlaceId unresolvedLocation) {
    if (player->player != PLAYER_DRACULA) return unresolvedLocation;
    if (unresolvedLocation >= HIDE && unresolvedLocation <= DOUBLE_BACK_5) {
        int resolvedIndex = player->moveCount - (unresolvedLocation - HIDE);
        return player->moves[resolvedIndex];
    } else if (unresolvedLocation == TELEPORT) {
        return CASTLE_DRACULA;
    }
    return unresolvedLocation;
}

/**
 * Handle location of move where a is first char b is second char
 * @param gameView
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

            if (placeId == CASTLE_DRACULA) {
                playerDetails->playerHealth += LIFE_GAIN_CASTLE_DRACULA;
            }
        }
    } else {
        // Avoid branching by evaluation the conditional only
        bool hasRested = playerDetails->lastResolvedLocation
                         == playerDetails->resolvedMoves[playerDetails->moveCount - 2];
        playerDetails->playerHealth += (LIFE_GAIN_REST * hasRested);
    }

    //Update player histories
    playerDetails->moves[moveCount] = placeId;
    playerDetails->resolvedMoves[moveCount] = resolvedId;
    playerDetails->moveCount++;
}

GameView GvNew(char *pastPlays, Message messages[]) {
    //
    GameView gameView = ConstructGameView();
    char placeAbbrev[3] = {0};
    char cur = pastPlays[0];
    while (cur != '\0') {
        Player player = gameView->turnNumber % NUM_PLAYERS;

        if (player == PLAYER_DRACULA) {
            // On Dracula's turn
            ProcessTraps(gameView);
        }


        int charIndex = gameView->turnNumber * (PLAY_STR_LENGTH + 1);

        /** Offsets from charIndex are:
            0 -> player
            1-2 -> place
            3-6 -> encounters
        */
        placeAbbrev[0] = pastPlays[charIndex + 1];
        placeAbbrev[1] = pastPlays[charIndex + 2];
        ProcessLocation(gameView, player, placeAbbrev);

        // Process turn encounters -> might need to check in specific order if not ordered in play string
        int encounterIndex = charIndex + 3;
        while (encounterIndex < charIndex + PLAY_STR_LENGTH) {
            if (!ProcessEncounter(gameView, player, pastPlays[encounterIndex])) {
                break;
            }
            ++encounterIndex;
        }

        gameView->gameScore -= (player == PLAYER_DRACULA) * 1;

        gameView->turnNumber++;
        cur = pastPlays[gameView->turnNumber * 8 - 1];
    }

    printf("Score: %d", gameView->gameScore);
    return gameView;
}

void GvFree(GameView gameView) {
    for (Player i = 0; i < NUM_PLAYERS; ++i) {
        FreePlayer(gameView->players[i]);
    }

    free(gameView->trapLocations);
    free(gameView);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

Player GvGetPlayer(GameView gv) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return PLAYER_LORD_GODALMING;
}

int GvGetScore(GameView gv) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

int GvGetHealth(GameView gv, Player player) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numTraps = 0;
    return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // use void *memcpy(void *dest, const void * src, size_t n)
    // to copy over
    *numReturnedMoves = 0;
    *canFree = false;
    return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedMoves = 0;
    *canFree = false;
    return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedLocs = 0;
    *canFree = false;
    return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedLocs = 0;
    *canFree = false;
    return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedLocs = 0;
    return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numReturnedLocs = 0;
    return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
