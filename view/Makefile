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
BINS = our_test testGameView testHunterView testDraculaView testMap

all: $(BINS)

our_test: our_test.o GameView.o Map.o Places.o path_finding.o binary_heap.o hash_table.o Queue.o
our_test.o: our_test.c GameView.h Map.h Places.h Game.h path_finding.h

testGameView: testGameView.o testUtils.o GameView.o Map.o Places.o path_finding.o binary_heap.o hash_table.o Queue.o
testGameView.o: testGameView.c GameView.h Map.h Places.h Game.h

testHunterView: testHunterView.o testUtils.o HunterView.o GameView.o Map.o Places.o path_finding.o binary_heap.o hash_table.o Queue.o
testHunterView.o: testHunterView.c HunterView.h GameView.h Map.h Places.h Game.h

testDraculaView: testDraculaView.o testUtils.o DraculaView.o GameView.o Map.o Places.o path_finding.o binary_heap.o hash_table.o Queue.o
testDraculaView.o: testDraculaView.c DraculaView.h GameView.h Map.h Places.h Game.h

testMap: testMap.o Map.o Places.o binary_heap.o hash_table.o
testMap.o: testMap.c Map.h Places.h

Places.o: Places.c Places.h Game.h
Map.o: Map.c Map.h Places.h Game.h binary_heap.h
GameView.o:	GameView.c GameView.h Game.h path_finding.h
HunterView.o: HunterView.c HunterView.h Game.h
DraculaView.o: DraculaView.c DraculaView.h Game.h
testUtils.o: testUtils.c Places.h Game.h
binary_heap.o: binary_heap.c binary_heap.h
hash_table.o: hash_table.c hash_table.h
path_finding.o: path_finding.c path_finding.h binary_heap.h hash_table.h Queue.h
Queue.o: Queue.c Queue.h

.PHONY: clean
clean:
	-rm -f ${BINS} *.o core
