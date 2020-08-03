//
// Created by eric on 17/7/20.
//

#include "PathFinding.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "GameView.h"
#include "Queue.h"
#include "Utilities.h"

/**
 * Resolves a location on dracula's trail
 * @param gameView
 * @param resolvedLocations
 * @param unresolvedLocation
 * @param moveIndex
 * @return
 */
PlaceId ResolveTrailLocation(GameView gameView, PlaceId* resolvedLocations,
                             PlaceId unresolvedLocation, int moveIndex) {
  if (unresolvedLocation >= DOUBLE_BACK_1 &&
      unresolvedLocation <= DOUBLE_BACK_5) {
    int resolvedIndex = moveIndex - (unresolvedLocation - DOUBLE_BACK_1);
    return resolvedLocations[resolvedIndex];
  } else if (unresolvedLocation == TELEPORT) {
    return CASTLE_DRACULA;
  } else if (unresolvedLocation == HIDE) {
    return resolvedLocations[moveIndex];
  }
  return unresolvedLocation;
}

/**
 * Returns array of reachable locations by rail given distance by rail
 * @param map
 * @param distanceByRail
 * @param currentId
 * @param placesCount
 * @return
 */
PlaceId* getRailReachable(Map map, int distanceByRail, PlaceId currentId,
                          int* placesCount) {
  Queue q = newQueue();
  PlaceId* reachablePlaces = malloc(sizeof(*reachablePlaces) * NUM_REAL_PLACES);
  bool* visited = calloc(NUM_REAL_PLACES, sizeof(*visited));
  int* distances = calloc(NUM_REAL_PLACES, sizeof(*distances));

  // Check memory got allocated successfully
  assert(visited != NULL);
  assert(reachablePlaces != NULL);
  assert(distances != NULL);

  visited[currentId] = true;
  QueueJoin(q, currentId);

  // Breadth first search to get reachable locations by rail
  while (!QueueIsEmpty(q)) {
    PlaceId currentPlace = QueueLeave(q);
    // Skip place if it is not reachable within the given number of
    // rail moves
    if (distances[currentPlace] >= distanceByRail) continue;
    ConnList connections = MapGetConnections(map, currentPlace);
    for (ConnList cur = connections; cur != NULL; cur = cur->next) {
      // This function only deals with rail connections
      if (cur->type != RAIL) continue;
      // If already visited then skip
      if (visited[cur->p]) continue;
      QueueJoin(q, cur->p);
      visited[cur->p] = true;
      distances[cur->p] = distances[currentPlace] + 1;
      reachablePlaces[*placesCount] = cur->p;
      (*placesCount)++;
    }
  }

  // Free allocated memory
  dropQueue(q);
  free(visited);
  free(distances);

  return reachablePlaces;
}

/**
 * Gets all reachable places in a single move for a player
 * @return
 */
PlaceId* GetPossibleMoves(GameView gameView, Map map, Player player,
                          PlaceId currentId, bool road, bool rail, bool boat,
                          int round, int* placesCount, bool resolveMoves,
                          bool applyTrailRestrictions) {
  // Get places the given place is connected to
  ConnList connections = MapGetConnections(map, currentId);

  // Calculate number of rail moves player can make
  int numberRailMoves =
      rail * (((int)player + round) % 4) * (player != PLAYER_DRACULA);

  // Variables for dracula trail handling
  int trailNumMoves = 0;
  bool canFree = false;
  PlaceId* trailMoves = NULL;
  PlaceId* locationHistory = NULL;
  bool canHide = placeIsLand(currentId);
  bool canDoubleBack = true;
  bool onTrailLookup[NUM_REAL_PLACES] = {false};

  // Determine whether can hide and double back based on if those moves are in
  // trail
  if (player == PLAYER_DRACULA && applyTrailRestrictions) {
    trailMoves = GvGetLastMoves(gameView, PLAYER_DRACULA, TRAIL_SIZE - 1,
                                &trailNumMoves, &canFree);
    locationHistory = GvGetLastLocations(gameView, PLAYER_DRACULA, TRAIL_SIZE - 1,
                                         &trailNumMoves, &canFree);
    FILE* draculaLog = fopen("dracula.log", "a");
    for (int i = 0; i < trailNumMoves; ++i) {
      fprintf(draculaLog, "Trail (%d): %s -> %s\n", i,
              placeIdToName(trailMoves[i]), placeIdToName(locationHistory[i]));
      onTrailLookup[locationHistory[i]] = true;
      if (trailMoves[i] == HIDE) {
        canHide = false;
      } else if (trailMoves[i] >= DOUBLE_BACK_1 &&
                 trailMoves[i] <= DOUBLE_BACK_5) {
        canDoubleBack = false;
      }
    }
    fclose(draculaLog);
  }

  *placesCount = 0;
  PlaceId* places = malloc(sizeof(PlaceId) * NUM_REAL_PLACES);
  CheckMallocSuccess(places, "Couldn't allocate for places array!\n");

  // Use as lookup to stop placing duplicates moves/locations in output
  bool placesAdded[NUM_REAL_PLACES] = {false};
  bool placesAdjacent[NUM_REAL_PLACES] = {false};
  placesAdjacent[currentId] = true;

  // Handle checking whether current place can be added as possible location
  // as dracula can only move to current location as HIDE if not HIDE not
  // already in trailMoves
  if (player != PLAYER_DRACULA || canHide) {
    if (player != PLAYER_DRACULA || resolveMoves) {
      places[0] = currentId;
    } else {
      places[0] = HIDE;
    }
    (*placesCount)++;
  }

  if (numberRailMoves > 0) {
    int railReachableCount = 0;
    PlaceId* railReachable =
        getRailReachable(map, numberRailMoves, currentId, &railReachableCount);
    for (int i = 0; i < railReachableCount; ++i) {
      if (placesAdded[railReachable[i]]) continue;
      places[(*placesCount)++] = railReachable[i];
      placesAdded[railReachable[i]] = true;
    }
    free(railReachable);
  }

  // Loop through connections and add possible locations/moves
  ConnList cur = connections;
  while (cur) {
    // Apply restriction that dracula can't go to HOSPITAL_PLACE
    if (!(player == PLAYER_DRACULA && cur->p == HOSPITAL_PLACE)) {
      // Applies movement restriction if dracula
      if (!placesAdded[cur->p] && !onTrailLookup[cur->p]) {
        if (cur->type == ROAD && road) {
          // If can use road connections then add it
          places[(*placesCount)++] = cur->p;
          placesAdded[cur->p] = true;
          placesAdjacent[cur->p] = true;
          FILE *draculaLog = fopen("dracula.log", "a");
          fprintf(draculaLog, "Added %s as adjacent\n", placeIdToName(cur->p));
          fclose(draculaLog);
        } else if (cur->type == BOAT && boat) {
          // If can use boat connections then add it
          places[(*placesCount)++] = cur->p;
          placesAdded[cur->p] = true;
          placesAdjacent[cur->p] = true;
          FILE *draculaLog = fopen("dracula.log", "a");
          fprintf(draculaLog, "Added %s as adjacent\n", placeIdToName(cur->p));
          fclose(draculaLog);
        }
      }
    }
    cur = cur->next;
  }

  // Handle dracula double back moves
  if (player == PLAYER_DRACULA && canDoubleBack && applyTrailRestrictions) {
    int locationCount = 0;
    PlaceId* resolvedLocations =
        GvGetLocationHistory(gameView, player, &locationCount, &canFree);

    for (int i = 0; i < (trailNumMoves > 5 ? trailNumMoves - 1 : trailNumMoves); i++) {
      // Need to perform resolve location of moves
      PlaceId place;
      PlaceId resolved = resolvedLocations[locationCount - 1 - i];
      printf("%d %s\n", i, placeIdToName(resolved));
      if (!placesAdjacent[resolved]) continue;  // Can only double back to adjacent places
      FILE *draculaLog = fopen("dracula.log", "a");
      fprintf(draculaLog, "Adjacent\n");
      for (int j = 0; j < NUM_REAL_PLACES; ++j) {
        if (placesAdjacent[j] == true) {
          fprintf(draculaLog, "\t -> %s\n", placeIdToName(j));
        }
      }

      fclose(draculaLog);

      if (resolveMoves) {
        place = resolved;
      } else {
        place = DOUBLE_BACK_1 + i;
      }
      draculaLog = fopen("dracula.log", "a");
      fprintf(draculaLog, "%s -> %s\n", placeIdToName(resolved), placeIdToName(DOUBLE_BACK_1 + i));
      fclose(draculaLog);
      if (!placesAdded[place]) {
        placesAdded[place] = true;
        places[(*placesCount)++] = place;
      }
    }
    free(resolvedLocations);
  }

  // Free memory
  free(trailMoves);
  free(locationHistory);

  // Reallocate to needed size
  if (*placesCount < NUM_REAL_PLACES) {
    places = realloc(places, sizeof(PlaceId) * (*placesCount));
  }
  return places;
}

/**
 * Uses dijkstra's path-finding algorithm with priority queue to find shortest
 * path from 1 node to all other nodes
 * @param map
 * @param from
 * @param end
 * @return HashTable containing all computed distances to places (place abbrev)
 */
HashTable GetPathLookupTableFrom(GameView gameView, Map map, Player player,
                                 Place from, bool road, bool rail, bool boat,
                                 int round, bool resolveMoves,
                                 bool applyTrailRestrictions) {
  // Prime numbers are suitable table sizes and not that many vertices exist
  // so can pick small value
  HashTable distances = HashTableCreate(181);

  for (int i = 0; i < NUM_REAL_PLACES; ++i) {
    Place p = PLACES[i];
    HashInsert(distances, p.abbrev, CreatePath(p.abbrev, INT_MAX, NULL));
  }

  // Create priority queue and push starting location with distance of 0
  Heap pq = HeapCreate(1024);
  HeapPush(pq, HeapItemCreate(0, CreatePath(from.abbrev, 0, NULL)));

  while (!HeapIsEmpty(pq)) {
    HeapItem currentItem = HeapPop(pq);
    Path currentVertex = (Path)currentItem->data;
    PlaceId currentPlace = placeAbbrevToId(currentVertex->place);
    int currentDistance = currentVertex->distance;

    // Vertex can be added multiple times to priority queue
    // We only want to process it the first time
    Path pathNode = (Path)HashGet(distances, currentVertex->place)->value;
    if (currentDistance > pathNode->distance) continue;
    int reachableCount = 0;

    // Gets possible moves/locations using restrictions as passed into function
    PlaceId* reachable =
        GetPossibleMoves(gameView, map, player, currentPlace, road, rail, boat,
                         round + currentDistance, &reachableCount, resolveMoves,
                         applyTrailRestrictions);

    // Loop through possible moves at the currently processed vertex
    for (int i = 0; i < reachableCount; ++i) {
      // Since reachable includes currentPlace then skip it
      if (reachable[i] == currentPlace) continue;
      const char* vertexAbbrev = placeIdToAbbrev(reachable[i]);

      // No weights on edges so simply add 1
      int distance = currentDistance + 1;

      pathNode = (Path)HashGet(distances, vertexAbbrev)->value;
      int neighbourDistanceLookup = pathNode->distance;

      // If distance is better then what has been currently found then set that
      // as path
      if (distance < neighbourDistanceLookup) {
        // Create new node for path for found vertex and set predecessor as
        // currentVertex
        Path new = CreatePath((char*)vertexAbbrev, distance, currentVertex);

        // Updated distances and insert to priority queue
        HashInsert(distances, vertexAbbrev, new);
        HeapPush(pq, HeapItemCreate(distance, new));
      }
    }
    // Free memory
    free(reachable);
    free(currentItem);
  }
  // Free memory
  HeapDestroy(pq);

  return distances;
}

Path CreatePath(char* place, int distance, Path predecessor) {
  // Create and allocate for path struct and intitialise all struct fields
  Path path = malloc(sizeof(*path));
  CheckMallocSuccess(path, "Couldn't allocate for path!\n");
  path->distance = distance;
  path->place = malloc(sizeof(strlen(place)));
  CheckMallocSuccess(path, "Couldn't allocate for place string!\n");
  strcpy(path->place, place);
  path->predecessor = predecessor;
  return path;
}

void FreePathNode(void* path) {
  free(((Path)path)->place);
  free(path);
}

Path* GetOrderedPathSequence(Path path) {
  Path* pathArr = malloc(sizeof(Path) * (path->distance + 1));
  CheckMallocSuccess(pathArr, "Couldn't allocate path array!\n");
  int i = path->distance;
  Path cur = path;
  // Step through predecessors and reorder so that they are in ascending order
  // instead of reverse
  while (cur) {
    pathArr[i] = cur;
    cur = cur->predecessor;
    i--;
  }
  return pathArr;
}

PlaceId* GetOrderedPlaceIds(Path path) {
  PlaceId* placeIdArr = malloc(sizeof(PlaceId) * (path->distance));
  CheckMallocSuccess(placeIdArr, "Couldn't allocate place id array!\n");
  int i = path->distance - 1;
  Path cur = path;
  // Step through predecessors and reorder so that they are in ascending order
  // instead of reverse
  while (cur && cur->predecessor) {
    placeIdArr[i] = placeAbbrevToId(cur->place);
    cur = cur->predecessor;
    i--;
  }
  return placeIdArr;
}

void PrintPathSequence(Path path) {
  int length = 4 * (path->distance + 1);
  char* sequenceStr = malloc(sizeof(char) * length);
  CheckMallocSuccess(sequenceStr, "Couldn't allocate sequence string!\n");
  sequenceStr[length - 1] = '\0';
  Path* pathArr = GetOrderedPathSequence(path);
  // Loop through path array and place the elements in a sequence string
  for (int j = 0; j < path->distance + 1; ++j) {
    strcpy(&sequenceStr[j * 4], pathArr[j]->place);
    if (j == path->distance) break;
    sequenceStr[(j * 4) + 2] = '-';
    sequenceStr[(j * 4) + 3] = '>';
  }
  printf("%s\n", sequenceStr);
}

PlaceId* GetShortestPathTo(GameView gameView, Player player, PlaceId dest,
                           int* pathLength) {
  // Getting map
  Map map = GetMap(gameView);

  // Getting data for Place
  *pathLength = 0;
  PlaceId currentLocation = GvGetPlayerLocation(gameView, player);
  if (currentLocation == dest) return NULL;

  Place* from = malloc(sizeof(struct place));
  CheckMallocSuccess(from, "Couldn't allocate Place!\n");

  int round = GvGetRound(gameView);
  HashTable pathLookup = NULL;

  // Finds path given appropriate player restrictions
  if (player == PLAYER_DRACULA) {
    pathLookup =
        GetPathLookupTableFrom(gameView, map, player, PLACES[currentLocation],
                               true, false, true, round, true, true);
  } else {
    pathLookup =
        GetPathLookupTableFrom(gameView, map, player, PLACES[currentLocation],
                               true, true, true, round, true, false);
  }
  if (pathLookup == NULL) return NULL;

  Path path = (Path)HashGet(pathLookup, placeIdToAbbrev(dest))->value;

  *pathLength = path->distance;
  return GetOrderedPlaceIds(path);
}