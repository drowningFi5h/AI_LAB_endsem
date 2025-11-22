#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <openssl/sha.h>

// Constants for elliptic curve
#define PRIME 1223
#define A 439
#define B 133

// AES-256 constants
#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 32
#define AES_ROUNDS 14
#define ROUND_KEY_SIZE (AES_BLOCK_SIZE * (AES_ROUNDS + 1))

// Elliptic Curve Point Structure
typedef struct
{
    int x;
    int y;
    int is_infinity;
} Point;

static const uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa9, 0x6d, 0x8b, 0xd4, 0x90, 0xdd, 0xe0, 0x13, 0x35, 0x44, 0xc2, 0x41, 0xa7, 0x98, 0x14,
    0xf5, 0x73, 0x86, 0x70, 0x9b, 0x10, 0x56, 0x81, 0x4b, 0xf8, 0xa6, 0x1c, 0x03, 0x0d, 0x4e, 0x0e,
    0x24, 0x1f, 0xdd, 0xb7, 0x47, 0xe6, 0xe8, 0x99, 0x82, 0x17, 0xb5, 0x1a, 0x2d, 0xe4, 0xa1, 0xc6,
    0x12, 0x14, 0x19, 0x1c, 0x1f, 0x22, 0x25, 0x28, 0x2b, 0x2e, 0x31, 0x34, 0x37, 0x3a, 0x3d, 0x40,
    0x43, 0x46, 0x49, 0x4c, 0x4f, 0x52, 0x55, 0x58, 0x5b, 0x5e, 0x61, 0x64, 0x67, 0x6a, 0x6d, 0x70,
    0x73, 0x76, 0x79, 0x7c, 0x7f, 0x82, 0x85, 0x88, 0x8b, 0x8e, 0x91, 0x94, 0x97, 0x9a, 0x9d, 0xa0,
    0xa3, 0xa6, 0xa9, 0xac, 0xaf, 0xb2, 0xb5, 0xb8, 0xbb, 0xbe, 0xc1, 0xc4, 0xc7, 0xca, 0xcd, 0xd0,
    0xd3, 0xd6, 0xd9, 0xdc, 0xdf, 0xe2, 0xe5, 0xe8, 0xeb, 0xee, 0xf1, 0xf4, 0xf7, 0xfa, 0xfd};
static const uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};
static const uint8_t RCON[15] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A
};

int mod(int a, int m)
{
    return ((a % m) + m) % m;
}

// Function to compute modular multiplicative inverse
int mod_inverse(int a, int m)
{
    a = mod(a, m);
    for (int x = 1; x < m; x++)
    {
        if (mod(a * x, m) == 1)
        {
            return x;
        }
    }
    return -1; // No modular inverse exists
}

// Elliptic curve operations
int is_point_on_curve(Point P) {
    if (P.is_infinity) return 1;
    
    // Verify y² = x³ + 439x + 133 (mod 1223)
    int left = mod(P.y * P.y, PRIME);
    int right = mod(P.x * P.x * P.x + A * P.x + B, PRIME);
    
    return left == right;
}
// Point addition on elliptic curve
Point point_add(Point P, Point Q) {
    Point R;
    
    // Handle point at infinity cases
    if (P.is_infinity) return Q;
    if (Q.is_infinity) return P;
    
    // Compute slope
    int slope;
    if (P.x == Q.x && P.y == Q.y) {
        // Point doubling case
        if (P.y == 0) {
            R.is_infinity = 1;
            return R;
        }
        int numerator = mod(3 * P.x * P.x + A, PRIME);
        int denominator = mod(2 * P.y, PRIME);
        int inv_denom = mod_inverse(denominator, PRIME);
        slope = mod(numerator * inv_denom, PRIME);
    } else {
        // Point addition case
        if (P.x == Q.x) {
            R.is_infinity = 1;
            return R;
        }
        int numerator = mod(Q.y - P.y, PRIME);
        int denominator = mod(Q.x - P.x, PRIME);
        int inv_denom = mod_inverse(denominator, PRIME);
        slope = mod(numerator * inv_denom, PRIME);
    }
    
    // Compute new point
    R.x = mod(slope * slope - P.x - Q.x, PRIME);
    R.y = mod(slope * (P.x - R.x) - P.y, PRIME);
    R.is_infinity = 0;
    
    return R;
}
// Scalar multiplication of a point
Point point_multiply(Point P, int scalar) {
    Point R;
    R.is_infinity = 1;
    
    Point Q = P;
    
    while (scalar > 0) {
        if (scalar & 1) {
            if (R.is_infinity) {
                R = Q;
            } else {
                R = point_add(R, Q);
            }
        }
        Q = point_add(Q, Q);
        scalar >>= 1;
    }
    
    return R;
}

// Find a random point on the curve
Point find_random_point() {
    Point P;
    srand(time(NULL));
    
    do {
        P.x = rand() % PRIME;
        P.y = rand() % PRIME;
        P.is_infinity = 0;
    } while (!is_point_on_curve(P));
    
    return P;
}

void print_point(Point P, const char* label) {
    if (P.is_infinity) {
        printf("%s: Point at Infinity\n", label);
    } else {
        printf("%s: (%d, %d)\n", label, P.x, P.y);
    }
}

// SHA-512 Key Derivation
void compute_sha512_key(Point shared_secret, unsigned char *key)
{
    char point_data[100];
    sprintf(point_data, "%d%d", shared_secret.x, shared_secret.y);
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512((unsigned char *)point_data, strlen(point_data), hash);
    memcpy(key, hash, SHA512_DIGEST_LENGTH);
}
void print_key(unsigned char *key, const char *label) { /* implementation remains unchanged */ }

// Diffie-Hellman Key Exchange
Point diffie_hellman_key_exchange(Point base_point, int private_key)
{
    return point_multiply(base_point, private_key);
}

// AES Helper Functions (subBytes, shiftRows, mixColumns, etc.)
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }
        
        // Check if a would overflow
        uint8_t hi_bit_set = (a & 0x80);
        
        // Shift a left
        a <<= 1;
        
        // If high bit was set before shifting, XOR with irreducible polynomial
        if (hi_bit_set) {
            a ^= 0x1B;  // x^8 + x^4 + x^3 + x + 1 polynomial
        }
        
        // Shift b right
        b >>= 1;
    }
    
    return p;
}
void subBytes(uint8_t *state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] = SBOX[state[i]];
    }
}
// Inverse SubBytes transformation
void invSubBytes(uint8_t *state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] = INV_SBOX[state[i]];
    }
}
void shiftRows(uint8_t *state) {
    // Row 0 (first row) remains unchanged
    
    // Row 1: Rotate left by 1
    uint8_t temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2: Rotate left by 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: Rotate left by 3
    temp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = temp;
}

void invShiftRows(uint8_t *state) {
    // Row 0 (first row) remains unchanged
    
    // Row 1: Rotate right by 1
    uint8_t temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;
    
    // Row 2: Rotate right by 2
    temp = state[10];
    state[10] = state[2];
    state[2] = temp;
    temp = state[14];
    state[14] = state[6];
    state[6] = temp;
    
    // Row 3: Rotate right by 3
    temp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = temp;
}
void mixColumns(uint8_t *state) {
    for (int col = 0; col < 4; col++) {
        uint8_t a[4];
        uint8_t b[4];
        
        // Copy column
        for (int i = 0; i < 4; i++) {
            a[i] = state[col * 4 + i];
            b[i] = (a[i] << 1) ^ ((a[i] & 0x80) ? 0x1B : 0x00);
        }
        
        // Matrix multiplication in Galois Field
        state[col * 4 + 0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
        state[col * 4 + 1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
        state[col * 4 + 2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
        state[col * 4 + 3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
    }
}
void invMixColumns(uint8_t *state) {
    for (int col = 0; col < 4; col++) {
        uint8_t a[4];
        
        // Copy column
        for (int i = 0; i < 4; i++) {
            a[i] = state[col * 4 + i];
        }
        
        // Inverse matrix multiplication in Galois Field
        state[col * 4 + 0] = gmul(a[0], 0x0E) ^ gmul(a[3], 0x0B) ^ gmul(a[2], 0x0D) ^ gmul(a[1], 0x09);
        state[col * 4 + 1] = gmul(a[1], 0x0E) ^ gmul(a[0], 0x0B) ^ gmul(a[3], 0x0D) ^ gmul(a[2], 0x09);
        state[col * 4 + 2] = gmul(a[2], 0x0E) ^ gmul(a[1], 0x0B) ^ gmul(a[0], 0x0D) ^ gmul(a[3], 0x09);
        state[col * 4 + 3] = gmul(a[3], 0x0E) ^ gmul(a[2], 0x0B) ^ gmul(a[1], 0x0D) ^ gmul(a[0], 0x09);
    }
}
void keyExpansion(uint8_t *key, uint8_t *roundKeys) {
    int i, j;
    uint8_t temp[4];

    // Copy the original key as the first round key
    memcpy(roundKeys, key, AES_KEY_SIZE);

    for (i = AES_KEY_SIZE / 4; i < (ROUND_KEY_SIZE / 4); i++) {
        // Copy the last word
        memcpy(temp, &roundKeys[(i - 1) * 4], 4);

        if (i % (AES_KEY_SIZE / 4) == 0) {
            // Rotate word
            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            // Apply S-box
            for (j = 0; j < 4; j++) {
                temp[j] = SBOX[temp[j]];
            }

            // XOR with round constant
            temp[0] ^= RCON[i / (AES_KEY_SIZE / 4) - 1];
        } else if (AES_KEY_SIZE > 24 && i % (AES_KEY_SIZE / 4) == 4) {
            // Apply S-box for AES-256 every 4th word
            for (j = 0; j < 4; j++) {
                temp[j] = SBOX[temp[j]];
            }
        }

        // XOR with the word from 4 positions earlier
        for (j = 0; j < 4; j++) {
            roundKeys[i * 4 + j] = roundKeys[(i - AES_KEY_SIZE / 4) * 4 + j] ^ temp[j];
        }
    }
}

void aesEncrypt(uint8_t *state, uint8_t *key) {
    uint8_t roundKeys[AES_KEY_SIZE * (AES_ROUNDS + 1)];
    keyExpansion(key, roundKeys);

    // Initial round
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] ^= roundKeys[i];
    }

    // Main rounds
    for (int round = 1; round <= AES_ROUNDS; round++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);

        // Add round key
        for (int i = 0; i < AES_BLOCK_SIZE; i++) {
            state[i] ^= roundKeys[round * AES_BLOCK_SIZE + i];
        }
    }
}

// AES-256 Decryption
void aesDecrypt(uint8_t *state, uint8_t *key) {
    uint8_t roundKeys[AES_KEY_SIZE * (AES_ROUNDS + 1)];
    keyExpansion(key, roundKeys);

    // Inverse rounds
    for (int round = AES_ROUNDS; round >= 1; round--) {
        // Subtract round key
        for (int i = 0; i < AES_BLOCK_SIZE; i++) {
            state[i] ^= roundKeys[round * AES_BLOCK_SIZE + i];
        }

        invMixColumns(state);
        invShiftRows(state);
        invSubBytes(state);
    }

    // Final round
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] ^= roundKeys[i];
    }
}
void triple_aes_encrypt(uint8_t *plaintext, uint8_t *key1, uint8_t *key2, uint8_t *ciphertext) {
    uint8_t temp1[AES_BLOCK_SIZE], temp2[AES_BLOCK_SIZE];

    // Step 1: Encrypt with K1
    memcpy(temp1, plaintext, AES_BLOCK_SIZE);
    aesEncrypt(temp1, key1);

    // Step 2: Decrypt with K2
    memcpy(temp2, temp1, AES_BLOCK_SIZE);
    aesDecrypt(temp2, key2);

    // Step 3: Encrypt with K1 again
    memcpy(ciphertext, temp2, AES_BLOCK_SIZE);
    aesEncrypt(ciphertext, key1);
}

// Triple-AES'-512 Decryption
void triple_aes_decrypt(uint8_t *ciphertext, uint8_t *key1, uint8_t *key2, uint8_t *plaintext) {
    uint8_t temp1[AES_BLOCK_SIZE], temp2[AES_BLOCK_SIZE];

    // Step 1: Decrypt with K1
    memcpy(temp1, ciphertext, AES_BLOCK_SIZE);
    aesDecrypt(temp1, key1);

    // Step 2: Encrypt with K2
    memcpy(temp2, temp1, AES_BLOCK_SIZE);
    aesEncrypt(temp2, key2);

    // Step 3: Decrypt with K1 again
    memcpy(plaintext, temp2, AES_BLOCK_SIZE);
    aesDecrypt(plaintext, key1);
}

// Compute MAC using SHA-512-based algorithm
void compute_mac(uint8_t *key, uint8_t *message, size_t message_len, uint8_t *mac) {
    unsigned char temp1[SHA512_DIGEST_LENGTH], temp2[SHA512_DIGEST_LENGTH];

    // KA ⊕ 125
    uint8_t key_xor_125[AES_KEY_SIZE];
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        key_xor_125[i] = key[i] ^ 125;
    }
    SHA512(key_xor_125, AES_KEY_SIZE, temp1);

    // (KA ⊕ 215) || MA
    uint8_t key_xor_215[AES_KEY_SIZE + message_len];
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        key_xor_215[i] = key[i] ^ 215;
    }
    memcpy(key_xor_215 + AES_KEY_SIZE, message, message_len);
    SHA512(key_xor_215, AES_KEY_SIZE + message_len, temp2);

    // Final MAC: SHA-512(temp1 || temp2)
    uint8_t concat[SHA512_DIGEST_LENGTH * 2];
    memcpy(concat, temp1, SHA512_DIGEST_LENGTH);
    memcpy(concat + SHA512_DIGEST_LENGTH, temp2, SHA512_DIGEST_LENGTH);
    SHA512(concat, SHA512_DIGEST_LENGTH * 2, mac);
}

int main() {
    // Elliptic curve setup
    Point alpha = find_random_point();
    print_point(alpha, "Random Point α");

    int nA, nB;
    printf("\nEnter Alice's private key (1-533): ");
    scanf("%d", &nA);
    printf("Enter Bob's private key (1-533): ");
    scanf("%d", &nB);

    // Diffie-Hellman key exchange
    Point AlicePublic = point_multiply(alpha, nA);
    Point BobPublic = point_multiply(alpha, nB);
    Point SharedSecret = point_multiply(AlicePublic, nB);

    // Derive keys
    unsigned char Ka[AES_KEY_SIZE], Kb[AES_KEY_SIZE];
    compute_sha512_key(SharedSecret, Ka);
    memcpy(Kb, Ka, AES_KEY_SIZE);

    // Input plaintext
    uint8_t plaintext[AES_BLOCK_SIZE];
    printf("\nEnter a 16-byte plaintext (in hexadecimal, space-separated): ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        scanf("%hhx", &plaintext[i]);
    }

    // Encrypt message
    uint8_t ciphertext[AES_BLOCK_SIZE];
    triple_aes_encrypt(plaintext, Ka, Ka + AES_BLOCK_SIZE / 2, ciphertext);

    // Compute MAC
    uint8_t macA[SHA512_DIGEST_LENGTH];
    compute_mac(Ka, plaintext, AES_BLOCK_SIZE, macA);

    // Decrypt message on Bob's side
    uint8_t decrypted[AES_BLOCK_SIZE];
    triple_aes_decrypt(ciphertext, Kb, Kb + AES_BLOCK_SIZE / 2, decrypted);

    // Compute MAC on Bob's side
    uint8_t macB[SHA512_DIGEST_LENGTH];
    compute_mac(Kb, decrypted, AES_BLOCK_SIZE, macB);

    // Output results
    printf("\nCiphertext: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\nMAC (Alice): ");
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        printf("%02x ", macA[i]);
    }
    printf("\nDecrypted message: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x ", decrypted[i]);
    }
    printf("\nMAC (Bob): ");
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        printf("%02x ", macB[i]);
    }

    // Verification
    if (memcmp(plaintext, decrypted, AES_BLOCK_SIZE) == 0 &&
        memcmp(macA, macB, SHA512_DIGEST_LENGTH) == 0) {
        printf("\n\nVerification successful: M_A = M_B, MAC_A = MAC_B.\n");
    } else {
        printf("\n\nVerification failed.\n");
    }

    return 0;
}