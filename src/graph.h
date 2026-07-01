#include <stdio.h>
#include <stdint.h>


struct Edge {
    uint64_t start;
    uint64_t end;
};

uint64_t count_edges(const uint64_t lenght);
uint64_t size_graph(const uint64_t lenght);
void init_graph(const uint8_t* buffer_mesh, const uint64_t lenght, uint8_t* buffer_graph);
int serialize_graph(FILE* stream, const uint64_t lenght, uint8_t* buffer);
