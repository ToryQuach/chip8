#include "utils.h"

#include <stdlib.h>

uint8_t random_u8(void) {
    int limit = RAND_MAX - (RAND_MAX % 256) - 1;
    int r;
    do {
        r = rand();
    } while (r > limit);
    return (uint8_t)(r % 256);
}