//
// Created by eric on 28/7/20.
//

#ifndef FURY_OF_DRACULA_HUNT_MINIMAX_H
#define FURY_OF_DRACULA_HUNT_MINIMAX_H

#include "GameView.h"
#include "HashTable.h"
#include "PathFinding.h"

int MiniMax(GameView state, Path **distanceLookup, int depth, int alpha, int beta);

#endif //FURY_OF_DRACULA_HUNT_MINIMAX_H
