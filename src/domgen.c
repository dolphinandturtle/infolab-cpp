#include <stdio.h>
#include <stdint.h>
#include "string.h"

// Structures
struct Coord {
    double x;
    double y;
};

// Debugging stuff
#define ERROR_NOT_ENOUGH_ARGUMENTS 1
#define ERROR_TO_MANY_ARGUMENTS 2

const char *DEBUG_MESSAGES[] = {

    [ERROR_NOT_ENOUGH_ARGUMENTS]
    ="No arguments have been supplied...\n",

    [ERROR_TO_MANY_ARGUMENTS]
    ="To many arguments have been supplied...\n"
};

// Idk code
uint64_t width_mesh(const uint64_t count) {
    return count + 2;
}

uint64_t height_mesh(const uint64_t count) {
    return count + 2;
}

uint64_t size_mesh(const uint64_t count) {
    return width_mesh(count) * height_mesh(count) * sizeof(struct Coord);
}

void init_mesh(const uint64_t count, uint8_t* buffer) {
    // Boiler
    const uint64_t width = width_mesh(count);
    const uint64_t height = height_mesh(count);
    struct Coord (*mesh)[height] = (struct Coord (*)[width])buffer;
    // Main
    const double step = 1 / (double)(count + 1);
    for (uint64_t i = 0; i < width; i++) {
        for (uint64_t j = 0; j < height; j++) {
            mesh[i][j].x = (double)i * step;
            mesh[i][j].y = (double)j * step;
        }
    }
    return;
}

int serialize_mesh(FILE* stream, const uint64_t count, uint8_t* buffer) {
    const uint64_t width = width_mesh(count);
    const uint64_t height = height_mesh(count);
    struct Coord (*mesh)[height] = (struct Coord (*)[width])buffer;
    int error = 0;
    uint64_t n = 0;
    for (uint64_t i = 0; i < width; i++) {
        for (uint64_t j = 0; j < height; j++) {
            error = fprintf(stream, "%ld %ld %ld %f %f\n", n, i, j, mesh[i][j].x, mesh[i][j].y);
            if (error < 0) {
                return error;
            }
            n++;
        }
    }
    return 0;
}

// Program
int main(int argc, char** argv) {
    if (argc == 1) {
        printf(DEBUG_MESSAGES[ERROR_NOT_ENOUGH_ARGUMENTS]);
        return ERROR_NOT_ENOUGH_ARGUMENTS;
    }
    else if (argc > 2) {
        printf(DEBUG_MESSAGES[ERROR_TO_MANY_ARGUMENTS]);
        return ERROR_TO_MANY_ARGUMENTS;
    }
    else {
        const uint64_t count = str_to_int(str_lenght(argv[1]), argv[1]);
        uint8_t buffer[size_mesh(count)];
        init_mesh(count, buffer);
        FILE* stream = fopen("coords.txt", "w");
        serialize_mesh(stream, count, buffer);
        return 0;
    }
}
