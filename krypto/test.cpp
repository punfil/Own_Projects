#include <iostream>
#include <vector>
#include <cstdint>
#include "common.h"

#include "hash_function.cpp"

using namespace std;

void initial_test() {
    array<uint16_t, 16> m = {
        0x0001, 0x0203, 0x0405, 0x0607, 
        0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 
        0x1011, 0x1213, 0x1415, 0x1617, 
        0x1819, 0x1A1B, 0x1C1D, 0x1E1F
    };

    array<uint16_t, 8> valid = {0xF089, 0x4377, 0x32AC, 0x4197, 0x63C3, 0x975A, 0x15CD, 0xDD5B};

    array<uint16_t, 8> w = {0};
    
    BlockHashFunction bhf = BlockHashFunction(0, w, m);
    auto out = bhf.calculate_hash();
    (void)out;
    assert(bhf.compare_hash(valid));
}

void test_hash_function() {
    vector<string> inputs = {
		"",
		"AbCxYz",
		"1234567890",
		"Ala ma kota, kot ma ale.",
		"Ty, ktory wchodzisz, zegnaj sie z nadzieja.",
		"Litwo, Ojczyzno moja! ty jestes jak zdrowie;"
	};

    array<uint16_t, 8> w = {0};
    BlockHashFunction bhf = BlockHashFunction(0, w, string_to_uint16t_vector(inputs[1])[0]);
    auto out = bhf.calculate_hash();
    for (const auto a: out) {
           printf("%x ", a);
        }
        printf("\n");
}

int main() {
    //initial_test();
    //cout << "Test 1 passed!" << endl;
    test_hash_function();
    return 0;
}