//
// Created by eric on 24/7/20.
//

#ifndef FURY_OF_DRACULA_QUEUE_H
#define FURY_OF_DRACULA_QUEUE_H

#include "Places.h"

typedef struct QueueRep *Queue;

Queue newQueue(void);                 // create new empty queue
void dropQueue(Queue);                // free memory used by queue
void QueueJoin(Queue, PlaceId item);  // add item on queue
PlaceId QueueLeave(Queue);            // remove item from queue
int QueueIsEmpty(Queue);              // check for no items

#endif  // FURY_OF_DRACULA_QUEUE_H
