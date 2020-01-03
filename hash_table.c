#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static int pow_int(int a, int b) {
    int ret = 1;
    for (int i = 0; i < b; i++){
        ret = ret * a;
    }
    return ret;
}

static int get_hash(const char* string, const int mod, const int size) {
    int length = strlen(string);
    int hash = 0;
    for(int i = 0; i < length; i++) {
        hash += pow_int(mod, length - (i + 1)) * (unsigned char)string[i];
        hash %= size;
    }
    return hash;
}

static int get_ht_hash(const char* string, const int size, const int attempts) {
    const int hash1 = get_hash(string, HT_PRIME, size);
    const int hash2 = get_hash(string, HT_PRIME2, size);
    return (hash1 + (attempts * (hash2 + 1))) % size;
}

static void free_ht_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

static int get_free_hash(hash_table* ht, const char* key) {
    int attempts = 0;
    int hash = 0;
    while (attempts < ht->size) {
        hash = get_ht_hash(key,ht->size,attempts);
        if(ht->items[hash] == NULL ||
           ht->items[hash] == &HT_DELETED_ITEM) {
            return hash;
        // If it is not null but the key is the same replace
        } else if (strcmp(ht->items[hash]->key,key) == 0) {
            free_ht_item(ht->items[hash]);
            return hash;
        }
        attempts++;
    }
    //resize
    printf("\033[1;31mFREEHASH: HASHTABLE TOO SMALL!!!\033[0m\n");
    return -1;
}

static int get_occupied_hash(hash_table* ht, const char* key) {
    int attempts = 0;
    int hash = 0;
    while (attempts < ht->size) {
        hash = get_ht_hash(key,ht->size,attempts);
        if(ht->items[hash] != NULL && strcmp(ht->items[hash]->key,key)==0) {
            return hash;
        }
        attempts++;
    }
    return -1;
}

ht_item* new_ht_item(const char* key, const char* value) {
    ht_item* item = (ht_item*)malloc(sizeof(ht_item));
    item->key = strdup(key);
    item->value = strdup(value);
    return item;
}

hash_table* new_hash_table(int size) {
    hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
    ht->size = size;
    ht->count = 0;
    ht->items = calloc(size, sizeof(ht_item*));
    return ht;
}

ht_item* get_ht_item(hash_table* ht, const char* key) {
    if(strcmp(key, "")==0) return NULL;
    int hash = get_occupied_hash(ht, key);
    if(hash == -1) return NULL;
    if (ht->items[hash] == &HT_DELETED_ITEM) return NULL;
    return ht->items[hash];
}

void insert_ht_item(hash_table* ht, const char* key, const char* value) {
    int hash = get_free_hash(ht, key);
    ht_item* item = new_ht_item(key, value);
    ht->items[hash] = item;
}

void delete_ht_item(hash_table* ht, const char* key) {
    if (get_ht_item(ht, key) == NULL) return;
    int hash = get_occupied_hash(ht, key);
    if (ht->items[hash] == &HT_DELETED_ITEM) return;
    free_ht_item(ht->items[hash]);
    ht->items[hash] = &HT_DELETED_ITEM;
}


void free_ht(hash_table* ht) {
    for(int i = 0; i < ht->size; i++) {
        if(ht->items[i] != NULL &&
           ht->items[i] != &HT_DELETED_ITEM) {
            free_ht_item(ht->items[i]);
        }
    }
    free(ht->items);
    free(ht);
}
