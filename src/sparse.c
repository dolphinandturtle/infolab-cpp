#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "mesh.h"


double bell(double x, double y) {
    return exp(-10*(x*x + y*y));
}

int get_matrix(const uint8_t* buffer_mesh, const double kappa, double (*f)(double, double), const uint64_t lenght, double matrix[lenght*lenght][lenght*lenght], double rhs[lenght*lenght]) {
    #ifdef DEBUG
    for (uint64_t i = 0; i < lenght * lenght; i++) {
        for (uint64_t j = 0; j < lenght * lenght; j++) {
            if (matrix[i][j] != 0) {
                printf("[DEBUG] <get_matrix> Matrix must be zero initialized.");
                return -1;
            }
        }
    }
    #endif
    struct Coord (*mesh)[lenght] = (struct Coord (*)[lenght])buffer_mesh;
    const double hsq = 1.0 / (double)((lenght + 1)*(lenght + 1));
    for (uint64_t i = 0; i < lenght; i++) {
        for (uint64_t j = 0; j < lenght; j++) {
            if (i >= 1) {
                matrix[mesh[i][j].n][mesh[i-1][j].n] = kappa/hsq;
            }
            if (i + 1 < lenght) {
                matrix[mesh[i][j].n][mesh[i+1][j].n] = kappa/hsq;
            }
            if (j >= 1) {
                matrix[mesh[i][j].n][mesh[i][j-1].n] = kappa/hsq;
            }
            if (j + 1 < lenght) {
                matrix[mesh[i][j].n][mesh[i][j+1].n] = kappa/hsq;
            }
            matrix[mesh[i][j].n][mesh[i][j].n] = -4*kappa/hsq;
            rhs[mesh[i][j].n] = -f(mesh[i][j].x, mesh[i][j].y);
        }
    }
    return 0;
}

int serialize_matrix(FILE* stream, const uint64_t lenght, double matrix[lenght*lenght][lenght*lenght]) {
    int error = 0;
    for (uint64_t i = 0; i < lenght * lenght; i++) {
        for (uint64_t j = 0; j < lenght * lenght; j++) {
            error = fprintf(stream, "%ld %ld %f\n", i+1, j+1, matrix[i][j]);
            if (error < 0) {
                return error;
            }
        }
    }
    return 0;
}

int serialize_column(FILE* stream, const uint64_t lenght, double column[lenght*lenght]) {
    int error = 0;
    for (uint64_t i = 0; i < lenght * lenght; i++) {
        error = fprintf(stream, "%ld %f\n", i+1, column[i]);
        if (error < 0) {
            return error;
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
    const uint64_t lenghtsq = lenght * lenght;
    fclose(stream);

    double (*matrix)[lenghtsq] = malloc(lenghtsq * lenghtsq * sizeof(double));
    double* rhs = malloc(lenghtsq * sizeof(double));
    for (uint64_t i = 0; i < lenghtsq; i++) {
        rhs[i] = 0;
        for (uint64_t j = 0; j < lenghtsq; j++) {
            matrix[i][j] = 0;
        }
    }

    stream = fopen(path, "r");
    uint8_t* buffer_mesh = malloc(size_mesh(lenght));
    deserialize_mesh(stream, lenght, buffer_mesh);
    fclose(stream);

    get_matrix(buffer_mesh, 0.01, &bell, lenght, matrix, rhs);

    stream = fopen("A.txt", "w");
    serialize_matrix(stream, lenght, matrix);
    fclose(stream);

    stream = fopen("rhs.txt", "w");
    serialize_column(stream, lenght, rhs);
    fclose(stream);

    free(matrix);
    free(rhs);
    free(buffer_mesh);

    return 0;
}
