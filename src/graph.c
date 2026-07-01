#include <stdio.h>
#include <stdint.h>
#include "mesh.h"
#include "graph.h"

uint64_t count_edges(const uint64_t lenght) {
    const uint64_t n_vrt = 4;
    const uint64_t n_brd = 4 * (lenght - 2);
    const uint64_t n_int = (lenght - 2) * (lenght - 2);
    return (n_vrt * 2 + n_brd * 3 + n_int * 4);
}

uint64_t size_graph(const uint64_t lenght) {
    return sizeof(struct Edge) * count_edges(lenght);
}

void init_graph(const uint8_t* buffer_mesh, const uint64_t lenght, uint8_t* buffer_graph) {
    struct Edge *graph = (struct Edge *)buffer_graph;
    struct Coord (*mesh)[lenght] = (struct Coord (*)[lenght])buffer_mesh;
    uint64_t offset = 0;

    // Upper left corner
    graph[0].start = mesh[0][0].n;
    graph[1].start = mesh[0][0].n;
    graph[0].end = mesh[0][1].n;
    graph[1].end = mesh[1][0].n;

    // Upper right corner
    graph[2].start = mesh[0][lenght-1].n;
    graph[3].start = mesh[0][lenght-1].n;
    graph[2].end = mesh[1][lenght-1].n;
    graph[3].end = mesh[0][lenght-2].n;

    // Lower left corner
    graph[4].start = mesh[lenght-1][0].n;
    graph[5].start = mesh[lenght-1][0].n;
    graph[4].end = mesh[lenght-1][1].n;
    graph[5].end = mesh[lenght-2][0].n;

    // Lower right corner
    graph[6].start = mesh[lenght-1][lenght-1].n;
    graph[7].start = mesh[lenght-1][lenght-1].n;
    graph[6].end = mesh[lenght-1][lenght-2].n;
    graph[7].end = mesh[lenght-2][lenght-1].n;

    // Top row
    offset = 8;
    for (uint64_t j = 1; j < lenght - 1; j++) {
        graph[3*(j-1)+offset+0].start = mesh[0][j].n;
        graph[3*(j-1)+offset+1].start = mesh[0][j].n;
        graph[3*(j-1)+offset+2].start = mesh[0][j].n;
        graph[3*(j-1)+offset+0].end = mesh[0][j-1].n;
        graph[3*(j-1)+offset+1].end = mesh[1][j].n;
        graph[3*(j-1)+offset+2].end = mesh[0][j+1].n;
    }
     
    // Left column
    offset += 3 * (lenght-2);
    for (uint64_t i = 1; i < lenght - 1; i++) {
        graph[3*(i-1)+offset+0].start = mesh[i][0].n;
        graph[3*(i-1)+offset+1].start = mesh[i][0].n;
        graph[3*(i-1)+offset+2].start = mesh[i][0].n;
        graph[3*(i-1)+offset+0].end = mesh[i-1][0].n;
        graph[3*(i-1)+offset+1].end = mesh[i][1].n;
        graph[3*(i-1)+offset+2].end = mesh[i+1][0].n;
    }
    
    // Right column
    offset += 3 * (lenght-2);
    for (uint64_t i = 1; i < lenght - 1; i++) {
        graph[3*(i-1)+offset+0].start = mesh[i][lenght-1].n;
        graph[3*(i-1)+offset+1].start = mesh[i][lenght-1].n;
        graph[3*(i-1)+offset+2].start = mesh[i][lenght-1].n;
        graph[3*(i-1)+offset+0].end = mesh[i-1][lenght-1].n;
        graph[3*(i-1)+offset+1].end = mesh[i][lenght-2].n;
        graph[3*(i-1)+offset+2].end = mesh[i+1][lenght-1].n;
    }
    
    // Bottom row
    offset += 3 * (lenght-2);
    for (uint64_t j = 1; j < lenght - 1; j++) {
        graph[3*(j-1)+offset+0].start = mesh[lenght-1][j].n;
        graph[3*(j-1)+offset+1].start = mesh[lenght-1][j].n;
        graph[3*(j-1)+offset+2].start = mesh[lenght-1][j].n;
        graph[3*(j-1)+offset+0].end = mesh[lenght-1][j-1].n;
        graph[3*(j-1)+offset+1].end = mesh[lenght-2][j].n;
        graph[3*(j-1)+offset+2].end = mesh[lenght-1][j+1].n;
    }
    
    // Interior
    offset += 3 * (lenght-2);
    for (uint64_t i = 1; i < lenght - 1; i++) {
        for (uint64_t j = 1; j < lenght - 1; j++) {
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+0].start = mesh[i][j].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+1].start = mesh[i][j].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+2].start = mesh[i][j].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+3].start = mesh[i][j].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+0].end = mesh[i][j+1].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+1].end = mesh[i+1][j].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+2].end = mesh[i][j-1].n;
            graph[4*(lenght-2)*(i-1)+4*(j-1)+offset+3].end = mesh[i-1][j].n;
        }
    }
    offset += 4 * (lenght - 2) * (lenght - 2);
    printf("%ld vs. %ld\n", offset, count_edges(lenght));
    return;
}

int serialize_graph(FILE* stream, const uint64_t lenght, uint8_t* buffer) {
    // Boilerplate
    struct Edge *graph = (struct Edge *)buffer;
    int error = 0;
    // TODO: only boundary
    for (uint64_t i = 0; i < count_edges(lenght); i++) {
        error = fprintf(stream, "%ld %ld %ld\n", i, graph[i].start, graph[i].end);
        if (error < 0) {
            return error;
        }
    }
    return 0;
}
