#include "main.h"

#include "command_table.h"

int main(int argc, char** argv) {
    char* filepath;
    char* output_filename;
    if(argc < 2) {
        printf("Please input a file as the first argument\n");
        return 1;
    }
    if (argc > 2) {
        output_filename = argv[2];
    } else {
        output_filename = "output.hack";
    }
    filepath = argv[1];

    FILE* file;
    file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error loading file: %s\n", filepath);
        return 1;
    }

    int ret = 0;
    char* buffer;
    char* code;
    buffer = (char *) malloc(2000000);
    code = (char*) malloc(2000000);
    ret = read_file(file, &buffer, 2000000);

#ifdef DEBUG
    printf("Parsing file: \n\n");
    printf("%s", buffer);
    printf("\nEnd File\n");
#endif

    init_command_ht();
    parse_file(buffer, code);

#ifdef DEBUG
    printf("FINAL CODE\n%s\n",code);
#endif

    FILE* output_file;
    output_file = fopen(output_filename, "w+");
    fputs(code, output_file);

    free(buffer);
    free(code);
    free_command_ht();

    ret = fclose(file);
    if (ret) {
        printf("Error closing file: %s\n", filepath);
    }
    ret = fclose(output_file);
    if (ret) {
        printf("Error closing file: %s\n", output_filename);
    }

    printf("Program ended successfully.\n");
    return 0;
}
