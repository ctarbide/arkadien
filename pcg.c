
/* https://www.pcg-random.org/download.html
 */

// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>

#include <stdio.h> /* perror */
#include <stdlib.h> /* exit */

#include "pcg.h"

void
pcg32_default_seed(struct pcg32 *rng, uint64_t inc)
{
    int i;
    struct timeval tv[1];
    uint64_t state;
    if (gettimeofday(tv, NULL)) {
        perror("gettimeofday");
        exit(1);
    }
    state = tv->tv_sec + tv->tv_usec;
    inc |= 1;
    for (i=0; i<16; i++)
        state = state * 6364136223846793005ULL + inc; /* stir */
    rng->state = state;
    rng->inc = inc;
}

uint32_t
pcg32_random(struct pcg32 *rng)
{
    uint64_t oldstate = rng->state;
    uint32_t xorshifted, rot;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    rot = oldstate >> 59u;
    return xorshifted >> rot | xorshifted << (-rot & 31);
}

#define CAT64(x,y) ((uint64_t)(x) << 32 | (y))
#define PCG64(x,y) CAT64(pcg32_random(x), pcg32_random(y))

void
pcg32x2_default_seed(struct pcg32x2 *rng, uint64_t inc)
{
    uint32_t a, b;
    pcg32_default_seed(&rng->rng1, inc);
    a = pcg32_random(&rng->rng1);
    b = pcg32_random(&rng->rng1);
    pcg32_default_seed(&rng->rng2, CAT64(a, b) | 1);
}

uint64_t
pcg32x2_random(struct pcg32x2 *rng)
{
    return PCG64(&rng->rng1, &rng->rng2);
}

/* generates a random number on [0,1) with 53-bit resolution
 *
 * log(67108864) / log(2) = 26.0
 * log(9007199254740992) / log(2) = 53.0
 *
 */

/* [0,1) */
double
pcg53(struct pcg32x2 *rng)
{
    uint64_t a = pcg32_random(&rng->rng1) >> (32 - 27);
    uint64_t b = pcg32_random(&rng->rng2) >> (32 - 26);
    return (a * (double)67108864.0 + b) / (double)9007199254740992.0;
}
