#include <vector>
#include <cstdint>
#include <string>
#include <omp.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <assert.h>
#include <array>

using namespace std;

vector<array<uint16_t, 16>> string_to_uint16t_vector(const string str) {
    vector<uint16_t> result;
    uint16_t num = 0;
    uint64_t chars_done = 0;
    for (char c : str) {
        if (!(chars_done%2)) {
            num = static_cast<uint16_t>(c);
            num = num << 8;
        } else {
            num += static_cast<uint16_t>(c);
            result.push_back(num);
        }

        chars_done++;
    }

    if (chars_done % 2) {
        num += 0x80;
        result.push_back(num);
    } else {
        result.push_back(0x8000);
    }

    if (result.size() % 16 || !result.size()) {
        while (result.size() % 16) {
            result.push_back(0x0000);
        }
    }

    reverse(result.begin(), result.end());

    vector<array<uint16_t, 16>> out;
    while (result.size()) {
        array<uint16_t, 16> middle;
        for (uint8_t i = 0; i < 16; i++) {
            middle[i] = result.back();
            result.erase(result.end());
        }

        out.push_back(middle);
    }

    return out;
}
