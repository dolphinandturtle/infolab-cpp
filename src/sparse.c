#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "mesh.h"


double bell(double x, double y) {
    return exp(-10*(x*x + y*y));
}

int genlinsys(FILE* stream_A, FILE* stream_rhs, const uint8_t* buffer_mesh, const double kappa, double (*f)(double, double), const uint64_t lenght) {
    struct Coord (*mesh)[lenght] = (struct Coord (*)[lenght])buffer_mesh;
    const double hsq = 1.0 / (double)((lenght + 1)*(lenght + 1));
    int error = 0;
    for (uint64_t i = 0; i < lenght; i++) {
        for (uint64_t j = 0; j < lenght; j++) {
            if (i >= 1) {
                error = fprintf(stream_A, "%ld %ld %f\n", mesh[i][j].n, mesh[i-1][j].n, kappa/hsq);
                if (error < 0) {
                    return error;
                }
            }
            if (i + 1 < lenght) {
                error = fprintf(stream_A, "%ld %ld %f\n", mesh[i][j].n, mesh[i+1][j].n, kappa/hsq);
                if (error < 0) {
                    return error;
                }
            }
            if (j >= 1) {
                error = fprintf(stream_A, "%ld %ld %f\n", mesh[i][j].n, mesh[i][j-1].n, kappa/hsq);
                if (error < 0) {
                    return error;
                }
            }
            if (j + 1 < lenght) {
                error = fprintf(stream_A, "%ld %ld %f\n", mesh[i][j].n, mesh[i][j+1].n, kappa/hsq);
                if (error < 0) {
                    return error;
                }
            }
            error = fprintf(stream_A, "%ld %ld %f\n", mesh[i][j].n, mesh[i][j].n, -4*kappa/hsq);
            if (error < 0) {
                return error;
            }
            error = fprintf(stream_rhs, "%ld %f\n", mesh[i][j].n, -f(mesh[i][j].x, mesh[i][j].y));
            if (error < 0) {
                return error;
            }
        }
    }
    return 0;
}

int main(int argc, char** argv) {

    FILE* stream = NULL;

    if (argc != 2) {
        printf("Needs coords.txt...\n");
        return -1;
    }

    const char* path = argv[1];

    stream = fopen(path, "r");
    const uint64_t lenght = size_mesh_serialized(stream);
    fclose(stream);

    stream = fopen(path, "r");
    uint8_t* buffer_mesh = malloc(size_mesh(lenght));
    deserialize_mesh(stream, lenght, buffer_mesh);
    fclose(stream);

    FILE* stream_A = fopen("A.txt", "w");
    FILE* stream_rhs = fopen("rhs.txt", "w");
    genlinsys(stream_A, stream_rhs, buffer_mesh, 0.01, &bell, lenght);
    fclose(stream_A);
    fclose(stream_rhs);

    free(buffer_mesh);

    return 0;
}
