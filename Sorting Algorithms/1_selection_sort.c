#include "data_loader.h"
#include "time.h"

/*
    Selection sort works by finding the smallest value in the data set and then
    swapping it with the value in the first index of an array. Then we find the 
    second smallest value which is then swapped with the second index of the array,
    and so on and so forth repeating the process until the array is sorted. 

    It has a time complexity of O(n^2) because of its double loop feature: first
    because we need to iterate through each index to sort, and second because we
    need to search for the smallest value. When we search, we also iterate through
    the whole array.
*/

int main() {
    int* integer_array = populate_random_integer_array();

    int swap_index = -1;
    int temp = 0;

    clock_t elapsed_time;

    elapsed_time = clock();
    printf("Starting selection sort...\n\n");

    //       |--- START OF THE ALGORITHM ---|
    for(int i = 0; i < DATA_SIZE - 1; i++) {
        /*
            We always assume that the first index is sorted and is the smallest
            value.
        */ 
        swap_index = i;

        // Then we iterate through the unsorted part of the array
        for(int j = i + 1; j < DATA_SIZE; j++) {
            /*
                We compare the value in index i to the value in the next index. 
            */ 
            if(integer_array[j] <= integer_array[swap_index]) {
                /*
                    If we find a value smaller than the value in index i, we change
                    the value of swapIndex. This takes note of the index of the
                    smallest value (note that our variable i is still the same 
                    because it is where we will swap the smallest value).
                */
                swap_index = j;
            }
        }

        /*
            Here, we simply swap their values and then iterates further until
            the array is sorted.
        */
        temp = integer_array[i];
        integer_array[i] = integer_array[swap_index];
        integer_array[swap_index] = temp;
    }
    //       |--- END OF THE ALGORITHM ---|

    elapsed_time = clock() - elapsed_time;

    printf("Sorting time: %f", (double)elapsed_time / CLOCKS_PER_SEC);

    free(integer_array);

    return 0;
}


/*
    Optimization is possible but it will not have a drastic change. 

    We can achieve optimization by terminating the inner loop when we have indeed 
    found the smallest value in the array. This requires that we know what the 
    possible smallest  values are. In the implementation here, our range is 0 and 
    all positive numbers until one million. But we do not need to go through them 
    all one-by-one. We can simply store the last known smallest value in a variable 
    and if we find that again in the array, then we can terminate the inner loop.

    Note that this optimization tip is data-specific and is not a general optimization
    method especially for more complicated data, to which I assume where selection
    sort is less of an option.
*/