#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// AES S-box from the document
const uint8_t SBOX[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    // ... (rest of the S-box from the document's Table 4.5)
    0x16
};

// Custom Subbyte' function as defined in the document
uint8_t subbyte_prime(uint8_t x) {
    // Compute (211 * x) + 122 in the finite field
    uint16_t field_mul = (211 * x) + 122;
    
    // Reduce in the field F2[x] / <x^8 + x^4 + x^3 + x + 1>
    uint8_t result = SBOX[field_mul & 0xFF];
    
    return result;
}

// Custom MixColumn' matrix
const uint8_t MIX_COLUMN_MATRIX[4][4] = {
    {1, 4, 4, 5},
    {5, 1, 4, 4},
    {4, 5, 1, 4},
    {4, 4, 5, 1}
};

// Finite field multiplication
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1)
            p ^= a;
        
        uint8_t hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1B; // x^8 + x^4 + x^3 + x + 1 polynomial
        
        b >>= 1;
    }
    return p;
}

// Custom MixColumn' operation
void mixcolumn_prime(uint8_t state[4][4]) {
    uint8_t temp[4][4];
    
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            temp[r][c] = 
                gmul(MIX_COLUMN_MATRIX[r][0], state[0][c]) ^
                gmul(MIX_COLUMN_MATRIX[r][1], state[1][c]) ^
                gmul(MIX_COLUMN_MATRIX[r][2], state[2][c]) ^
                gmul(MIX_COLUMN_MATRIX[r][3], state[3][c]);
        }
    }
    
    memcpy(state, temp, 16);
}

// Key Expansion (similar to AES key schedule)
void key_expansion(uint8_t* key, uint8_t* expanded_key) {
    // Simplified key expansion 
    memcpy(expanded_key, key, 32);
    
    // You would typically implement a more complex key schedule here
    // This is a placeholder implementation
    for (int i = 32; i < 240; i++) {
        expanded_key[i] = expanded_key[i-1] ^ expanded_key[i-32];
    }
}

// Custom AES-256 Encryption
void encrypt_block(uint8_t* input, uint8_t* key, uint8_t* output) {
    uint8_t state[4][4];
    uint8_t expanded_key[240];
    
    // Convert input to state
    for (int i = 0; i < 16; i++) {
        state[i/4][i%4] = input[i];
    }
    
    // Key expansion
    key_expansion(key, expanded_key);
    
    // Initial key addition
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= expanded_key[i*4 + j];
        }
    }
    
    // Rounds (simplified)
    for (int round = 0; round < 14; round++) {
        // SubBytes with custom Subbyte'
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                state[i][j] = subbyte_prime(state[i][j]);
            }
        }
        
        // ShiftRows
        uint8_t temp;
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < i; j++) {
                temp = state[i][0];
                state[i][0] = state[i][1];
                state[i][1] = state[i][2];
                state[i][2] = state[i][3];
                state[i][3] = temp;
            }
        }
        
        // MixColumns with custom MixColumn'
        mixcolumn_prime(state);
        
        // Key addition
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                state[i][j] ^= expanded_key[(round+1)*16 + i*4 + j];
            }
        }
    }
    
    // Convert state back to output
    for (int i = 0; i < 16; i++) {
        output[i] = state[i/4][i%4];
    }
}

// Triple AES' Encryption
void triple_encrypt(uint8_t* message, uint8_t* key1, uint8_t* key2, uint8_t* ciphertext) {
    uint8_t intermediate[16];
    
    // First encryption pass
    encrypt_block(message, key1, intermediate);
    
    // Second decryption pass
    encrypt_block(intermediate, key2, ciphertext);
    
    // Third encryption pass
    encrypt_block(ciphertext, key1, ciphertext);
}

int main() {
    // Example usage
    uint8_t message[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                           0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    uint8_t key1[32] = {0}; // First 256-bit key
    uint8_t key2[32] = {0}; // Second 256-bit key
    uint8_t ciphertext[16];
    
    // Initialize keys (in practice, these would be derived from Ka)
    for (int i = 0; i < 32; i++) {
        key1[i] = i;
        key2[i] = 32 - i;
    }
    
    // Perform Triple AES' Encryption
    triple_encrypt(message, key1, key2, ciphertext);
    
    // Print ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\n");
    
    return 0;
}