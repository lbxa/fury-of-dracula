########################################################################
# COMP2521 20T2 // the Fury of Dracula // the View
# view/Makefile: build tests for GameView/HunterView/DraculaView
#
# You can modify this if you add additional files
#
# 2018-12-31	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
# 2020-07-10    v2.0    Team Dracula <cs2521@cse.unsw.edu.au>
#
########################################################################

CC = gcc
CFLAGS = -Wall -Werror -g
BINS = OurTest testGameView testHunterView testDraculaView testMap

all: $(BINS)

OurTest: OurTest.o GameView.o Map.o Places.o PathFinding.o BinaryHeap.o HashTable.o Queue.o Players.o Utilities.o
OurTest.o: OurTest.c GameView.h Map.h Places.h Game.h PathFinding.h

testGameView: testGameView.o testUtils.o GameView.o Map.o Places.o PathFinding.o BinaryHeap.o HashTable.o Queue.o Players.o Utilities.o
testGameView.o: testGameView.c GameView.h Map.h Places.h Game.h

testHunterView: testHunterView.o testUtils.o HunterView.o GameView.o Map.o Places.o PathFinding.o BinaryHeap.o HashTable.o Queue.o Players.o Utilities.o
testHunterView.o: testHunterView.c HunterView.h GameView.h Map.h Places.h Game.h

testDraculaView: testDraculaView.o testUtils.o DraculaView.o GameView.o Map.o Places.o PathFinding.o BinaryHeap.o HashTable.o Queue.o Players.o Utilities.o
testDraculaView.o: testDraculaView.c DraculaView.h GameView.h Map.h Places.h Game.h

testMap: testMap.o Map.o Places.o BinaryHeap.o HashTable.o Utilities.o
testMap.o: testMap.c Map.h Places.h

Places.o: Places.c Places.h Game.h
Map.o: Map.c Map.h Places.h Game.h BinaryHeap.h
GameView.o:	GameView.c GameView.h Game.h PathFinding.h Players.h
HunterView.o: HunterView.c HunterView.h Game.h
DraculaView.o: DraculaView.c DraculaView.h Game.h
testUtils.o: testUtils.c Places.h Game.h
BinaryHeap.o: BinaryHeap.c BinaryHeap.h
HashTable.o: HashTable.c HashTable.h
PathFinding.o: PathFinding.c PathFinding.h BinaryHeap.h HashTable.h Queue.h Utilities.h
Players.o: Players.c Players.h
Queue.o: Queue.c Queue.h
Utilities.o: Utilities.c Utilities.h

.PHONY: clean
clean:
	-rm -f ${BINS} *.o core
