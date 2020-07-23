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

struct Vampire {
    int vampirelife;
    int vampire_has_matured;
    int dracula_revealed;
};

struct gameView {

    Map map;
    PlayerDetails players[NUM_PLAYERS];
    int gameScore;
    int turnNumber;
    int numberTraps;
    PlaceId trapLocations[TRAIL_SIZE];
    PlaceId vampireLocation;
    int roundVampirePlaced;
    Vampire vamps;
    

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
 * Handle encounter events
 * @param gameView
 * @return was player killed in encounter
 */
bool ProcessEncounter(GameView gameView, Player player, char encounter) {
    /**
     * Clear traps/vampire
     */
     if (encounter == '.') return false;
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
    playerDetails->lastResolvedLocation = resolvedId;
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
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        PrintPlayer(gameView->players[i]);
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

Round GvGetRound(GameView gv)
{
	assert (gv != NULL);	
    return gv->turnNumber;
}

Player GvGetPlayer(GameView gv)
{
	assert (gv != NULL);
	return gv->turnNumber % NUM_PLAYERS;
}

int GvGetScore(GameView gv)
{	
	assert (gv != NULL);
    assert (gv->gameScore <= 366);
	return gv->gameScore;
}

int GvGetHealth(GameView gv, Player player)
{	
	assert (gv != NULL);

	if (player >= NUM_PLAYERS || player < 0) {

		return -1;

	} else if (player == PLAYER_DRACULA) {

		assert (gv->players[player]->playerHealth > 0);
	
	} else {
        
        assert (gv->players[player]->playerHealth >= 0 &&
                gv->players[player]->playerHealth <= 9);
    }
    return gv->players[player]->playerHealth;	
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	gv->players[player]->lastResolvedLocation;
}

/*not sure if needed
PlaceId playerlocation(char *pastPlays, int array_num) {
	PlaceId place;
	char player_loc[3] = {};

	strcpy(player_loc, &pastPlays[array_num], 2);
	player_loc[2] = '\0';

	place = placeAbbrevToId(player_loc);

	if (place == UNKNOWN_PLACE) {
		place = DraculaLocation(player_loc);
	}
		return place;
}

PlaceId DraculaLocation(char *place) {
	int plc = UNKNOWN_PLACE;
	switch (place) {
		case "C?":
			plc = CITY_UNKNOWN;
			break;
		case "S?":
			plc = SEA_UNKNOWN;
			break;
		case "HI":
			plc = HIDE;
			break;
		case "D1":
			plc = DOUBLE_BACK_1;
			break;
		case "D2":
			plc = DOUBLE_BACK_2;
			break;
		case "D3":
			plc = DOUBLE_BACK_3;
			break;
		case "D4":
			plc = DOUBLE_BACK_4;
			break;
		case "D5":
			plc = DOUBLE_BACK_5;
			break;
		case "TP":
			plc = TELEPORT;
	}

	return plc;
}
*/

// does GvGetPlayerLocation get the current location? or new function tbm?
PlaceId GvSetVampire(GameView gv) {
    int vampround = GvGetRound(gv);
    PlaceId vamplocation = GvGetPlayerLocation(gv, PLAYER_DRACULA);
    
    if (vampround % 13 == 0 || vampround == 1) {
        // aims to check with the current location of player who is dracula
        if (placeIdToType(GvGetPlayerLocation(gv, PLAYER_DRACULA) != SEA)) {
            gv->vampireLocation = vamplocation;
            gv->vamps.vampirelife =  true;
            return vamplocation;
        }
    }
    return NOWHERE;
}

// Reveals the dracula and assigns roles
// function to check if the vampire has been vanquished
static bool GvSetVampireDead(Gameview gv, GvGetPlayer(gv)) {
    Vampire vamp_buddy;
    // if the hunters and the vampire are in the same location,
    //the vampire is dead}
    if (GvGetPlayer(gv) != PLAYER_DRACULA) {
        if (gv->players[GvGetPlayer(gv)].lastResolvedLocation == gv->vampireLocation) {
            vamp_buddy->vampirelife = false;
            return false;
        }
    } else {
        return true;
    }
    

PlaceId GvGetVampireLocation(GameView gv)
{
	PlaceId vamplocation = GvSetVampire(gv);
    Vampire vamp_buddy = gv->vamps;
    if (gv->GvSetVampireDead(gv, GvGetPlayer(gv)) == false) {
        return NOWHERE;
    }
    else if (vamplocation == NOWHERE) {
        return NOWHERE;
    }
    else if (vamp_buddy->vampire_has_matured == true) {
        return NOWHERE;
    }
    else if (vamp_buddy.dracula_revealed == true) {
        return gv->vampireLocation;
    }
    else {
        return vamplocation;
    }
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
