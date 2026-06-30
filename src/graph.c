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
    // Domanda: Perche non uso un incrementatore?
    // Risposta: Perche sono masochista >w< e mi piace la staticita'
    uint64_t offset = 0;
    uint64_t root = 0;
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
    offset = 8 - 3*1;
    for (uint64_t j = 1; j < lenght - 1; j++) {
        root = j;
        graph[3*j+offset+0].start = root;
        graph[3*j+offset+1].start = root;
        graph[3*j+offset+2].start = root;
        graph[3*j+offset+0].end = root + 1;
        graph[3*j+offset+1].end = root + lenght;
        graph[3*j+offset+2].end = root - 1;
    }
    // Left
    offset = 3 * (lenght-1) + offset - 3*1;
    for (uint64_t i = 1; i < lenght - 1; i++) {
        root = lenght * i;
        graph[3*i+offset+0].start = root;
        graph[3*i+offset+1].start = root;
        graph[3*i+offset+2].start = root;
        graph[3*i+offset+0].end = root - lenght;
        graph[3*i+offset+1].end = root + 1;
        graph[3*i+offset+2].end = root + lenght;
    }
    // Right
    offset = 3 * (lenght-1) + offset - 3*1;
    for (uint64_t i = 1; i < lenght - 1; i++) {
        root = lenght * (i + 1) - 1;
        graph[3*i+offset+0].start = root;
        graph[3*i+offset+1].start = root;
        graph[3*i+offset+2].start = root;
        graph[3*i+offset+0].end = root + lenght;
        graph[3*i+offset+1].end = root - 1;
        graph[3*i+offset+2].end = root - lenght;
    }
    // Bottom
    offset = 3 * (lenght-1) + offset - 3*1;
    for (uint64_t j = 1; j < lenght - 1; j++) {
        root = lenght * (lenght - 1) + j;
        graph[3*j+offset+0].start = root;
        graph[3*j+offset+1].start = root;
        graph[3*j+offset+2].start = root;
        graph[3*j+offset+0].end = root + 1;
        graph[3*j+offset+1].end = root - lenght;
        graph[3*j+offset+2].end = root - 1;
    }

    // Interior
    offset = 3 * (lenght-1) + offset - 4*1;
    for (uint64_t i = 1; i < lenght - 1; i++) {
        for (uint64_t j = 1; j < lenght - 1; j++) {
            root = lenght * i + j;
            graph[4*i+offset+0].start = root;
            graph[4*i+offset+1].start = root;
            graph[4*i+offset+2].start = root;
            graph[4*i+offset+3].start = root;
            graph[4*i+offset+0].end = root - lenght;
            graph[4*i+offset+1].end = root + 1;
            graph[4*i+offset+2].end = root + lenght;
            graph[4*i+offset+3].end = root - 1;
        }
    }

    return;
}

int serialize_graph(FILE* stream, const uint64_t count, uint8_t* buffer) {
    // Boilerplate
    struct Edge *graph = (struct Edge *)buffer;
    int error = 0;
    uint64_t n = 0;
    // TODO: only boundary
    for (uint64_t i = 0; i < count_edges_graph(count); i++) {
        error = fprintf(stream, "%ld %ld %ld\n", n, graph[i].start, graph[i].end);
        if (error < 0) {
            return error;
        }
        n++;
    }
    // This function shouldn't be used in its current conditions
    return -1;
}
