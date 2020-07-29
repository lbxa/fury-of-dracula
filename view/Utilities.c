//
// Created by eric on 29/7/20.
//
#include <stdlib.h>
#include <stdio.h>
#include "Utilities.h"

void CheckMallocSuccess(void *mem, char *errorMessage) {
    if (mem == NULL) {
        fprintf(stderr, "%s", errorMessage);
        exit(EXIT_FAILURE);
    }
}