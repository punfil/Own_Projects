#include <array>
#include <cstdint>
#include <cstring>
#include <assert.h>
#include <iostream>

#define ROUNDS_COUNT 6

using namespace std;

class BlockHashFunction {
private:
    /* Inicjalna wartość stanu to w[i] = 0 dla i = 0...7 */
    array<uint16_t, 8> w;
    array<uint16_t, 16> m;

    array<array<uint16_t, 4>, 4> v;

    void G(uint16_t& a, uint16_t& b, uint16_t& c, uint16_t& d, uint16_t& x, uint16_t& y) {
        a = a + b + x;
        d = rol((d ^ a), 3);
        c = c + d;
        b = rol((b ^ c), 11);
        a = a + b + y;
        d = rol((d ^ a), 2);
        c = c + d;
        b = rol((b ^ c), 5);
    }

    void conduct_iteration() {
        /* Przeksztalcenia pionowe */
        G(v[0][0], v[1][0], v[2][0], v[3][0], m[0], m[1]);
        G(v[0][1], v[1][1], v[2][1], v[3][1], m[2], m[3]);
        G(v[0][2], v[1][2], v[2][2], v[3][2], m[4], m[5]);
        G(v[0][3], v[1][3], v[2][3], v[3][3], m[6], m[7]);

        /* Przeksztalcenia poziome */
        G(v[0][0], v[1][1], v[2][2], v[3][3], m[8], m[9]);
        G(v[0][1], v[1][2], v[2][3], v[3][0], m[10], m[11]);
        G(v[0][2], v[1][3], v[2][0], v[3][1], m[12], m[13]);
        G(v[0][3], v[1][0], v[2][1], v[3][2], m[14], m[15]);

        /* Zamien wartosci tablicy m wedlug permutacji s */
        auto m_temp = m;
        m[2] = m_temp[0];
        m[6] = m_temp[1];
        m[3] = m_temp[2];
        m[10] = m_temp[3];
        m[7] = m_temp[4];
        m[0] = m_temp[5];
        m[4] = m_temp[6];
        m[13] = m_temp[7];
        m[1] = m_temp[8];
        m[11] = m_temp[9];
        m[12] = m_temp[10];
        m[5] = m_temp[11];
        m[9] = m_temp[12];
        m[14] = m_temp[13];
        m[15] = m_temp[14];
        m[8] = m_temp[15];
    }

    uint16_t rol(uint16_t num, int shift) {
        shift = shift % 16;
        if (shift == 0) return num;

        return (num << shift) | (num >> (16 - shift));
    }

    int mod(int a, int b) {
        return ((a % b) + b) % b;
    }

    void print_m() {
        for (int i = 0; i < 16; i++) {
            printf("%x ", this->m[i]);
        }

        printf("\n");
    }

    void print_v() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%x ", this->v[i][j]);
            }
            
            printf("\n");
        }
    }

public:
    BlockHashFunction(uint16_t block_num, array<uint16_t, 8> w, array<uint16_t, 16> m) {
        if (block_num != 0) {
            for (const auto element: w) {
                if (element == 0) assert(false);
            }
        }

        /* Najpierw stan wejściowy w zamieniany jest na macierz v */
        for (uint8_t i = 0; i < 4; i++) {
            v[0][i] = w[i];
            v[1][i] = w[i+4];
        }

        v[2][0] = 0x03F4;
        v[2][1] = 0x774C;
        v[2][2] = 0x5690;
        v[2][3] = 0xC878;
        v[3][0] = 0;
        v[3][1] = block_num;
        v[3][2] = 0;
        v[3][3] = 0;

        this->m = m;
        this->w = w;
    }

    array<uint16_t, 8> calculate_hash() {
         for (uint8_t i = 0; i < ROUNDS_COUNT; i++) {
            conduct_iteration();
        }

        for (uint16_t i = 0; i < 4; i++) {
            w[i] = w[i] ^ v[0][i] ^ v[2][i];
            w[i+4] = w[i+4] ^ v[1][i] ^ v[3][i];
        }

        return w;
    }

    bool compare_hash(array<uint16_t, 8> other) {
        for (uint8_t i = 0; i < 8; i++) {
            if (__glibc_likely(other[i] != this->w[i])) {
                return false;
            }
        }

        return true;
    }
};