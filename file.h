#ifndef __FILE_H_
#define __FILE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Give reference to buffer so it can be dynamically reallocated.
int read_file(FILE* a, char** buffer, int buffer_size);
int read_line(FILE* a, char** buffer, int buffer_size);

#endif // __FILE_H_
