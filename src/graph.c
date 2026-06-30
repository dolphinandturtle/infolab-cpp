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
    // Boilerplate
    const uint64_t lenght = count + 2;
    struct Edge *graph = (struct Edge *)buffer;

    // Corners

    // 0 0
    graph[0].start = 0;
    graph[1].start = 0;
    graph[0].end = 1;
    graph[1].end = lenght;
    // x 0
    graph[2].start = lenght - 1;
    graph[3].start = lenght - 1;
    graph[2].end = lenght - 2;
    graph[3].end = 2 * lenght - 1;
    // 0 y
    graph[4].start = lenght * (lenght - 1);
    graph[5].start = lenght * (lenght - 1);
    graph[4].end = lenght * (lenght - 2);
    graph[5].end = lenght * (lenght - 1) + 1;
    // x y
    graph[6].start = lenght * lenght - 1;
    graph[7].start = lenght * lenght - 1;
    graph[6].end = lenght * (lenght - 1) - 1;
    graph[7].end = lenght * lenght - 2;

    // Edges

    // Top
    for (uint64_t j = 1; j < lenght - 1; j++) {
        graph[2*j+6].start = j;
        graph[2*j+7].start = j;
        graph[2*j+6].end = j - 1;
        graph[2*j+7].end = j + 1;
    }
    // Left
    for (uint64_t i = 1; i < lenght - 1; i++) {
        // Sono stanco sono le 5 del mattino vado a letto... :3
        continue;
    }
    // Right
    for (uint64_t i = 1; i < lenght - 1; i++) {
        continue;
    }
    // Bottom
    for (uint64_t j = 1; j < lenght - 1; j++) {
        continue;
    }

    // Interior
    for (uint64_t i = 1; i < lenght - 1; i++) {
        for (uint64_t j = 1; j < lenght - 1; j++) {
            continue;
    }

    return;
}
