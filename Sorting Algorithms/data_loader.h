#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Set to preferred data size and maximum integer value—not exceeding int limit
//                                                      Otherwise, just use the
//                                                      unsigned int data type.
#define DATA_SIZE 900000        
#define MAX_INT_VALUE 1000000   

int* populate_random_integer_array() {
    int* data_array = malloc(DATA_SIZE * sizeof(int));
    int random_value = 0;

    if(!data_array) {
        exit(EXIT_FAILURE);
    }

    FILE *fptr;
    fptr = fopen("unsorted_integer_array.txt", "w");

    srand (time(NULL));
    for(int i = 0; i < DATA_SIZE; i++) {
        /*
            RAND_MAX is equal to 32767 per Windows OS. Using the code in this
            comment:
                                rand() % MAX_INT_VALUE
                    will not result to an integer larger than RAND_MAX. 

            The line of code as used in this function sets up two rand() functions 
            to reach MAX_INT_VALUE. This code is made simply for this function. A 
            different and more apt approach would be to use bitwise operations. 
            Below is an example that can cover 32-bits. 
                                (rand() << 15)  | rand()

        */ 
        random_value = (rand() * (RAND_MAX + 1) + rand()) % MAX_INT_VALUE;
        data_array[i] = random_value;
        fprintf(fptr, "%d\n", random_value);
    }

    fclose(fptr);

    return data_array;
}


