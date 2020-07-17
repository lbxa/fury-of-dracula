//
// Created by eric on 17/7/20.
//

#ifndef FURY_OF_DRACULA_PATH_FINDING_H
#define FURY_OF_DRACULA_PATH_FINDING_H

#include "Places.h"
#include "hash_table.h"
#include "binary_heap.h"
#include "Map.h"

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
HashTable GetPathLookupTableFrom(Map map, Place from);

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

#endif //FURY_OF_DRACULA_PATH_FINDING_H
