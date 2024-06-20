#pragma once

#include <stdint.h>

struct pcg32 {
    uint64_t state;
    uint64_t inc; /* sequence/stream, always odd */
};

struct pcg32x2 {
    struct pcg32 rng1;
    struct pcg32 rng2;
};

void pcg32_default_seed(struct pcg32 *rng, uint64_t inc);
uint32_t pcg32_random(struct pcg32 *rng);

void pcg32x2_default_seed(struct pcg32x2 *rng, uint64_t inc);
uint64_t pcg32x2_random(struct pcg32x2 *rng);

/* [0,1) */
double pcg53(struct pcg32x2 *rng);
