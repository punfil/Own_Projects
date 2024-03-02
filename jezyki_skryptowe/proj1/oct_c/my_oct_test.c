#include "my_oct.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void test_single_number(int64_t number) {
    unsigned char *oct_num = my_oct(number);
    fprintf(stdout, "%s\n", oct_num);
    free(oct_num);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1;
    }

    int64_t number = atoll(argv[1]);

    test_single_number(number);

    return 0;
}
