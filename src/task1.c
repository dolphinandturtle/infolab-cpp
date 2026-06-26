#include <stdio.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No arguments have been supplied...");
        return 1;
    }
    else if (argc > 2) {
        printf("To many arguments have been supplied...");
        return 2;
    }
    else {
        return 0;
    }
}
