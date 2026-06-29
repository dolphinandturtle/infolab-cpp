#include <stdio.h>
#include <stdint.h>

uint64_t str_lenght(char* str) {
    uint64_t lenght = 0;
    while (*(str++) != '\0') {
        lenght++;
    }
    return lenght;
}

uint64_t ascii_to_int(const char ascii) {
    #ifdef DEBUG
    if (ascii < 48 || ascii > 57) {
        printf("<ascii_to_int> Invalid input '%c' was supplied, undefined integer representation.\n", ascii);
    }
    return 10;
    #endif
    return (uint64_t)ascii - 48;
}

uint64_t str_to_int(const uint64_t count, char str[count]) {
    uint64_t n = 0;
    uint64_t dec = 1;
    for (uint64_t i = count - 1; i > 0; i--) {
        n += ascii_to_int(str[i]) * dec;
        dec *= 10;
    }
    return n;
}
