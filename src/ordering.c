#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "string.h"


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
    for (uint64_t i = 0; i < pow2len(l); i++) {
        cache[i] = 0;
    }

    // Building heads
    for (uint64_t i = 0; i < l; i++) {
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
    fprintf(stream, "%ld\n", count);
    for (uint64_t i = 0; i < count; i++) {
        error = fprintf(stream, "%ld %ld\n", i, ordering[i]);
        if (error < 0) {
            return error;
        }
    }
    return 0;
}

uint64_t copy_bytes(const uint64_t count, const uint8_t bytes[count], uint8_t* buffer, const char eos) {
    for (uint64_t i = 0; i < count; i++) {
        if (bytes[i] == eos) {
            return i + 1;
        }
        else {
            buffer[i] = bytes[i];
        }
    }
    return count;
}

uint64_t size_ordering_serialized(FILE* stream) {
    uint8_t line[256] = "";
    fgets((char*)line, 256, stream);
    return str_to_int((uint8_t)(str_lenght(line)-1), line);
}

int deserialize_ordering(FILE* stream, uint64_t* ord) {
    // 256 is a placeholder
    uint8_t line[256] = "";
    uint8_t tail = 0;
    uint8_t head = 0;
    uint8_t str[256] = "";
    uint64_t m = 0, n = 0;

    // Skip first line
    fgets((char*)line, 256, stream);
    fgets((char*)line, 256, stream);

    while (!feof(stream)) {
        // fgets((char*)line, 256, stream);
        //printf("%s\n", line);

        // m
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, ' ');
        m = str_to_int((uint8_t)(head-1)-tail, str);
        tail = head;

        // n
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, '\n');
        n = str_to_int((uint8_t)(head-1)-tail, str);
        tail = 0;
        head = 0;

        ord[m] = n;
        printf("%ld %ld\n", m, n);
        fgets((char*)line, 256, stream);
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Needs input number...\n");
        return -1;
    }
    const uint8_t l = (uint8_t)str_to_int(str_lenght((uint8_t*)argv[1]), (uint8_t*)argv[1]);

    uint64_t* ord = malloc(pow2len(l)*pow2len(l)*sizeof(uint64_t));
    sepgen(l, ord);
    FILE* stream = fopen("ordering.txt", "w");
    serialize_ordering(stream, pow2len(l) * pow2len(l), ord);
    fclose(stream);
    free(ord);

    stream = fopen("ordering.txt", "r");
    uint64_t count2 = size_ordering_serialized(stream);
    fclose(stream);

    stream = fopen("ordering.txt", "r");
    printf("count2: %ld\n", count2);
    uint64_t buffer2[count2];
    deserialize_ordering(stream, buffer2);
    fclose(stream);
    return 0;
}
