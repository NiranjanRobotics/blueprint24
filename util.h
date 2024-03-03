#ifndef util_h
#define util_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define ERROR(message) \
    do \
    { \
        fprintf(stderr, "[VM] Error: %s\n", message); \
        exit(1); \
    } \
    while (false)

#endif