//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include "hash_table.h"
#include "Map.h"
#include <stdio.h>
#include <time.h>
#include "path_finding.h"
#include "GameView.h"

void print_hash_node(HashNode node) {
    Path p = (Path) node->value;
    printf("[%s -> %d]\n", p->place, p->distance);
}

void print_heap_item(HeapItem node) {
    Path p = (Path) node->data;
    printf("[%s -> %d]\n", p->place, p->distance);
}

int main(void) {

//    Map map = MapNew();
//    clock_t start = clock();
////    HashTable distances;
//    HashTable *paths_lookup = GetAllPathLookup(map);
//    HashNode path = HashGet(paths_lookup[placeAbbrevToId("LS")], "KL");
//    clock_t end = clock();
//    HashTableDisplay(paths_lookup[0], print_hash_node);
//    printf("%s -> %d\n", path->key, ((Path) path->value)->distance);
//    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);
//    PrintPathSequence((Path) path->value);

    GameView gameView = GvNew("GED.... SGE.... HZU.... MCA.... DCF.V.. GMN.... SCFVD.. HGE.... MLS.... DBOT... GLO.... SMR.... HCF.... MMA.... DC?T... GPL.... SMS.... HMR.... MGR.... DBAT... GLO.... SBATD.. HMS.... MMA.... DC?T... GPL.... SSJ.... HBA.... MGR.... DC?T... GPL.... SSJ.... HBA.... MGR.... DC?T...", NULL);
    int numLocs = 0;
    PlaceId *reachable = GvGetReachableByType(gameView, 0, 3,
            placeAbbrevToId("LS"), true, true, true, &numLocs);
    printf("\n");
    for (int i = 0; i < numLocs; i++) {
        printf("%s\n", placeIdToName(reachable[i]));
    }

//    heap_display(h);
//    EmptyHeap(h);
    return 0;
}