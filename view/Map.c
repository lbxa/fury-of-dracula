////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// Map.c: an implementation of a Map type
// You can change this as much as you want!
// If you modify this, you should submit the new version.
//
// 2017-11-30	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "Map.h"
#include "Places.h"
#include "hash_table.h"
#include "limits.h"
#include "binary_heap.h"
#include "Game.h"
#include <string.h>

struct map {
    int nV; // number of vertices
    int nE; // number of edges
    ConnList connections[NUM_REAL_PLACES];
};

static void addConnections(Map m);
static void addConnection(Map m, PlaceId v, PlaceId w, TransportType type);
static inline bool isSentinelEdge(Connection c);

static ConnList connListInsert(ConnList l, PlaceId v, TransportType type);
static bool connListContains(ConnList l, PlaceId v, TransportType type);

////////////////////////////////////////////////////////////////////////

/** Creates a new map. */
// #vertices always same as NUM_REAL_PLACES
Map MapNew(void)
{
    Map m = malloc(sizeof(*m));
    if (m == NULL) {
        fprintf(stderr, "Couldn't allocate Map!\n");
        exit(EXIT_FAILURE);
    }

    m->nV = NUM_REAL_PLACES;
    m->nE = 0;
    for (int i = 0; i < NUM_REAL_PLACES; i++) {
        m->connections[i] = NULL;
    }

    addConnections(m);
    return m;
}

/** Frees all memory allocated for the given map. */
void MapFree(Map m)
{
    assert (m != NULL);

    for (int i = 0; i < m->nV; i++) {
        ConnList curr = m->connections[i];
        while (curr != NULL) {
            ConnList next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(m);
}

////////////////////////////////////////////////////////////////////////

/** Prints a map to `stdout`. */
void MapShow(Map m)
{
    assert(m != NULL);

    printf("V = %d, E = %d\n", m->nV, m->nE);
    for (int i = 0; i < m->nV; i++) {
        for (ConnList curr = m->connections[i]; curr != NULL; curr = curr->next) {
            printf("%s connects to %s by %s\n",
                   placeIdToName((PlaceId) i),
                   placeIdToName(curr->p),
                   transportTypeToString(curr->type)
            );
        }
    }
}

////////////////////////////////////////////////////////////////////////

/** Gets the number of places in the map. */
int MapNumPlaces(Map m)
{
    assert(m != NULL);
    return m->nV;
}

/** Gets the number of connections of a particular type. */
int MapNumConnections(Map m, TransportType type)
{
    assert(m != NULL);
    assert(transportTypeIsValid(type) || type == ANY);

    int nE = 0;
    for (int i = 0; i < m->nV; i++) {
        for (ConnList curr = m->connections[i]; curr != NULL; curr = curr->next) {
            if (curr->type == type || type == ANY) {
                nE++;
            }
        }
    }

    return nE;
}

////////////////////////////////////////////////////////////////////////

/// Add edges to Graph representing map of Europe
static void addConnections(Map m)
{
    assert(m != NULL);

    for (int i = 0; !isSentinelEdge(CONNECTIONS[i]); i++) {
        addConnection(m, CONNECTIONS[i].v, CONNECTIONS[i].w, CONNECTIONS[i].t);
    }
}

/// Add a new edge to the Map/Graph
static void addConnection(Map m, PlaceId start, PlaceId end, TransportType type)
{
    assert(m != NULL);
    assert(start != end);
    assert(placeIsReal(start));
    assert(placeIsReal(end));
    assert(transportTypeIsValid(type));

    // don't add edges twice
    if (connListContains(m->connections[start], end, type)) return;

    m->connections[start] = connListInsert(m->connections[start], end, type);
    m->connections[end]   = connListInsert(m->connections[end], start, type);
    m->nE++;
}

/// Is this the magic 'sentinel' edge?
static inline bool isSentinelEdge(Connection c)
{
    return c.v == -1 && c.w == -1 && c.t == ANY;
}

/// Insert a node into an adjacency list.
static ConnList connListInsert(ConnList l, PlaceId p, TransportType type)
{
    assert(placeIsReal(p));
    assert(transportTypeIsValid(type));

    ConnList new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Couldn't allocate ConnNode");
        exit(EXIT_FAILURE);
    }

    new->p = p;
    new->type = type;
    new->next = l;
    return new;
}

/// Does this adjacency list contain a particular value?
static bool connListContains(ConnList l, PlaceId p, TransportType type)
{
    assert(placeIsReal(p));
    assert(transportTypeIsValid(type));

    for (ConnList curr = l; curr != NULL; curr = curr->next) {
        if (curr->p == p && curr->type == type) {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////

ConnList MapGetConnections(Map m, PlaceId p)
{
    assert(placeIsReal(p));
    return m->connections[p];
}

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
HashTable mapFindShortestPathsFrom(Map map, Place from) {
    // Create vertex dictionary

    // Prime numbers are suitable table sizes and not that many vertices exist
    // so can pick small value
    HashTable distances = create_hash_table(181);

    for (int i = 0; i < NUM_REAL_PLACES; ++i) {
        Place p = PLACES[i];
        hash_insert(distances, p.abbrev, INT_MAX);
    }

    Heap pq = heap_create(1024);
    heap_push(pq, create_heap_item(0, from.abbrev));

    while (!is_heap_empty(pq)) {
        HeapItem current_vertex = heap_pop(pq);
        PlaceId current_place = placeAbbrevToId(current_vertex->key);
        int current_distance = current_vertex->value;

        // Vertex can be added multiple times to pq. We only want to process it the first time
        if (current_distance > hash_get(distances, current_vertex->key)->value) continue;
        int reachable_count = 0;
        PlaceId *reachable = get_reachable_places_in_move(map, 0, current_place, &reachable_count);

        for (int i = 0; i < reachable_count; ++i) {
            char *vertex_abbrev = placeIdToAbbrev(reachable[i]);
            int distance = current_distance + 1; // No weights on edges so far
            int neighbour_distance_lookup = hash_get(distances, vertex_abbrev)->value;
            if (distance < neighbour_distance_lookup) {
                hash_insert(distances, vertex_abbrev, distance);
                heap_push(pq, create_heap_item(distance, vertex_abbrev));
            }
        }
    }
    heap_destroy(pq);
    return distances;
}

HashTable* getAllPossiblePaths(Map map) {
    HashTable *paths_lookup = malloc(sizeof(HashTable) * NUM_REAL_PLACES);
    for (int i = 0; i < NUM_REAL_PLACES; i++) {
        paths_lookup[i] = mapFindShortestPathsFrom(map, PLACES[i]);
    }
    return paths_lookup;
}