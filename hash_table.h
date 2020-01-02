#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define HT_PRIME 131
#define HT_PRIME2 137

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} hash_table;

hash_table* new_hash_table(int size);
ht_item* new_ht_item(const char* key, const char* value);

ht_item* get_ht_item(hash_table* ht, const char* key);
void insert_ht_item(hash_table* ht, const char* key, const char* value);
void delete_ht_item(hash_table* ht, const char* key);
void free_ht(hash_table* ht);
#endif // __HASH_TABLE_H_
