#ifndef __COMMAND_TABLE_H_
#define __COMMAND_TABLE_H_

#include "hash_table.h"

void init_command_ht();
void free_command_ht();

extern hash_table* jump_ht;
extern hash_table* dest_ht;
extern hash_table* comp_ht;

extern hash_table* symbol_ht;

#endif // __COMMAND_TABLE_H_
