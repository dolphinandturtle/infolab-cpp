#include <stdio.h>
#include <stdint.h>

struct Coord {
    double x;
    double y;
};

void write_mesh(FILE *stream, uint64_t count) {
    double h = 1.0 / (count + 1.0);
    uint64_t n = 0;
    for (uint64_t j = 0; j <= count + 1; j++) {
        for (uint64_t i = 0; i <= count + 1; i++) {
            fprintf(stream, "%d %d %d %f %f\n", n, i, j, i * h, j * h);
            n++;
        }
    }
    return;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No arguments have been supplied...\n");
        return 1;
    }
    else if (argc > 2) {
        printf("To many arguments have been supplied...\n");
        return 2;
    }
    else {
        FILE *stream = fopen("coords.txt", "w");
        write_mesh(stream, 100);
        return 0;
    }
}
