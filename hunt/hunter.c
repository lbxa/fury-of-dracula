////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "hunter.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"

#define PLAY_RANDOM

void makeFirstMove(HunterView hv) {
  registerBestPlay((char*)PLACES[rand() % NUM_REAL_PLACES].abbrev,
                   "Have we nothing Toulouse?");
}

void makeRandomMove(HunterView hv) {
  int numMoves = 0;
  PlaceId* possibleMoves = HvWhereCanIGo(hv, &numMoves);
  registerBestPlay((char*)placeIdToAbbrev(possibleMoves[rand() % numMoves]),
                   "Have we nothing Toulouse?");
}

void decideHunterMove(void* hv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//  srand ( GvGetPlayer(1) );
  HunterView view = (HunterView)hv;

  FILE *turnLog = fopen("turns.log", "a");
  fprintf(turnLog, "\nHunter Move (%d)\n", HvGetRound(view));
  fclose(turnLog);

  if (HvGetRound(view) == 0) {
    makeFirstMove(view);
  } else {
    if (view) makeRandomMove(view);
  }
}
