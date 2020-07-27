//
// Created by eric on 17/7/20.
//

#ifndef FURY_OF_DRACULA_PATH_FINDING_H
#define FURY_OF_DRACULA_PATH_FINDING_H

#include "Places.h"
#include "hash_table.h"
#include "binary_heap.h"
#include "Map.h"
#include "GameView.h"

typedef struct path {
    int distance;
    char *place;
    struct path *predecessor;
} *Path;

/**
 * Gets a path lookup table which has distances and paths from the given place
 * to any other place on the map
 * @param map
 * @param from
 * @param end
 * @return HashTable containing all computed distances to places (place abbrev)
 */
HashTable GetPathLookupTableFrom(GameView gameView, Map map, Player player, Place from, bool road, bool rail, bool boat,
                                 int round, bool resolveMoves, bool applyTrailRestrictions);

/**
 * Gets a path lookup table which has distances and paths from any
 * place to any other place on the map
 * @param map
 * @return
 */
HashTable* GetAllPathLookup(Map map);

/**
 * Creates a dynamically allocated path node and returns it
 * @param place
 * @param distance
 * @param predecessor
 * @return
 */
Path CreatePath(char *place, int distance, Path predecessor);

/**
 * Frees all memory allocated to a given path
 * @param path
 */
void FreePathNode(Path path);

/**
 * Prints the path sequence of the given path to stdout
 * @param path
 */
void PrintPathSequence(Path path);

/**
 * Gets an array of length path->distance + 1
 * @param path
 * @return
 */
Path* GetOrderedPathSequence(Path path);

/**
 * Gets an array of PlaceId's in order from given path
 * @param path
 * @return
 */
PlaceId* GetOrderedPlaceIds(Path path);

/**
 * Gets reachable places in move given travel type restirctions from given PlaceId
 * @param map
 * @param round
 * @param currentId
 * @param placesCount
 * @param road
 * @param boat
 * @return array of PlaceId for reachable locations
 */
PlaceId *
GetPossibleMoves(GameView gameView, Map map, Player player, PlaceId currentId, bool road, bool rail, bool boat,
                 int round, int *placesCount, bool resolveMoves, bool applyTrailRestrictions);

#endif //FURY_OF_DRACULA_PATH_FINDING_H
