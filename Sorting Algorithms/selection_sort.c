#include "data_loader.h"
#include "time.h"

/*
    Selection sort works by...

    It has a time complexity of O(n^2).
*/

int main() {
    int* integer_array = populate_random_integer_array();

    int swapIndex = -1;
    int temp = 0;

    clock_t time;

    time = clock();
    printf("Starting the sort...\n\n");
    for(int i = 0; i < DATA_SIZE - 1; i++) {
        // We assume first that the first index of the array is the smallest element
        swapIndex = i;

        // Then we iterate the whole array to find the smallest element
        for(int j = i + 1; j < DATA_SIZE; j++) {
            if(integer_array[j] <= integer_array[swapIndex]) {
                swapIndex = j;
            }
        }
        temp = integer_array[i];
        integer_array[i] = integer_array[swapIndex];
        integer_array[swapIndex] = temp;
    }
    time = clock() - time;

    printf("Sorting time: %f", (double)time / CLOCKS_PER_SEC);

    free(integer_array);

    return 0;
}