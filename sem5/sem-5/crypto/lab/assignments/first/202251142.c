#include <stdio.h>
#include <stdint.h>

// Substitution box and permutation box for the encryption algorithm
uint8_t sbox[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
uint8_t inv_sbox[16] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};

uint8_t perm[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
uint8_t inv_perm[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

// Function to convert a 4-character hexadecimal string to a 16-bit unsigned integer
uint16_t hex_to_uint16(const char* hex_str) {
    uint16_t val = 0;
    for (int i = 0; i < 4; i++) {
        val <<= 4;
        if (hex_str[i] >= 'A')
            val |= hex_str[i] - 'A' + 10;
        else
            val |= hex_str[i] - '0';
    }
    return val;
}

// Key scheduling function that derives 5 round keys from the 32-bit master key
void key_scheduling(uint32_t key, uint16_t round_keys[5]) {
    round_keys[0] = (key >> 16) & 0xFFFF; // First 16 bits of the key
    round_keys[1] = key & 0xFFFF;         // Last 16 bits of the key
    round_keys[2] = round_keys[0] ^ round_keys[1];
    round_keys[3] = round_keys[1] ^ round_keys[2];
    round_keys[4] = round_keys[2] ^ round_keys[3];
}

// Function to apply the S-box (or inverse S-box if 'inverse' is set)
uint16_t sbox_transform(uint16_t input, int inverse) {
    uint16_t result = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t nibble = (input >> (12 - 4 * i)) & 0xF;
        result |= (inverse ? inv_sbox[nibble] : sbox[nibble]) << (12 - 4 * i);
    }
    return result;
}

// Function to apply permutation (or inverse permutation if 'inverse' is set)
uint16_t permute(uint16_t input, int inverse) {
    uint16_t result = 0;
    for (int i = 0; i < 16; i++) {
        int bit = (input >> (15 - i)) & 1;
        int new_pos = (inverse ? inv_perm[i] : perm[i]) - 1;
        result |= bit << (15 - new_pos);
    }
    return result;
}

// Function to encrypt plaintext using the round keys
uint16_t encrypt(uint16_t plaintext, uint16_t round_keys[5]) {
    uint16_t state = plaintext;
    for (int i = 0; i < 4; i++) {
        state ^= round_keys[i];            // XOR with round key
        state = sbox_transform(state, 0);  // Apply S-box
        state = permute(state, 0);         // Apply permutation
    }
    state ^= round_keys[4];                // XOR with final round key
    return state;
}

// Function to decrypt ciphertext using the round keys
uint16_t decrypt(uint16_t ciphertext, uint16_t round_keys[5]) {
    uint16_t state = ciphertext;
    state ^= round_keys[4];                // Apply final round key
    for (int i = 3; i >= 0; i--) {
        state = permute(state, 1);         // Apply inverse permutation
        state = sbox_transform(state, 1);  // Apply inverse S-box
        state ^= round_keys[i];            // XOR with round key
    }
    return state;
}

int main() {
    char key_hex[9], plaintext_hex[5];
    uint32_t master_key;
    uint16_t plaintext, ciphertext, decrypted;
    uint16_t round_keys[5];

    // Input for 32-bit key
    printf("Enter the 32-bit hexadecimal key (for example, 12345678): ");
    scanf("%8s", key_hex);
    sscanf(key_hex, "%x", &master_key);

    // Generate round keys using the new key scheduling logic
    key_scheduling(master_key, round_keys);

    // Input for 16-bit plaintext
    printf("Enter the 16-bit plaintext in hexadecimal (for example, FACE): ");
    scanf("%4s", plaintext_hex);
    plaintext = hex_to_uint16(plaintext_hex);

    // Encrypt
    ciphertext = encrypt(plaintext, round_keys);
    printf("Ciphertext: %04X\n", ciphertext);

    // Display round keys
    printf("Round Keys:\n");
    for (int i = 0; i < 5; i++) {
        printf("K%d: %04X\n", i + 1, round_keys[i]);
    }

    // Decrypt
    decrypted = decrypt(ciphertext, round_keys);
    printf("Decrypted text: %04X\n", decrypted);

    return 0;
}
