#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
        const uint64_t lenght = str_to_int(str_lenght((uint8_t*)argv[1]), (uint8_t*)argv[1]);
        FILE* stream = NULL;

        // ha vinto lord std, malloc infine serve...
        // pero' sono stato molto responsabile
        uint8_t* buffer_mesh = malloc(size_mesh(lenght));
        init_mesh(lenght, buffer_mesh);
        stream = fopen("coords.txt", "w");
        serialize_mesh(stream, lenght, buffer_mesh);
        fclose(stream);

        // stream = fopen("coords.txt", "r");
        // uint64_t count2 = size_mesh_serialized(stream);
        // fclose(stream);
        // 
        // stream = fopen("coords.txt", "r");
        // uint8_t buffer2[size_mesh(count2)];
        // deserialize_mesh(stream, count2, buffer2);
        // fclose(stream);

        uint8_t* buffer_graph = malloc(size_graph(lenght));
        stream = fopen("connectivity.txt", "w");
        init_graph(buffer_mesh, lenght, buffer_graph);
        serialize_graph(stream, lenght, buffer_graph);
        fclose(stream);

        free(buffer_mesh);
        free(buffer_graph);

        return 0;
    }
}
