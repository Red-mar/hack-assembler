#include "main.h"

#include "command_table.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Please input a file as the first argument\n");
        return 1;
    }
    char* filepath = argv[1];


    printf("Loading: %s as file!\n", filepath);

    FILE* file;
    file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error loading file: %s\n", filepath);
        return 1;
    }

    int ret = 0;
    char* buffer;
    buffer = (char *) malloc(5096);
    ret = read_file(file, &buffer, 5096);

#ifdef DEBUG
    printf("Parsing file: \n\n");
    printf("%s", buffer);
    printf("\nEnd File\n");
#endif

    init_command_ht();
    parse_file(buffer);
    free(buffer);
    free_command_ht();

    ret = fclose(file);
    if (ret) {
        printf("Error closing file: %s\n", filepath);
    }

    printf("Program ended successfully.\n");
    return 0;
}
