#include "hash_table.h"

/*
    This function creates a new hash table item
*/
static ht_item* ht_new_item(const char* key, const char* value) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(key);
    i->value = strdup(value);
    return i;
}

/*
    Frees struct variables first before the whole struct. Memory management in C
    structs needs you to first "free" or deallocate the variables inside the
    struct. 
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
    This is our hashing function.Theoretically, a hashing function's output should 
    be unique for every input key. This ideal scenario is what we call an independent 
    uniform hash function or a random oracle. It is not something that can be 
    reasonably implemented.
    
    In the hashing function below, the key-string is converted to a large integer
    which then undergoes the modulo operator, utilizing the bucket_count of the
    hash table. It makes use of a prime number (a) that is larger than the alphabet
    size of ASCII (128). Why so? If we were to set (a) to a lower value than 
    the chosen alphabet size, then it is possible to generate hash values that
    populate only the "lower" indices of the hash table or in more technical
    terms, it will lead to a higher rate of collisions, reducing the efficiency
    of the hash table.
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
    In practice, a hashing function can have an input key (key1) and another 
    input key (key2)—wherein both of them are different values but still managed
    to produce the same hash value. This is called a collision. If a collision
    was to occur, what do we do? Overwrite the existing index? That solution
    can lead to loss of data which is not optimal. 

    There are multiple ways to address collisions. One of them, as seen here, is
    through the use of open addressing through double hashing. Double hashing 
    uses the idea of using an offset. Hash A is an index and if that index is occupied,
    we "offset" the index by another Hash B. 

    Open addressing, on the other hand, is a method that resizes the hash table to 
    fit the total number of keys. 

    In the implementation below, the hash value is simply hashed twice but does not
    check whether that index is occupied.
*/
static int ht_get_hash(const char* string, const int bucket_count, const int collisions) {
    const int hash_a = ht_hash(string, HT_PRIME_1, bucket_count);
    const int hash_b = ht_hash(string, HT_PRIME_2, bucket_count);

    return (hash_a + (collisions * (hash_b + 1))) % bucket_count;
}

/*
    This function essentially resizes the hash table to contain more values.
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


/*
    Below are the resize functions but are unused in this implementation. 
*/
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
    One of the key characteristics of a hash table is its O(1) search-time
    complexity. This can only be achieved through the presumption that you are
    searching with a key. 
    
    As stated, each "index" in the hash table is simply a mathematically-computed 
    hash value. Given this idea, we can simply ask for the key again, compute the 
    hash value, and then retrieve the value at the address. Here, there are no 
    comparisons nor should any arrays or array-like data structures are iterated through. 
    
    In theory, basic mathematical operations is O(1). Since hashing functions are 
    simply mathematical operations, computing the hash from a key is also O(1)—
    we already get the index from this computation.
*/
char* ht_search(ht_hash_table* hashtable, const char* key) {
    int index = ht_get_hash(key, hashtable->size, 0);
    ht_item* item = hashtable->items[index];
    int i = 1;

    /*
        In the implementation here below, you see a loop. As stated, different
        input keys can yield the same hash output. Here, we simply iterate through
        each item with the same hash output and then compare the keys. Again,
        a key can only hold one value.

        In a related discussion, this is how collisions can influence the efficiency
        of a hash table: if many different keys can yield the same hash value, 
        then that means we have to iterate through the list of items in that
        hash value to find what we are searching for. Relatively, this means that
        a hash table at its worst (through poor hashing functions resulting to 
        plentiful collisions) has a time complexity of O(n) where n is simply
        the number of collisions. 
    */
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
    Simply mark an item as HT_DELETED_ITEM or NULL
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



