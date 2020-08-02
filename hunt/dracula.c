////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "dracula.h"

#include <pthread.h>
#include <time.h>
#include <stdio.h>

#include "DraculaView.h"
#include "Game.h"

void DvMakeFirstMove(DraculaView hv) {
  int place = rand() % 23;
  printf("First: %s\n", placeIdToName(place));
  registerBestPlay((char*)PLACES[place].abbrev,
                   "Have we nothing Toulouse?");
}

void DvMakeRandomMove(DraculaView hv) {
  int numMoves = 0;
  PlaceId* possibleMoves = DvGetValidMoves(hv, &numMoves);
  if (numMoves == 0) {
    FILE *draculaLog = fopen("dracula.log", "a");
    fprintf(draculaLog, "Move: %s\n", placeIdToName(TELEPORT));
    fclose(draculaLog);
    registerBestPlay((char*)placeIdToAbbrev(TELEPORT), "");
  } else {
    int moveIndex = rand() % numMoves;
    int placeId = possibleMoves[moveIndex];
    FILE *draculaLog = fopen("dracula.log", "a");
    fprintf(draculaLog, "Move: %s\n", placeIdToName(placeId));
    fclose(draculaLog);

    registerBestPlay((char*)placeIdToAbbrev(placeId),
                     "Have we nothing Toulouse?");
  };
}

void decideDraculaMove(void* dv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  DraculaView view = (DraculaView)dv;
  srand ( time(NULL) );


  if (DvGetRound(view) == 0) {
    DvMakeFirstMove(view);
  } else {
    if (view) DvMakeRandomMove(view);
  }
}
