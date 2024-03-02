#include "my_oct.h"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>

#define NUMBERS_FILE "../../numbers_generated.txt"
#define MAX_LINE_LENGTH 40

struct ret_val {
    int64_t *num_ptr;
    uint64_t num_cnt;
};

struct ret_val *read_numbers_from_file() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int64_t *numbers = NULL;
    uint64_t num_elements = 0;
    uint64_t array_size = 0;

    struct ret_val *ret_val = calloc(1, sizeof(struct ret_val));
    if (!ret_val) {
        fprintf(stderr, "Cannot allocate enough memory!\n");
        return NULL;
    }

    file = fopen(NUMBERS_FILE, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        free(ret_val);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        int64_t number;
        if (sscanf(line, "%" SCNd64, &number) == 1) {
            if (num_elements >= array_size) {
                array_size += 1000;
                numbers = realloc(numbers, array_size * sizeof(int64_t));
                if (numbers == NULL) {
                    fprintf(stderr, "Error reallocating memory\n");
                    fclose(file);
                    free(ret_val);
                    return NULL;
                }
            }

            numbers[num_elements] = number;
            num_elements++;
        } else {
            fprintf(stderr, "Error converting line to int64_t: %s\n", line);
        }
    }

    fclose(file);

    ret_val->num_ptr = numbers;
    ret_val->num_cnt = num_elements;
    return ret_val;
}

int main(int argc, char **argv) {
    struct ret_val *dec_numbers = read_numbers_from_file();
    uint16_t N = 6000;
    struct timespec start, end, checkpoint, resolution;

    clock_getres(CLOCK_MONOTONIC, &resolution);
    printf("Clock resolution: %ld nanoseconds\n", resolution.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < N; i++) {
        for (uint64_t j = 0; j < dec_numbers->num_cnt; j++) {
            free(my_oct(dec_numbers->num_ptr[j]));
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &checkpoint);

    for (int i = 0; i < N; i++) {
        for (uint64_t j = 0; j < dec_numbers->num_cnt; j++) {
            __asm__ __volatile__(
                "NOP"
            );
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_diff_1 = (checkpoint.tv_sec - start.tv_sec) + (checkpoint.tv_nsec - start.tv_nsec) / 1e9;
    double time_diff_2 = (end.tv_sec - checkpoint.tv_sec) + (end.tv_nsec - checkpoint.tv_nsec) / 1e9;

    free(dec_numbers->num_ptr);
    free(dec_numbers);

    printf("Time difference: %.9f seconds\n", time_diff_1);
    printf("Time difference: %.9f seconds\n", time_diff_2);
}
