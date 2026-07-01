#include <stdio.h>
#include <stdint.h>
#include "string.h"

int main(void) {
    char* float_string[] = {
        "0.0", "0.1", "1.0",
        "0.123456789", "1234.567890"
    };

    double conv = 0.0;
    for (uint8_t i = 0; i < sizeof(float_string) / sizeof(char*); i++) {
        conv = str_to_double(str_lenght(float_string[i]), float_string[i]);
        printf("[PASS] %s -> %.10f\n", float_string[i], conv);
    }

    return 0;
}
