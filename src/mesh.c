#include <stdio.h>
#include <stdint.h>
#include "string.h"
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

int deserialize_mesh(FILE* stream) {
    // 256 is a placeholder
    uint8_t line[256] = "";
    uint8_t tail = 0;
    uint8_t head = 0;
    uint8_t str[256] = "";
    while (!feof(stream)) {
        fgets((char*)line, 256, stream);
        printf("%s\n", line);

        // n
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, ' ');
        printf("%ld\n", str_to_int((uint8_t)(head-1)-tail, str));
        tail = head;

        // i
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, ' ');
        printf("%ld\n", str_to_int((uint8_t)(head-1)-tail, str));
        tail = head;

        // j
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, ' ');
        printf("%ld\n", str_to_int((uint8_t)(head-1)-tail, str));
        tail = head;

        // x
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, ' ');
        printf("%f\n", str_to_double((uint8_t)(head-1)-tail, str));
        tail = head;

        // y
        head += (uint8_t)copy_bytes(str_lenght(line), line+head, str, '\n');
        printf("%f\n", str_to_double((uint8_t)(head-1)-tail, str));
        tail = 0;
        head = 0;
    }
    return 0;
}
