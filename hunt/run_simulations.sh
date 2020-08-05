#!/bin/bash

numSimulations=5

rm ./logs/*

for i in $( eval echo {1..$numSimulations} )
do
  echo "Running: $i"
  /web/cs2521/20T2/ass/ass2/hunt/dryrun/rungame.sh Makefile BinaryHeap.c DraculaView.c Game.c GameView.c HashTable.c HunterView.c PathFinding.c Players.c Queue.c Utilities.c dracula.c hunter.c minimax.c BinaryHeap.h DraculaView.h GameView.h HashTable.h HunterView.h PathFinding.h Players.h Queue.h Utilities.h minimax.h | tee logs/"$i".log 1>/dev/null
done