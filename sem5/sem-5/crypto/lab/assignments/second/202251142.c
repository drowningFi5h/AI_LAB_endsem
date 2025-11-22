#include <stdio.h>
#include <stdint.h>

// Permutation tables
const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int E[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[4] = {2, 4, 3, 1};

// S-boxes
const uint8_t S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
const uint8_t S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

void print_binary(uint16_t value, int size) {
    for (int i = size - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void decimal_to_binary(uint16_t decimal, uint8_t *binary, int size) {
    for (int i = size - 1; i >= 0; i--) {
        binary[i] = decimal & 1;
        decimal >>= 1;
    }
}

uint16_t binary_to_decimal(uint8_t *binary, int size) {
    uint16_t decimal = 0;
    for (int i = 0; i < size; i++) {
        decimal = (decimal << 1) | binary[i];
    }
    return decimal;
}

void apply_permutation(uint8_t *input, uint8_t *output, const int *permutation, int size) {
    uint8_t temp[size];
    for (int i = 0; i < size; i++) {
        temp[i] = input[permutation[i] - 1];
    }
    for (int i = 0; i < size; i++) {
        output[i] = temp[i];
    }
}

void apply_shift(uint8_t *input, uint8_t *output, int size) {
    for (int i = 0; i < 5; i++) {
        output[i] = input[(i + 1) % 5];
        output[i + 5] = input[((i + 1) % 5) + 5];
    }
}

void apply_double_shift(uint8_t *input, uint8_t *output, int size) {
    for (int i = 0; i < 5; i++) {
        output[i] = input[(i + 2) % 5];
        output[i + 5] = input[((i + 2) % 5) + 5];
    }
}

void xor_arrays(uint8_t *a, uint8_t *b, uint8_t *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = a[i] ^ b[i];
    }
}

void apply_sbox(uint8_t *input, uint8_t *output, const uint8_t sbox[4][4]) {
    int row = input[0] * 2 + input[1];
    int col = input[2] * 2 + input[3];
    uint8_t value = sbox[row][col];
    output[0] = (value >> 1) & 1;
    output[1] = value & 1;
}

void generate_round_keys(uint16_t key, uint8_t *k1, uint8_t *k2) {
    uint8_t key_bin[10], p10_out[10], shift_out[10], p8_out[8];
    
    decimal_to_binary(key, key_bin, 10);
    printf("key: ");
    print_binary(key, 10);
    
    apply_permutation(key_bin, p10_out, P10, 10);
    printf("P10(K): ");
    print_binary(binary_to_decimal(p10_out, 10), 10);
    
    apply_shift(p10_out, shift_out, 10);
    printf("shift(P10(K)): ");
    print_binary(binary_to_decimal(shift_out, 10), 10);
    
    apply_permutation(shift_out, k1, P8, 8);
    printf("P8(shift(P10(K))) -> k1: ");
    print_binary(binary_to_decimal(k1, 8), 8);
    
    apply_double_shift(p10_out, shift_out, 10);
    printf("shift^2(P10(K)): ");
    print_binary(binary_to_decimal(shift_out, 10), 10);
    
    apply_permutation(shift_out, k2, P8, 8);
    printf("P8(shift^2(P10(K))) -> k2: ");
    print_binary(binary_to_decimal(k2, 8), 8);
}

void function_f(uint8_t *right_half, uint8_t *round_key, uint8_t *output) {
    uint8_t expanded[8], xor_result[8], s0_in[4], s1_in[4], s0_out[2], s1_out[2], s_out[4];
    
    apply_permutation(right_half, expanded, E, 8);
    printf("E(R): ");
    print_binary(binary_to_decimal(expanded, 8), 8);
    
    xor_arrays(expanded, round_key, xor_result, 8);
    printf("E(R) ^ k: ");
    print_binary(binary_to_decimal(xor_result, 8), 8);
    
    for (int i = 0; i < 4; i++) {
        s0_in[i] = xor_result[i];
        s1_in[i] = xor_result[i + 4];
    }
    
    apply_sbox(s0_in, s0_out, S0);
    apply_sbox(s1_in, s1_out, S1);
    
    s_out[0] = s0_out[0];
    s_out[1] = s0_out[1];
    s_out[2] = s1_out[0];
    s_out[3] = s1_out[1];
    
    printf("S(E(R) ^ k): ");
    print_binary(binary_to_decimal(s_out, 4), 4);
    
    apply_permutation(s_out, output, P4, 4);
    printf("P4(S(E(R) ^ k)): ");
    print_binary(binary_to_decimal(output, 4), 4);
}

uint8_t encrypt(uint8_t plaintext, uint16_t key) {
    uint8_t plain_bin[8], temp[8], left[4], right[4];
    uint8_t k1[8], k2[8];
    uint8_t f_out[4], xor_out[4];
    
    printf("Encryption Process\n");
    generate_round_keys(key, k1, k2);
    
    printf("plaintext: ");
    print_binary(plaintext, 8);
    
    decimal_to_binary(plaintext, plain_bin, 8);
    apply_permutation(plain_bin, temp, IP, 8);
    printf("IP(P): ");
    print_binary(binary_to_decimal(temp, 8), 8);
    
    for (int i = 0; i < 4; i++) {
        left[i] = temp[i];
        right[i] = temp[i + 4];
    }
    
    printf("L1: ");
    print_binary(binary_to_decimal(left, 4), 4);
    printf("R1: ");
    print_binary(binary_to_decimal(right, 4), 4);
    
    function_f(right, k1, f_out);
    xor_arrays(left, f_out, xor_out, 4);
    
    printf("L2: ");
    print_binary(binary_to_decimal(right, 4), 4);
    printf("R2: ");
    print_binary(binary_to_decimal(xor_out, 4), 4);
    
    for (int i = 0; i < 4; i++) {
        left[i] = right[i];
        right[i] = xor_out[i];
    }
    
    function_f(right, k2, f_out);
    xor_arrays(left, f_out, xor_out, 4);
    
    printf("L3: ");
    print_binary(binary_to_decimal(xor_out, 4), 4);
    printf("R3: ");
    print_binary(binary_to_decimal(right, 4), 4);
    
    for (int i = 0; i < 4; i++) {
        temp[i] = xor_out[i];
        temp[i + 4] = right[i];
    }
    
    printf("output (y): ");
    print_binary(binary_to_decimal(temp, 8), 8);
    
    apply_permutation(temp, plain_bin, IP_INV, 8);
    
    uint8_t ciphertext = binary_to_decimal(plain_bin, 8);
    printf("output (y): ");
    print_binary(ciphertext, 8);
    
    return ciphertext;
}

uint8_t decrypt(uint8_t ciphertext, uint16_t key) {
    uint8_t cipher_bin[8], temp[8], left[4], right[4];
    uint8_t k1[8], k2[8];
    uint8_t f_out[4], xor_out[4];
    
    printf("Decryption Process\n");
    generate_round_keys(key, k1, k2);
    
    printf("ciphertext: ");
    print_binary(ciphertext, 8);
    
    decimal_to_binary(ciphertext, cipher_bin, 8);
    apply_permutation(cipher_bin, temp, IP, 8);
    printf("IP(C): ");
    print_binary(binary_to_decimal(temp, 8), 8);
    
    for (int i = 0; i < 4; i++) {
        left[i] = temp[i];
        right[i] = temp[i + 4];
    }
    
    printf("L1: ");
    print_binary(binary_to_decimal(left, 4), 4);
    printf("R1: ");
    print_binary(binary_to_decimal(right, 4), 4);
    
    function_f(right, k2, f_out);
    xor_arrays(left, f_out, xor_out, 4);
    
    printf("L2: ");
    print_binary(binary_to_decimal(right, 4), 4);
    printf("R2: ");
    print_binary(binary_to_decimal(xor_out, 4), 4);
    
    for (int i = 0; i < 4; i++) {
        left[i] = right[i];
        right[i] = xor_out[i];
    }
    
    function_f(right, k1, f_out);
    xor_arrays(left, f_out, xor_out, 4);
    
    printf("L3: ");
    print_binary(binary_to_decimal(xor_out, 4), 4);
    printf("R3: ");
    print_binary(binary_to_decimal(right, 4), 4);
    
    for (int i = 0; i < 4; i++) {
        temp[i] = xor_out[i];
        temp[i + 4] = right[i];
    }
    
    printf("output (y): ");
    print_binary(binary_to_decimal(temp, 8), 8);
    
    apply_permutation(temp, cipher_bin, IP_INV, 8);
    
    uint8_t plaintext = binary_to_decimal(cipher_bin, 8);
    printf("output (y): ");
    print_binary(plaintext, 8);
    
    return plaintext;
}


int main() {
    uint8_t plaintext, ciphertext, decrypted;
    uint16_t key;
    
    printf("Enter 8-bit plaintext (0-255): ");
    scanf("%hhu", &plaintext);
    
    printf("Enter 10-bit key (0-1023): ");
    scanf("%hu", &key);
    
    printf("\n----------------------------- Printing Encryption summary -----------------------------\n");
    ciphertext = encrypt(plaintext, key);
    
    printf("\n----------------------------- Printing Decryption summary -----------------------------\n");
    decrypted = decrypt(ciphertext, key);
    
    printf("\n----------------------------- Printing Overall Summary -----------------------------\n");
    printf("\nPlaintext: %d\n", plaintext);
    printf("\nCiphertext: %d\n", ciphertext);
    printf("\nDecrypted: %d\n", decrypted);
    
    if (plaintext == decrypted) {
        printf("Verification successful: Plaintext matches decrypted text.\n");
    } else {
        printf("Verification failed: Plaintext does not match decrypted text.\n");
    }
    
    return 0;
}