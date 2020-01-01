#include "file.h"

int read_file(FILE* file, char** buffer, int buffer_size) {
    int ret = 0;
    int lines = 0;
    // 128 character until realloc;
    int line_buffer_size = 128;
    while (ret != EOF) {
        char* line_buffer;
        line_buffer = (char*) malloc(line_buffer_size);
        ret = read_line(file, &line_buffer, line_buffer_size);
        strcat(*buffer, line_buffer);
        free(line_buffer);
        lines++;
        if (buffer_size <= line_buffer_size * lines) {
            int new_buffer_size = buffer_size * 2;
            #ifdef DEBUG
            printf("Reallocating file buffer to %d.\n", new_buffer_size);
            #endif
            *buffer = (char *)realloc(*buffer, new_buffer_size);
            if (!*buffer) {
                printf("Could not allocate buffer.\n");
                return 1;
            }
            buffer_size = new_buffer_size;
        }
    }
    return ret;
}

int read_line(FILE* file, char** buffer, int buffer_size) {
    char c = ' ';
    char end_marker = '\n';
    int buffer_i = 0;
    int ret;
    while (c != end_marker) {
        ret = getc(file);
        if (ret == EOF) break;
        c = (char) ret;
        *(*buffer + buffer_i) = c;
        buffer_i++;
        if(buffer_size <= buffer_i) {
            int new_buffer_size = buffer_size * 2;
            #ifdef DEBUG
            printf("Reallocating line buffer to %d.\n", new_buffer_size);
            #endif
            *buffer = (char *)realloc(*buffer, new_buffer_size);
            if (!*buffer) {
                printf("Could not allocate buffer.\n");
                return 1;
            }
            buffer_size = new_buffer_size;
        }

    }
    *(*buffer + buffer_i) = '\0';

    return ret;
}
