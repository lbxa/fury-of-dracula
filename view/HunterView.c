////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
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
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
#include "path_finding.h"
// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct hunterView {
	GameView gameView;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	GameView gameView = GvNew(pastPlays, messages);
    new->gameView = gameView;
	

	// TODO: Add helper function to copy messages into messageList   

	return new;
}

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GvFree(hv->gameView);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	return GvGetRound(hv->gameView);
}

Player HvGetPlayer(HunterView hv)
{
	return GvGetPlayer(hv->gameView);
}

int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gameView);
}

int HvGetHealth(HunterView hv, Player player)
{
	return GvGetHealth(hv->gameView, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	return GvGetPlayerLocation(hv->gameView, player);
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return GvGetVampireLocation(hv->gameView);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// Dracula player struct, go backwards through his resolved location array
	// check between MIN_REAL_PLACE AND MAX_REAL_PLACE
	
	int *numReturnedMoves = 0;
	bool canModify = true;
	PlaceId *draculaHistory = GvGetMoveHistory(hv->gameView, PLAYER_DRACULA, numReturnedMoves, &canModify);
	

	for (int i = *numReturnedMoves; i >= 0; i--) {
		PlaceId currentMove = draculaHistory[i];
		if (currentMove >= MIN_REAL_PLACE && currentMove <= MAX_REAL_PLACE) {
			*round = i;
			return draculaHistory[i];
		}
	}
	*round = 0;
	return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    //	HashTable pathLookup = GetPathLookupTableFrom();
	// HashNode path = HashGet(pathLookup, "KL"); -> KL is dest
	*pathLength = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	Map map = GetMap(hv->gameView);
	Player currentPlayer = GvGetPlayer(hv->gameView);
	PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, currentPlayer);
	int currentRound = GvGetRound(hv->gameView);

	return GetPossibleMoves(hv->gameView, map, currentPlayer, currentLocation, true, true, true, currentRound, numReturnedLocs, true, false);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	Map map = GetMap(hv->gameView);
	PlaceId currentLocation = GvGetPlayerLocation(hv->gameView, player);
	int currentRound = GvGetRound(hv->gameView);
	return GetPossibleMoves(hv->gameView, map, player, currentLocation, true, true, true, currentRound, numReturnedLocs, true, false);
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
