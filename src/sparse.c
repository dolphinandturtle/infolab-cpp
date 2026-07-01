#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mesh.h"

double bell(double x, double y) {
    return exp(-10*(x*x + y*y));
}

int get_matrix(const double kappa, const double h, double (*f)(double, double), const uint64_t lenght, double matrix[lenght*lenght][lenght*lenght], double rhs[lenght*lenght]) {
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
    for (uint64_t i = 0; i < lenght; i++) {
        for (uint64_t j = 0; j < lenght; j++) {
            if (i >= 1) {
                matrix[i*lenght+j][(i-1)*lenght+j] = kappa/(h*h);
            }
            if (i + 1 < lenght) {
                matrix[i*lenght+j][(i+1)*lenght+j] = kappa/(h*h);
            }
            if (j >= 1) {
                matrix[i*lenght+j][i*lenght+j-1] = kappa/(h*h);
            }
            if (j + 1 < lenght) {
                matrix[i*lenght+j][i*lenght+j+1] = kappa/(h*h);
            }
            matrix[i*lenght+j][i*lenght+j] = -4*kappa/(h*h);
            // Dummy code soon to be changed.
            rhs[i*lenght+j] = -f((double)i/(double)lenght, (double)j/(double)lenght);
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

int main(void) {

    FILE* stream = NULL;

    uint64_t lenght = 16;
    double matrix[lenght*lenght][lenght*lenght];
    double rhs[lenght*lenght];
    for (uint64_t i = 0; i < lenght * lenght; i++) {
        rhs[i] = 0;
        for (uint64_t j = 0; j < lenght * lenght; j++) {
            matrix[i][j] = 0;
        }
    }
    get_matrix(10, 0.01, &bell, lenght, matrix, rhs);
    stream = fopen("A.txt", "w");
    serialize_matrix(stream, lenght, matrix);
    fclose(stream);

    stream = fopen("rhs.txt", "w");
    serialize_column(stream, lenght, rhs);
    fclose(stream);

    return 0;
}
