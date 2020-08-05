########################################################################
# COMP2521 20T2 // the Fury of Dracula // the Hunt
# hunt/Makefile: build your Dracula and Hunter AIs
#
# 2018-12-31	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
# 2020-07-10	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

CC = gcc
CFLAGS = -Wall -Werror -O3

# do not change the following line
BINS = dracula hunter

# add any other *.o files that your solution requires
# (and add their dependencies below after DraculaView.o)
# if you're not using Map.o or Places.o, you can remove them
OBJS = GameView.o Map.o Places.o PathFinding.o BinaryHeap.o HashTable.o Queue.o Players.o Utilities.o minimax.o

# add whatever system libraries you need here (e.g. -lm)
LIBS = -lm

all: $(BINS)

dracula: playerDracula.o dracula.o DraculaView.o $(OBJS) $(LIBS)
hunter: playerHunter.o hunter.o HunterView.o $(OBJS) $(LIBS)

playerDracula.o: player.c dracula.h Game.h DraculaView.h GameView.h Places.h 
	$(CC) $(CFLAGS) -DI_AM_DRACULA -c $< -o $@
playerHunter.o: player.c hunter.h Game.h HunterView.h GameView.h Places.h 
	$(CC) $(CFLAGS) -c $< -o $@

dracula.o: dracula.c dracula.h DraculaView.h GameView.h Places.h Game.h
hunter.o: hunter.c hunter.h HunterView.h GameView.h Places.h Game.h

Places.o: Places.c Places.h Game.h
Map.o: Map.c Map.h Places.h Game.h BinaryHeap.h
GameView.o:	GameView.c GameView.h Game.h PathFinding.h Players.h Utilities.h
HunterView.o: HunterView.c HunterView.h Game.h
DraculaView.o: DraculaView.c DraculaView.h Game.h
BinaryHeap.o: BinaryHeap.c BinaryHeap.h
HashTable.o: HashTable.c HashTable.h
PathFinding.o: PathFinding.c PathFinding.h BinaryHeap.h HashTable.h Queue.h Utilities.h
Players.o: Players.c Players.h
Queue.o: Queue.c Queue.h
Utilities.o: Utilities.c Utilities.h
minimax.o: minimax.c minimax.h Utilities.h


# if you use other ADTs, add dependencies for them here
BinaryHeap.o: BinaryHeap.c BinaryHeap.h
HashTable.o: HashTable.c HashTable.h
Queue.o: Queue.c Queue.h

.PHONY: clean
clean:
	-rm -f $(BINS) *.o core
