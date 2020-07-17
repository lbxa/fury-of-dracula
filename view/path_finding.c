//
// Created by eric on 17/7/20.
//

#include "path_finding.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////
// PATH FINDING
// Full lookup if need to recreate each turn will entail creating 4 lookups for each starting initial state of having
// 0, 1, 2, 3 rail moves + a lookup for no rail moves at any time (dracula)

/**
 * Gets all reachable places in a single move for a player
 * @return
 */
PlaceId *get_reachable_places_in_move(Map map, int distance_by_rail, PlaceId currentId, int *places_count) {
    // For now only return immediate connections no special regard for rail

    // TODO: This function will need to determine additional reachable locations based on distance that can be travelled by rail

    ConnList connections = MapGetConnections(map, currentId);

    *places_count = 0;
    int places_allocation = 16;
    // Initial size of 16
    PlaceId *places = malloc(sizeof(PlaceId) * places_allocation);

    ConnList cur = connections;
    while (cur) {
        if (*places_count > places_allocation) {
            places_allocation++;
            places = realloc(places, sizeof(PlaceId) * places_allocation);
        }
        places[(*places_count)++] = cur->p;
        cur = cur->next;
    }

    if (*places_count < places_allocation) {
        places = realloc(places, sizeof(PlaceId) * *places_count);
    }
    return places;
}

/**
 * Uses djikstras path-finding algorithm with priority queue to find shortest path from 1 node to all other nodes
 * @param map
 * @param from
 * @param end
 * @return HashTable containing all computed distances to places (place abbrev)
 */
HashTable GetPathLookupTableFrom(Map map, Place from) {
    // Create vertex dictionary

    // Prime numbers are suitable table sizes and not that many vertices exist
    // so can pick small value
    HashTable distances = HashTableCreate(181);

    for (int i = 0; i < NUM_REAL_PLACES; ++i) {
        Place p = PLACES[i];
        HashInsert(distances, p.abbrev, CreatePath(p.abbrev, INT_MAX, NULL));
    }

    Heap pq = HeapCreate(1024);
    HeapPush(pq, HeapItemCreate(0, CreatePath(from.abbrev, 0, NULL)));

    while (!HeapIsEmpty(pq)) {
        HeapItem current_item = HeapPop(pq);
        Path current_vertex = (Path) current_item->data;
        PlaceId current_place = placeAbbrevToId(current_vertex->place);
        int current_distance = current_vertex->distance;

        // Vertex can be added multiple times to pq. We only want to process it the first time
        Path path_node = (Path) HashGet(distances, current_vertex->place)->value;
        if (current_distance > path_node->distance) continue;
        int reachable_count = 0;
        PlaceId *reachable = get_reachable_places_in_move(map, 0, current_place, &reachable_count);

        for (int i = 0; i < reachable_count; ++i) {
            const char *vertex_abbrev = placeIdToAbbrev(reachable[i]);
            int distance = current_distance + 1; // No weights on edges so simply add 1
            path_node = (Path) HashGet(distances, vertex_abbrev)->value;
            int neighbour_distance_lookup = path_node->distance;
            if (distance < neighbour_distance_lookup) {
                // Create new node for path for found vertex and set predecessor as current_vertex
                Path new = CreatePath((char *) vertex_abbrev, distance, current_vertex);
                HashInsert(distances, vertex_abbrev, new);
                HeapPush(pq, HeapItemCreate(distance, new));
            }
        }
    }
    HeapDestroy(pq);
    return distances;
}

HashTable* GetAllPathLookup(Map map) {
    HashTable *paths_lookup = malloc(sizeof(HashTable) * NUM_REAL_PLACES);
    for (int i = 0; i < NUM_REAL_PLACES; i++) {
        paths_lookup[i] = GetPathLookupTableFrom(map, PLACES[i]);
    }
    return paths_lookup;
}

Path CreatePath(char *place, int distance, Path predecessor) {
    Path path = malloc(sizeof(*path));
    path->distance = distance;
    path->place = malloc(sizeof(strlen(place)));
    strcpy(path->place, place);
    path->predecessor = predecessor;
    return path;
}

void FreePathNode(Path path) {
    free(path->place);
    free(path);
}

Path* GetOrderedPathSequence(Path path) {
    Path *pathArr = malloc(sizeof(Path) * (path->distance + 1));
    int i = path->distance;
    Path cur = path;
    while (cur) {
        pathArr[i] = cur;
        cur = cur->predecessor;
        i--;
    }
    return pathArr;
}

void PrintPathSequence(Path path) {
    int length = 4 * (path->distance + 1);
    char *sequence_str = malloc(sizeof(char) * length);
    sequence_str[length - 1] = '\0';
    Path *pathArr = GetOrderedPathSequence(path);
    for (int j = 0; j < path->distance + 1; ++j) {
        strcpy(&sequence_str[j*4], pathArr[j]->place);
        if (j == path->distance) break;
        sequence_str[(j*4) + 2] = '-';
        sequence_str[(j*4) + 3] = '>';
    }
    printf("%s\n", sequence_str);
}