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
    int sequence_capacity;
    int sequence_length;
    char *place;
    PlaceId *sequence;
} *Path;

/**
 * Uses dijkstra's path-finding algorithm with priority queue
 * @param map
 * @param from
 * @param end
 * @return HashTable containing all computed distances to places (place abbrev)
 */
HashTable mapFindShortestPathsFrom(Map map, Place from);

HashTable* getAllPossiblePaths(Map map);

Path create_path(char *place, int distance);

void path_insert_place(Path path, PlaceId place);

void free_path_node(Path path);

#endif //FURY_OF_DRACULA_PATH_FINDING_H
