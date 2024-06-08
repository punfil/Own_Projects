#include <iostream>
#include "hash_function.cpp"
#include "common.h"
#include <string>

using namespace std;

string n_variation(uint64_t &n, string letters, int m)
{
    int baseSize = static_cast<int>(letters.size());
    string variation = "";
    for (int i = 0; i < m; i++)
    {
        variation.push_back(letters[n % baseSize]);
        n /= baseSize;
    }

    return variation;
}

int main() {
    omp_set_num_threads(12);
    vector<uint16_t> lengths = { 2, 3, 4, 5, 6, 7, 8 };
    std::vector<std::array<uint16_t, 8>> hashes = {
        {0x2B9B, 0xDD28, 0x94E4, 0x9CB6, 0xD9E8, 0x4606, 0xE33C, 0xB38E},
        {0x2846, 0xF826, 0x0C12, 0xD04E, 0xA0C7, 0x8626, 0xF3E3, 0x58EA},
        {0x5E4A, 0x81F2, 0x3402, 0x94FF, 0x587D, 0x3565, 0xCA75, 0xBFBF},
        {0x5B0B, 0x5C4D, 0x60CD, 0x10CF, 0xA234, 0xFA3D, 0x2F23, 0xD208},
        {0x5B87, 0x893E, 0x4B04, 0x5758, 0xB55A, 0x3C85, 0xAB4A, 0x1F92},
        {0x5B5C, 0xF9FF, 0x40AB, 0x4E96, 0x6CB1, 0x3408, 0x5BF7, 0x5D09},
        {0xDD07, 0xDC53, 0x77A4, 0xB184, 0x58DA, 0x298C, 0x0653, 0xFF18}
    };

    string letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*-_=+([{<)]}>'\";:?,.\\/|";

    for (size_t i = 0; i < lengths.size(); i++)
    {
        auto start = chrono::high_resolution_clock::now();
        std::cout << lengths[i] << "-element variations..." << endl;
        uint64_t num_variations = static_cast<int64_t>(pow(letters.size(), lengths[i]));
        std::cout << "No variations: " << num_variations << endl;
        bool found = false;

        #pragma omp parallel for schedule(static)
        for (uint64_t j = 0; j < num_variations; j++) {
            if (found) {
                continue;
            }

            string variation = n_variation(j, letters, lengths[i]);
            array<uint16_t, 8> w = {0};
            auto bhf = BlockHashFunction(0, w, string_to_uint16t_vector(variation)[0]);
            array<uint16_t, 8> hash = bhf.calculate_hash();
            (void)hash;
            if (bhf.compare_hash(hashes[i])) {
                #pragma omp critical
                {
                    std::cout << "Found: " << variation << endl;
                    found = true;
                }
            }
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration<double>(end - start);
        std::cout << "Took: " << duration.count() << " seconds" << endl;
        std::cout << "-------------------------------------------------" << endl;
    }

    return 0;
}