#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "mesh.h"
#include "graph.h"


#define ERROR_NOT_ENOUGH_ARGUMENTS 1
#define ERROR_TO_MANY_ARGUMENTS 2

const char *DEBUG_MESSAGES[] = {

    [ERROR_NOT_ENOUGH_ARGUMENTS]
    ="No arguments have been supplied...\n",

    [ERROR_TO_MANY_ARGUMENTS]
    ="To many arguments have been supplied...\n"
};

int main(int argc, char** argv) {
    if (argc == 1) {
        printf(DEBUG_MESSAGES[ERROR_NOT_ENOUGH_ARGUMENTS]);
        return ERROR_NOT_ENOUGH_ARGUMENTS;
    }
    else if (argc > 2) {
        printf(DEBUG_MESSAGES[ERROR_TO_MANY_ARGUMENTS]);
        return ERROR_TO_MANY_ARGUMENTS;
    }
    else {
        const uint64_t count = str_to_int(str_lenght((uint8_t*)argv[1]), (uint8_t*)argv[1]);
        FILE* stream = NULL;

        uint8_t buffer[size_mesh(count)];
        init_mesh(count, buffer);
        stream = fopen("coords.txt", "w");
        serialize_mesh(stream, count, buffer);
        fclose(stream);

        stream = fopen("coords.txt", "r");
        deserialize_mesh(stream);
        fclose(stream);

        uint8_t buffer_graph[size_graph(count)];
        stream = fopen("connectivity.txt", "w");
        serialize_graph(stream, count, buffer_graph);
        fclose(stream);
        return 0;
    }
}
