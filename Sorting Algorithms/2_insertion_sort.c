#include "data_loader.h"
#include "time.h"

/*
    Insertion sort works by selecting the first unsorted value and inserting it
    to the sorted part of the array. This can be done two ways: first by iteratively
    swapping the unsorted to value until it has reached a point where it is sorted.
    Second is by finding the index where it should be, swap it there, and move
    the larger values to a higher index. In this implementation, we use the first.

    It has a time complexity of O(n^2) because of its double loop feature. 
*/

int main() {
     int* integer_array = populate_random_integer_array();

    int swap_index = -1, unsorted_array_index = -1;
    int temp = 0, inner_loop = 0;

    clock_t elapsed_time;

    elapsed_time = clock();
    printf("Starting insertion sort...\n\n");

    //       |--- START OF THE ALGORITHM ---|
    for(int i = 0; i < DATA_SIZE - 1; i++) {
        /*
            We assume that the index i is already sorted
        */
        swap_index = i;

        /*
            Check only i + 1, the first unsorted value
        */
        unsorted_array_index = i + 1;
        
        inner_loop = 1;
        while(swap_index != -1 && inner_loop == 1) {
            if(integer_array[unsorted_array_index] >= integer_array[swap_index]) {
                /*
                    Stop the inner loop because the value is already sorted at 
                    this point.
                */
                inner_loop = 0;
            } else {
                /*
                    If the value in the unsorted index is smaller, then swap to the
                    left until it has reached a point where it is larger or equal to
                    it.                
                */
                temp = integer_array[swap_index];
                integer_array[swap_index] = integer_array[unsorted_array_index];
                integer_array[unsorted_array_index] = temp;

                /*
                    We decrement both so we move along the sorted array
                */
                unsorted_array_index--;
                swap_index--;
            }
        }
    }
    //       |--- END OF THE ALGORITHM ---|
    elapsed_time = clock() - elapsed_time;

    printf("Sorting time: %f", (double)elapsed_time / CLOCKS_PER_SEC);

    free(integer_array);

    return 0;
}