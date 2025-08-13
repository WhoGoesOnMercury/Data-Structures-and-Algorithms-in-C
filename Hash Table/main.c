/*
    This is a hashtable implementation in C based on James Routley's tutorial. A 
    more in-depth description of the codebase will be located in the link below.
        
    Here is a link to the Github repository: https://github.com/jamesroutley/write-a-hash-table
    Here is a link to an e-portfolio-like website of his: https://routley.io/

*/

#include "hash_table.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_INPUT 256

// Helper to remove newline from fgets
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    ht_hash_table* ht = ht_new();

    int running = 1, commandInput = -1;
    char key[MAX_INPUT], value[MAX_INPUT];

    printf("Initializing base hashtable...\n");
    Sleep(500); 

    while (running) {
        printf("\n=== HASH TABLE MENU ===\n");
        printf("[1] INSERT KEY-VALUE PAIR\n");
        printf("[2] SEARCH USING KEY\n");
        printf("[3] DELETE KEY-VALUE PAIR\n");
        printf("[4] EXIT\n");
        printf("Choose Command: ");

        if (scanf("%d", &commandInput) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n'); 
            continue;
        }
        while (getchar() != '\n'); 

        switch (commandInput) {
            case 1: 
                printf("Enter key: ");
                if (fgets(key, sizeof(key), stdin)) {
                    trim_newline(key);
                }
                printf("Enter value: ");
                if (fgets(value, sizeof(value), stdin)) {
                    trim_newline(value);
                }
                ht_insert(ht, key, value);
                printf("Inserted (%s, %s)\n", key, value);
                break;

            case 2:
                printf("Enter key: ");
                if (fgets(key, sizeof(key), stdin)) {
                    trim_newline(key);
                }
                {
                    char* result = ht_search(ht, key);
                    if (result) {
                        printf("Value found: %s\n", result);
                    } else {
                        printf("Key not found.\n");
                    }
                }
                break;

            case 3:
                printf("Enter key: ");
                if (fgets(key, sizeof(key), stdin)) {
                    trim_newline(key);
                }
                ht_delete(ht, key);
                printf("Deleted key: %s\n", key);
                break;

            case 4:
                running = 0;
                break;

            default:
                printf("Invalid input.\n");
        }
    }

     // Delete the hashtable here; memory-safe
    printf("Deleting hashtable...\n");
    Sleep(500);
    ht_del_hash_table(ht);
    printf("Exiting the application...\n");
    Sleep(500);

    return 0;
}
