#!/usr/bin/env bash

# MAKEFILE ???
DEBUG_FLAGS="-fsanitize=leak -g -O0"


gcc -Wall $DEBUG_FLAGS main.c parser.c file.c hash_table.c command_table.c -o assembler
