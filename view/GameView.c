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
// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct gameView {
	Map map;
	PlayerDetails players[NUM_PLAYERS];
	int gameScore;
	int turnNumber;
	int numberTraps;
	Place *trapLocations;
	Place playerlocation[NUM_PLAYERS];
    Place vampireLocation;
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
    new->trapLocations = NULL;

    // Create hunters
    for (int i = 0; i < NUM_PLAYERS - 1; ++i) {
        new->players[i] = CreatePlayer(i, GAME_START_HUNTER_LIFE_POINTS);
    }
    // Create dracula
    new->players[PLAYER_DRACULA] = CreatePlayer(PLAYER_DRACULA, GAME_START_BLOOD_POINTS);

    return new;
}

/**
 * Handle encounter events, guaranteed to be an encounter and not '.'
 * @param gameView
 */
void ProcessEncounter(GameView gameView, Player player, char encounter) {

}

/**
 * Handle location of move where a is first char b is second char
 * @param gameView
 */
void ProcessLocation(GameView gameView, Player player, char placeAbbrev[3]) {
    PlaceId placeId = placeAbbrevToId(placeAbbrev);
}

GameView GvNew(char *pastPlays, Message messages[])
{
	//
	GameView gameView = ConstructGameView();
    char placeAbbrev[3] = {0};
	char cur = pastPlays[0];
	while (cur != '\0') {
	    int charIndex = gameView->turnNumber * 8;
	    Player player = gameView->turnNumber % NUM_PLAYERS;
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
	    while (pastPlays[encounterIndex] != '.' && encounterIndex < charIndex + 7) {
            ProcessEncounter(gameView, player, pastPlays[encounterIndex]);
	        ++encounterIndex;
	    }

        gameView->turnNumber++;
        cur = pastPlays[gameView->turnNumber * 8 - 1];
	}
	return gameView;
}

void GvFree(GameView gameView)
{
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
	return gv->gameScore;
}

int GvGetHealth(GameView gv, Player player)
{	
	assert (gv != NULL);

	if (player >= NUM_PLAYERS || player < 0) {

		return -1;

	} else if (player == PLAYER_DRACULA) {

		return gv->players[PLAYER_DRACULA]->playerHealth;
	
	} else {

		return gv->players[player]->playerHealth;
	}
	
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	return gv->playerlocation[player];
}

/*not sure if needed
PlaceId playerlocation(char *pastPlays, int array_num) {
	PlaceId place;
	char player_loc[3] = {};

	strcpy(player_loc, &pastPlays[array_num], 2);
	player_loc[2] = '\0git ';

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


PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
