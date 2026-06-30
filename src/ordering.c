#include <stdio.h>
#include <stdint.h>


static uint8_t max(uint8_t a, uint8_t b) {
    return (a > b) ? a : b;
}

static uint64_t pow2len(uint8_t l) {
    return (uint64_t)(1 << l) - 1;
}

uint8_t pi(const uint64_t i) {
    uint8_t n = 1;
    uint64_t j = i;
    while (!(j % 2)) {
        j /= 2;
        n++;
    }
    return n;
}

uint64_t count_cross(uint8_t l) {
    return 2 * pow2len(l) - 1;
}

void sepgen(uint8_t l, uint8_t *buffer) {
    uint64_t* ordering = (uint64_t*)buffer;
    uint64_t count_free = pow2len(l) * pow2len(l);
    uint64_t imap[l];
    uint64_t heads[l];
    for (uint64_t i = 1; i <= l; i++) {
        heads[i] = 0;
    }
    imap[1] = 0;
    for (uint8_t i = l, mul = 1; i >= 2; i--, mul *= 4) {
        count_free -= mul * count_cross(i);
        imap[i] = count_free;
    }
    for (uint64_t i = 1; i <= l; i++) {
        printf("%ld -> %ld\n", i, imap[i]);
    }
    uint8_t m = 0;
    for (uint64_t i = 1; i <= pow2len(l); i++) {
        for (uint64_t j = 1; j <= pow2len(l); j++) {
            m = max(pi(i), pi(j));
            // My IQ is overflowing (very negative)
            printf("%ld, %ld -> %d\n", i, j, m);
            ordering[imap[m] + heads[m]++] = (pow2len(l) * (i-1) + j);
        }
    }
    for (uint64_t i = 1; i <= pow2len(l) * pow2len(l); i++) {
        printf("%ld -> %ld\n", i, ordering[i]);
    }
    return;
}

int main(void) {
    uint8_t buffer[2048] = {0};
    sepgen(3, buffer);
    return 0;
}
