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

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "Game.h"
#include "hunter.h"
#include "HunterView.h"

void decideHunterMove(void* hv)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	HunterView view = (HunterView) hv;
	registerBestPlay("TO", "Have we nothing Toulouse?");
    time_t start = clock();
    while ((((double) (clock() - start)) / CLOCKS_PER_SEC) < ((double) 2)) {}
    printf("Run\n\n");
    registerBestPlay("SO", "Have we nothing Toulouse?");
}
