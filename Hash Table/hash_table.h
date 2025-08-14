#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "prime.h"

/* 
    Any prime larger than 128 is to be used because we are making use of the 
    ASCII alphabet. 
*/
#define HT_PRIME_1 373
#define HT_PRIME_2 613
#define HT_INITIAL_BASE_SIZE 53

/*
    This struct basically defines a key-value pair. 
*/
typedef struct {
    char* key;
    char* value;
} ht_item;

/*
    A hash table is essentially composed of a collection of key-value pairs. Here,
    we also include the size of the hash table or the "buckets" as well as the 
    "count" or the number of items in the hash table.
*/
typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static ht_item HT_DELETED_ITEM = {NULL, NULL};

void ht_insert(ht_hash_table* hashtable, const char* key, const char* value);
char* ht_search(ht_hash_table* hashtable, const char* key);
void ht_delete(ht_hash_table* hahstable, const char* key);
void ht_del_hash_table(ht_hash_table* hashtable);
ht_hash_table* ht_new();