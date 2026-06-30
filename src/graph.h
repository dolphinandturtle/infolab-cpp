#include <stdio.h>
#include <stdint.h>


struct Edge {
    uint64_t start;
    uint64_t end;
};

uint64_t size_graph(const uint64_t count);
void init_graph(const uint64_t count, uint8_t* buffer);
int serialize_graph(FILE* stream, const uint64_t count, uint8_t* buffer);
