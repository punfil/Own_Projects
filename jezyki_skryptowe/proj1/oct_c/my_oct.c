#include "my_oct.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_OCT_NUMBER_LENGTH 50

void strrev(char *str) {
    int length, start, end;

    if (!str) {
        fprintf(stderr, "NULL ptr passed.\n");
        return;
    }

    length = strlen(str);
    if (length <= 1) {
        return;
    }

    start = 0;
    end = length - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *my_oct(int64_t dec_num) {
    char *oct_num;
    bool is_positive;
    uint32_t length = 0;
    
    if (!dec_num) {
        return "0o0";
    }

    oct_num = calloc(1, MAX_OCT_NUMBER_LENGTH);
    if (!oct_num) {
        fprintf(stderr, "Not enough memory.\n");
        goto err;
    }

    is_positive = dec_num > 0 ? true : false;
    if (!is_positive) {
        dec_num = -dec_num;
    }

    while (dec_num && length < MAX_OCT_NUMBER_LENGTH - 3) {
        oct_num[length++] = (dec_num % 8) + '0';
        dec_num /= 8;
    }

    oct_num[length] = '\0';
    strcat(oct_num, is_positive ? "o0" : "o0-");
    strrev(oct_num);

    return oct_num;
err:
    return NULL;
}
