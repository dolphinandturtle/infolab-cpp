#include <stdio.h>
#include <stdint.h>
#include "mesh.h"

// The width and height terms are constantly
// offsetted by 2 because we are going to
// handle mostly the boundary, then numbers
// are nicer on paper.
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
    struct Coord (*mesh)[height] = (struct Coord (*)[height])buffer;
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
    struct Coord (*mesh)[height] = (struct Coord (*)[height])buffer;
    int error = 0;
    uint64_t n = 0;
    // Discarding boudaries (1 < i < width - 1)
    for (uint64_t i = 1; i < width - 1; i++) {
        // Discarding boudaries (1 < j < height - 1)
        for (uint64_t j = 1; j < height - 1; j++) {
            error = fprintf(stream, "%ld %ld %ld %f %f\n", n, i, j, mesh[i][j].x, mesh[i][j].y);
            if (error < 0) {
                return error;
            }
            n++;
        }
    }
    return 0;
}
