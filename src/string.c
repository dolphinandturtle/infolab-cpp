#include <stdio.h>
#include <stdint.h>

uint64_t str_lenght(char* str) {
    uint64_t lenght = 0;
    while (*(str++) != '\0') {
        lenght++;
    }
    return lenght;
}

uint8_t ascii_to_int(const uint8_t ascii) {
    #ifdef DEBUG
    if (ascii < 48 || ascii > 57) {
        printf("<ascii_to_int> Invalid input '%c' was supplied, undefined integer representation.\n", ascii);
        return UINT8_MAX;
    }
    #endif
    return ascii - 48;
}

uint64_t str_to_int(const uint64_t count, uint8_t str[count]) {
    uint64_t n = 0;
    uint64_t dec = 1;
    for (uint64_t i = 0; i < count; i++) {
        n += ascii_to_int(str[count - 1 - i]) * dec;
        dec *= 10;
    }
    return n;
}
