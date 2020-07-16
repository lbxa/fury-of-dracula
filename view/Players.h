//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_PLAYERS_H
#define FURY_OF_DRACULA_PLAYERS_H

#include "Game.h"
#include "Places.h"

typedef struct playerDetails {
    Player player;
    int location_history_count;
    Place *location_history;
} PlayerDetails;


#endif //FURY_OF_DRACULA_PLAYERS_H
