#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#include "query.h"
#include "hash.h"

int hash(const char* train_id, int table_size){
    int sum = 0;
    for(int i = 0; train_id[i] != '\0'; i++){
        sum += train_id[i];
    }
    int key = sum % table_size;
    return key;
}

void build_hash_table(HashTable* table, Train trains[], int count){
    table->size = HASH_TABLE_SIZE;
    table->buckets = (HashNode**)malloc(sizeof(HashNode*)*HASH_TABLE_SIZE);
    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        table->buckets[i] = NULL;
    }
    for(int i = 0; i < count; i++){
        int bucket = hash(trains[i].train_id, HASH_TABLE_SIZE);
        HashNode* node = (HashNode*)malloc(sizeof(HashNode));
        strcpy(node->train_id, trains[i].train_id);
        node->index = i;
        node->next = NULL;
        
        if(table->buckets[bucket] == NULL){
            table->buckets[bucket] = node;
        }else{
            HashNode* p = table->buckets[bucket];
            while(p->next != NULL){
                p = p->next;
            }
            p->next = node;
        }
    }
}

int hash_lookup(HashTable* table, const char* train_id){
    int bucket = hash(train_id, table->size);
    HashNode*p = table->buckets[bucket];
    while(p != NULL){
        if(strcmp(p->train_id, train_id) == 0){
            return p->index;
        }
        p = p->next;
    }
    return -1;
}