#include <stdio.h>
#include <stdint.h>


static uint8_t max(uint8_t a, uint8_t b) {
    return (a > b) ? a : b;
}

static uint64_t pow2len(uint8_t l) {
    return (uint64_t)(1 << l) - 1;
}

uint8_t fo2(const uint64_t n, uint8_t* cache) {

    /* In the limit where factorization is iterated
    'noticeable' times, caching previous results
    avoids repeating calculations. This is more
    efficient with the cost of added space
    complexity. */

    // The zero count is reserved for uncached results
    uint8_t count = 1;
    uint64_t quot = n;
    while (!(quot % 2)) {
        if (!cache[quot]) {
            quot /= 2;
            count++;
        }
        else {
            // Compensation for cache result offset
            count += (uint8_t)(cache[quot] - 1);
            break;
        }
    }
    cache[n] = count;
    // Compensation for cache result offset
    return count - 1;
}

void sepgen(uint8_t l, uint64_t* ord) {
    uint64_t nord = pow2len(l) * pow2len(l);
    uint64_t offsets[l];
    uint64_t heads[l];
    uint8_t cache[pow2len(l)];

    // Clearing cache
    for (uint64_t i = 1; i <= pow2len(l); i++) {
        cache[i] = 0;
    }

    // Building heads
    for (uint64_t i = 1; i <= l; i++) {
        heads[i] = 0;
    }

    // Building offsets
    offsets[l-1] = nord - (2 * pow2len(l) - 1);
    #ifdef DEBUG
    printf("[DEBUG] S%d (offset): %ld\n", l-1, offsets[l-1]);
    #endif
    for (uint8_t i = 0, mul = 4; i < l - 1; i++, mul *= 4) {
        offsets[l-2-i] = offsets[l-1-i] - mul * (2 * pow2len((uint8_t)(l-1-i)) - 1);
        #ifdef DEBUG
        printf("[DEBUG] S%d (offset): %ld\n", l-2-i, offsets[l-2-i]);
        #endif
    }

    // Ordering nodes with the 'Alan George Method'
    uint8_t m = 0;
    for (uint64_t i = 0; i < pow2len(l); i++) {
        for (uint64_t j = 0; j < pow2len(l); j++) {
            m = max(fo2(i+1, cache), fo2(j+1, cache));
            ord[pow2len(l)*i+j] = offsets[m] + heads[m]++;
            #ifdef DEBUG
            printf("[DEBUG] ");
            printf("(%ld,%ld) ~ %ld ", i, j, pow2len(l)*i+j);
            printf("==[max(fo2(%ld),fo2(%ld))->%d]==> ", i+1, j+1, m);
            printf("%ld\n", offsets[m] + heads[m] - 1);
            #endif
        }
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return;
}

int serialize_ordering(FILE* stream, const uint64_t count, uint64_t ordering[count]) {
    int error = 0;
    for (uint64_t i = 0; i < count; i++) {
        error = fprintf(stream, "%ld %ld\n", i, ordering[i]);
        if (error < 0) {
            return error;
        }
    }
    return 0;
}

int main(void) {
    uint64_t ord[2048] = {0};
    sepgen(3, ord);
    FILE* stream = fopen("ordering.txt", "w");
    serialize_ordering(stream, pow2len(3) * pow2len(3), ord);
    fclose(stream);
    return 0;
}
