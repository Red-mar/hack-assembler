#ifndef __PARSER_H_
#define __PARSER_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#include "command_table.h"

#define VARIABLE_MEMORY_OFFSET 16

char* parse_file(char* buffer, char* code);
enum command{NOT_SET=0,A_COMMAND=1,L_COMMAND=2,C_COMMAND=3};
enum command parse_command(const char* line);

// NOTE: DOES NOT CHECK BOUNDS
int parse_symbol(const char* line, char* symbol, enum command cmd);
int parse_dest(const char* line, char* dest);
int parse_comp(const char* line, char* comp);
int parse_jump(const char* line, char* jump);
int parse_comments(char* line);

char* parse_c_command(const char* dest, const char* comp, const char* jump, char* return_string);
char* parse_a_command(hash_table* ht, char* symbol, char* return_string);
void parse_l_command(hash_table* ht, char* symbol, int lines);

#endif // __PARSER_H_
