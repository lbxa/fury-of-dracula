//
// Created by eric on 17/7/20.
//

#include "path_finding.h"
#include "Queue.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////
// PATH FINDING
// Full lookup if need to recreate each turn will entail creating 4 lookups for each starting initial state of having
// 0, 1, 2, 3 rail moves + a lookup for no rail moves at any time (dracula)

/**
 * Returns array of reachable locations by rail given distance by rail
 * //TODO: Check if requires minor optimisations as looked like returned duplicates
 * @param map
 * @param distanceByRail
 * @param currentId
 * @param placesCount
 * @return
 */
PlaceId *getRailReachable(Map map, int distanceByRail, PlaceId currentId, int *placesCount) {
    Queue q = newQueue();
    PlaceId *reachablePlaces = malloc(sizeof(*reachablePlaces) * NUM_REAL_PLACES);
    bool *visited = calloc(NUM_REAL_PLACES, sizeof(*visited));
    int *distances = calloc(NUM_REAL_PLACES, sizeof(*distances));

    assert(visited != NULL);
    assert(reachablePlaces != NULL);
    assert(distances != NULL);

    visited[currentId] = true;
    QueueJoin(q, currentId);

    while (!QueueIsEmpty(q)) {
        PlaceId currentPlace = QueueLeave(q);
        if (distances[currentPlace] >= distanceByRail) continue;
        ConnList connections = MapGetConnections(map, currentPlace);
        for (ConnList cur = connections; cur != NULL; cur = cur->next) {
            if (cur->type != RAIL) continue;
            if (visited[cur->p]) continue;
            QueueJoin(q, cur->p);
            visited[cur->p] = true;
            distances[cur->p] = distances[currentPlace] + 1;
            reachablePlaces[*placesCount] = cur->p;
            (*placesCount)++;
        }
    }

    dropQueue(q);
    free(visited);
    free(distances);
    return reachablePlaces;
}

/**
 * Gets all reachable places in a single move for a player
 * @return
 */
PlaceId *
GetReachablePlacesInMove(Map map, PlaceId currentId, bool road, bool boat, int distanceByRail, int *placesCount) {
    // For now only return immediate connections no special regard for rail

    ConnList connections = MapGetConnections(map, currentId);

    *placesCount = 0;
    PlaceId *places = malloc(sizeof(PlaceId) * NUM_REAL_PLACES);
    bool placesAdded[NUM_REAL_PLACES] = {false};

    ConnList cur = connections;
    while (cur) {
        if (cur->type == RAIL) {
            if (distanceByRail > 0) {
                places[(*placesCount)++] = cur->p;
                placesAdded[cur->p] = true;
                int railReachableCount = 0;
                PlaceId *railReachable = getRailReachable(map, distanceByRail, currentId, &railReachableCount);
                for (int i = 0; i < railReachableCount; ++i) {
                    if (placesAdded[railReachable[i]]) continue;
                    places[(*placesCount)++] = railReachable[i];
                    placesAdded[railReachable[i]] = true;
                }
                free(railReachable);
            }
        } else {
            if (!placesAdded[cur->p]) {
                if (cur->type == ROAD && road) {
                    places[(*placesCount)++] = cur->p;
                } else if (cur->type == BOAT && boat) {
                    places[(*placesCount)++] = cur->p;
                }
                placesAdded[cur->p] = true;
            }
        }
        cur = cur->next;
    }

    if (*placesCount < NUM_REAL_PLACES) {
        places = realloc(places, sizeof(PlaceId) * (*placesCount));
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
        HeapItem currentItem = HeapPop(pq);
        Path currentVertex = (Path) currentItem->data;
        PlaceId currentPlace = placeAbbrevToId(currentVertex->place);
        int currentDistance = currentVertex->distance;

        // Vertex can be added multiple times to pq. We only want to process it the first time
        Path pathNode = (Path) HashGet(distances, currentVertex->place)->value;
        if (currentDistance > pathNode->distance) continue;
        int reachableCount = 0;
        PlaceId *reachable = GetReachablePlacesInMove(map, currentPlace, true, true, 3, &reachableCount);

        for (int i = 0; i < reachableCount; ++i) {
            const char *vertexAbbrev = placeIdToAbbrev(reachable[i]);
            int distance = currentDistance + 1; // No weights on edges so simply add 1
            pathNode = (Path) HashGet(distances, vertexAbbrev)->value;
            int neighbourDistanceLookup = pathNode->distance;
            if (distance < neighbourDistanceLookup) {
                // Create new node for path for found vertex and set predecessor as currentVertex
                Path new = CreatePath((char *) vertexAbbrev, distance, currentVertex);
                HashInsert(distances, vertexAbbrev, new);
                HeapPush(pq, HeapItemCreate(distance, new));
            }
        }
    }
    HeapDestroy(pq);
    return distances;
}

HashTable* GetAllPathLookup(Map map) {
    HashTable *pathsLookup = malloc(sizeof(HashTable) * NUM_REAL_PLACES);
    for (int i = 0; i < NUM_REAL_PLACES; i++) {
        pathsLookup[i] = GetPathLookupTableFrom(map, PLACES[i]);
    }
    return pathsLookup;
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
    char *sequenceStr = malloc(sizeof(char) * length);
    sequenceStr[length - 1] = '\0';
    Path *pathArr = GetOrderedPathSequence(path);
    for (int j = 0; j < path->distance + 1; ++j) {
        strcpy(&sequenceStr[j * 4], pathArr[j]->place);
        if (j == path->distance) break;
        sequenceStr[(j * 4) + 2] = '-';
        sequenceStr[(j * 4) + 3] = '>';
    }
    printf("%s\n", sequenceStr);
}