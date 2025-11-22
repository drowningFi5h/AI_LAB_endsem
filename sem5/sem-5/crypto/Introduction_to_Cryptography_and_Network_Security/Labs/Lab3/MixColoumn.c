#include <stdio.h>

// Define the AES Galois Field (GF) multiplication function
unsigned char aes_gf_multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char mask = 0x01;
    unsigned char high_bit_set;

    for (int i = 0; i < 8; ++i) {
        if (b & mask) {
            result ^= a;
        }

        high_bit_set = (a & 0x80);
        a <<= 1;

        if (high_bit_set) {
            a ^= 0x1b;  // This is the irreducible polynomial x^8 + x^4 + x^3 + x + 1
        }

        mask <<= 1;
    }

    return result;
}

// Mix a single column using the AES MixColumns operation
static void aes_mix_single_column(unsigned char state[4]) {
    unsigned char temp[4];

    temp[0] = aes_gf_multiply(0x02, state[0]) ^ aes_gf_multiply(0x03, state[1]) ^ state[2] ^ state[3];
    temp[1] = state[0] ^ aes_gf_multiply(0x02, state[1]) ^ aes_gf_multiply(0x03, state[2]) ^ state[3];
    temp[2] = state[0] ^ state[1] ^ aes_gf_multiply(0x02, state[2]) ^ aes_gf_multiply(0x03, state[3]);
    temp[3] = aes_gf_multiply(0x03, state[0]) ^ state[1] ^ state[2] ^ aes_gf_multiply(0x02, state[3]);

    for (int i = 0; i < 4; i++) {
        state[i] = temp[i];
    }
}

int main() {
    // Example input column
    unsigned char column[4] = {175, 152, 227, 110}; // Decimal values

    // Call aes_mix_single_column function
    aes_mix_single_column(column);

    // Output the modified column after MixColumns operation
    printf("Column after MixColumns operation:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d\n", column[i]);
    }

    return 0;
}
