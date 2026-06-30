#include <stdio.h>
#include <stdint.h>

struct Coord {
    double x;
    double y;
};

uint64_t width_mesh(const uint64_t count);
uint64_t height_mesh(const uint64_t count);
uint64_t size_mesh(const uint64_t count);
void init_mesh(const uint64_t count, uint8_t* buffer);
int serialize_mesh(FILE* stream, const uint64_t count, uint8_t* buffer);
