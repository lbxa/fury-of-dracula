//
// Created by eric on 29/7/20.
//
#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>

void CheckMallocSuccess(void *mem, char *errorMessage) {
  if (mem == NULL) {
    fprintf(stderr, "%s", errorMessage);
    exit(EXIT_FAILURE);
  }
}

int min(int a, int b) { return (a < b) ? a : b; }

int max(int a, int b) { return (a > b) ? a : b; }