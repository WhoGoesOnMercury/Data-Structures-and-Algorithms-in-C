#include "data_loader.h"

int main() {
    int* integer_array = populate_random_integer_array();

    free(integer_array);

    return 0;
}