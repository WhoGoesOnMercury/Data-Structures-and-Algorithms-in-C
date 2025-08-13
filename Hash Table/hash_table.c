#include "hash_table.h"

/*
    Creates a new item struct
*/
static ht_item* ht_new_item(const char* key, const char* value) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(key);
    i->value = strdup(value);
    return i;
}

/*
    Frees struct variables first before the whole struct
*/
static void ht_del_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

/*
    Essentially deletes hash table by first deleting the items
*/
void ht_del_hash_table(ht_hash_table* hashtable) {
    for (int i = 0; i < hashtable->size; i++) {
        ht_item * item = hashtable->items[i];
        if(item != NULL) {
            ht_del_item(item);
        }
    } 
    free(hashtable->items);
    free(hashtable);
}

/*
    Hashing function; 'a' should be a number larger than the alphabet size of ASCII (128); also should be a prime
*/
static int ht_hash(const char* string, const int a, const int bucket_count) {
    long hash = 0;
    const int string_length = strlen(string);
    for (int i = 0; i < string_length; i++) {
        hash += (long)pow(a, string_length - (i+1)) * string[i];
        hash = hash % bucket_count;
    }
    return (int)hash;
}

/*
    Function to handle collisions
*/
static int ht_get_hash(const char* string, const int bucket_count, const int collisions) {
    const int hash_a = ht_hash(string, HT_PRIME_1, bucket_count);
    const int hash_b = ht_hash(string, HT_PRIME_2, bucket_count);

    return (hash_a + (collisions * (hash_b + 1))) % bucket_count;
}

/*
    Resize hash table 
*/
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = base_size;

    ht->size = next_prime(ht->size);

    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

ht_hash_table* ht_new() {
    return ht_new_sized((int)HT_INITIAL_BASE_SIZE);
}

static void ht_resize(ht_hash_table* hashtable, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < hashtable->size; i++) {
        ht_item* item = hashtable->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    hashtable->size = new_ht->size;
    hashtable->count = new_ht->count;

    const int tmp_size = hashtable->size;
    hashtable->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = hashtable->items;
    hashtable->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* hashtable) {
    const int new_size = hashtable->size * 2;
    ht_resize(hashtable, new_size);
}

static void ht_resize_down(ht_hash_table* hashtable) {
    const int new_size = hashtable->size / 2;
    ht_resize(hashtable, new_size);
}

/*
    Function to insert a new key-value pair
*/
void ht_insert(ht_hash_table* hashtable, const char* key, const char* value) {
    const int load = hashtable->count * 100 / hashtable->size;
    if (load > 70) {
        ht_resize_up(hashtable);
    }

    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, hashtable->size, 0);
    ht_item* current_item = hashtable->items[index];
    int i = 1;
    while (current_item != NULL) {
        if (current_item != &HT_DELETED_ITEM) {
            if (strcmp(current_item->key, key) == 0) {
                ht_del_item(current_item);
                hashtable->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, hashtable->size, i);
        current_item = hashtable->items[index];
        i++;
    }
    hashtable->items[index] = item;
    hashtable->count++;
}

/*
    Search the table if key exists then get the value
*/
char* ht_search(ht_hash_table* hashtable, const char* key) {
    int index = ht_get_hash(key, hashtable->size, 0);
    ht_item* item = hashtable->items[index];
    int i = 1;
    while (item != NULL) {
        if(item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
            index = ht_get_hash(key, hashtable->size, i);
            item = hashtable->items[index];
            i++;
        }
    }
    return NULL;
}

/*
    Simply mark an item as HT_DELETED_ITEM since item can be part of a collision chain
*/
void ht_delete(ht_hash_table* hashtable, const char* key) {
    const int load = hashtable->count * 100 / hashtable->size;
    if (load < 10) {
        ht_resize_up(hashtable);
    }
    
    int index = ht_get_hash(key, hashtable->size, 0);
    ht_item* item = hashtable->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                hashtable->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, hashtable->size, i);
        item = hashtable->items[index];
        i++;
    }
    hashtable->count--;
}



