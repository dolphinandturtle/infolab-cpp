#include <stdio.h>
#include <stdint.h>
#include "graph.h"


static uint64_t count_edges_graph(const uint64_t count) {
    const uint64_t lenght = count + 2;
    // The sum of border nodes and interior nodes has to
    // stay consistent meaning the square of the lenght.
    const uint64_t count_border_nodes = 4 * lenght - 4;
    const uint64_t count_interior_nodes = (lenght - 2) * (lenght - 2);
    // Border has 2 times as many edges as its nodes.
    // Interior has 4 times as many edges as its nodes.
    return (2 * count_border_nodes) + (4 * count_interior_nodes);
}

uint64_t size_graph(const uint64_t count) {
    return count_edges_graph(count) * sizeof(struct Edge);
}

void init_graph(const uint64_t count, uint8_t* buffer) {
    struct Edge *graph = (struct Edge *)buffer;
    return;
}
