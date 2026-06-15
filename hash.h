#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#define HASH_TABLE_SIZE 503

typedef struct HashNode{
    char train_id[16];
    int index;
    struct HashNode* next;
} HashNode;

typedef struct{
    HashNode** buckets;
    int size;
}HashTable;

int hash(const char* train_id, int table_size);
void build_hash_table(HashTable* table, Train trains[], int count);
int hash_lookup(HashTable* table, const char* train_id);
#endif