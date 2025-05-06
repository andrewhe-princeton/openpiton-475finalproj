// size_t if not typically available or to avoid stddef.h
typedef unsigned long size_t; // Adjust type based on processor (e.g., unsigned int)

// NULL if not available
#ifndef NULL
#define NULL ((void *)0)
#endif

#define ARRAY_SIZE 100000      // Size of the array to search CHANGE THIS!
#define NUM_SEARCHES  30 // How many searches to perform for blsenchmark

// Placed globally to avoid large stack allocation
int sorted_array[ARRAY_SIZE];

// Comparison function for integers
int compare_ints(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;

    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// Manual implementation of binary search
void* my_bsearch(const void *key, const void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)) {
    size_t low = 0;
    size_t high = nmemb; // one past the end for initial high

    if (nmemb == 0) return NULL;

    while (low < high) {
        size_t mid = low + (high - low) / 2; // overflow
        const char *mid_ptr = (const char *)base + mid * size;
        int cmp = compar(key, mid_ptr);

        if (cmp < 0) {
            high = mid; // Key is in the lower half
        } else if (cmp > 0) {
            low = mid + 1; // Key is in the upper half
        } else {
            return (void *)mid_ptr; // Key found
        }
    }

    return NULL; // Key not found
}


int main(void) {
    size_t i;
    int key;
    void *result;
    volatile int found_count = 0; // Use volatile to prevent optimization

    // Initialize the sorted array
    for (i = 0; i < ARRAY_SIZE; ++i) {
        sorted_array[i] = i * 2;
    }

    for (i = 0; i < NUM_SEARCHES; ++i) {
        // Search for an existing element
        key = sorted_array[ARRAY_SIZE / 2];
        result = my_bsearch(&key, sorted_array, ARRAY_SIZE, sizeof(int), compare_ints);
        if (result != NULL) {
            found_count++;
        }

        // Search fora non-existing element
        key = sorted_array[ARRAY_SIZE / 3] + 1;
        result = my_bsearch(&key, sorted_array, ARRAY_SIZE, sizeof(int), compare_ints);
        if (result == NULL) {
           // outcome for non-existing key
           found_count--;
        }

         // look for the first element
        key = sorted_array[0];
        result = my_bsearch(&key, sorted_array, ARRAY_SIZE, sizeof(int), compare_ints);
        if (result != NULL) {
            found_count++;
        }

        // Search for the last element
        key = sorted_array[ARRAY_SIZE - 1];
        result = my_bsearch(&key, sorted_array, ARRAY_SIZE, sizeof(int), compare_ints);
        if (result != NULL) {
            found_count++;
        }
    }

    return 0; 
}