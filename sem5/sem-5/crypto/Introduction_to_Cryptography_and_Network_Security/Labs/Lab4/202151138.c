/*
Name: Sanidhya Kumar
Roll No: 202151138
Lab Assignment 04
Date: 23rd April 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

//------------------------ DEFINING THINGS USED FREQUENTLY ------------------------

// Defining things that would be used throughout the code
#define PRIME 1021 // prime number given = 1021
// Elliptical curve given: y^2 = x^3 + 449x + 233 over Zp
#define A_COEFFICIENT 449 
#define B_COEFFICIENT 233

// Defining macros that would be used in SHA

// Rotate a word right by a specified number of bits
#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))
// CH function: bitwise XOR of (x AND y) with ((NOT x) AND z)
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
// MAJ function: bitwise XOR of ((x AND y) with ((x AND z) XOR (y AND z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
// EP0 function: specific bitwise rotations applied to a word x
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
// EP1 function: specific bitwise rotations applied to a word x
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
// SIG0 function: specific bitwise rotations applied to a word x
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
// SIG1 function: specific bitwise rotations applied to a word x
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

// Defining digest and block size of SHA256
#define SHA256_BLOCK_SIZE 64 // 64 bytes = 512 bits
#define SHA256_DIGEST_SIZE 32 //32 bytes = 256 bits

//------------------------ FUNCTIONS RELATED TO ELLIPTICAL CURVE ------------------------

// Point structure representing a point on the elliptic curve
typedef struct {
    int x;
    int y;
} Point;

// Extended Euclidean Algorithm to find the modular inverse
int modInverse(int a, int m) {
    int m0 = m, t, q; // m0 is the original m value
    int x0 = 0, x1 = 1;
    
    if (m == 1) return 0; // If m is 1, then the modular inverse is 0

    // Apply extended Euclid Algorithm
    while (a > 1) {
        q = a / m; // q is quotient
        t = m;
        m = a % m, a = t; // m is remainder now, process same as euclid's algo
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0; // Make x1 positive

    return x1;
}

// Addition operation on the elliptic curve
Point ec_add(Point P, Point Q) {
    Point result; // Resultant point
    int slope; // Slope of the line passing through points P and Q

    //Lets consider each possible case which can arise while adding two points on the elliptic curve

    // Case 1: P is the point at infinity (0,1)
    if (P.x == 0 && P.y == 1) {
        return Q;
    }
    // Case 2: Q is the point at infinity (0,1)
    if (Q.x == 0 && Q.y == 1) {
        return P;
    }

    // Case 3: P and Q, both are point at infinity (0,1)
    if (P.x == 0 && P.y == 1 && Q.x == 0 && Q.y == 1) {
        return P;
    }

    // Case 4: Q is mirror image of P in x-axis
    if (P.x == Q.x && P.y == (-Q.y + PRIME) % PRIME) {
        result.x = 0;
        result.y = 1;
        return result;
    }

    // Case 5: P = Q, basically tangent at P
    if (P.x == Q.x && P.y == Q.y) {
        slope = ((((3 * P.x * P.x) % PRIME + A_COEFFICIENT) % PRIME) * (modInverse(2 * P.y, PRIME) % PRIME)) % PRIME;
    }

    // Case: P != Q, both are seperate points on the elliptical curve
    else {
        slope = (((Q.y - P.y + PRIME) % PRIME) * modInverse((Q.x - P.x + PRIME) % PRIME, PRIME)) % PRIME;
    }

    // Calculate the x and y coordinates of the resultant point using slope which we obtained fromcabove cases
    result.x = ((slope * slope) % PRIME + (PRIME - P.x) + (PRIME - Q.x)) % PRIME;
    result.y = ((slope * (P.x - result.x + PRIME)) % PRIME + (PRIME - P.y)) % PRIME;
    // IMP NOTE - I incorporated making R to -R in the above formulae itself, so that I don't have to handle it separately

    // Handle negative coordinates if they arise
    if (result.x < 0) {
        result.x += PRIME;
    }
    if (result.y < 0) {
        result.y += PRIME;
    }

    return result; // Return answer of P+Q
}

// Scalar multiplication on the elliptic curve
Point ec_scalar_mul(int scalar, Point P) {
    Point result = {0, 1}; // Point at infinity
    for (int i = 0; i < scalar; i++) {
        // Multiplying P with a scalar is basically adding P to the result scalar times
        result = ec_add(result, P);
    }
    return result;
}

// Find all valid points (x, y) on the elliptical curve
void find_alphas(Point *alphas, int *num_alphas) {
    *num_alphas = 0; // Initially number of valid points is 0
     for(int i = 0; i < PRIME; i++) {
        // Calulating LHS of the equation
        int left = (i * i) % PRIME;
        for(int j = 0; j < PRIME; j++) {
            // Calculating RHS of the equation
            int right = (j*j*j + A_COEFFICIENT * j + B_COEFFICIENT) % PRIME; 
            // if LHS = RHS, then the point is valid and hence add it to the array of valid points
            if(right == left) {
                alphas[*num_alphas].x = j;
                alphas[*num_alphas].y = i;
                (*num_alphas)++; // Increment the number of valid points
            }
        }
    }
}

// Choose a random alpha from the list of valid points
Point choose_random_alpha(Point *alphas, int num_alphas) {
    int random_index = rand() % num_alphas; // generate a random index between [0, num_valid-points]
    return alphas[random_index]; // get the alpha present at the random index
}

//------------------------ FUNCTIONS RELATED TO SHA ------------------------

// Structure for SHA-256 context
typedef struct {
    uint8_t data[64]; // Data buffer for input message blocks
    uint32_t datalen; // Length of the data in the buffer
    unsigned long long bitlen; // Total length of the message in bits
    uint32_t state[8]; // Intermediate hash state
} SHA256_CTX;

// Constants for SHA-256 algorithm
uint32_t sha256_k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Performing SHA-256 transformation on a data block
void sha256_transform(SHA256_CTX *ctx, const uint8_t data[]) {
    uint32_t m[64], a, b, c, d, e, f, g, h, i, j, t1, t2;

    // Prepare message schedule
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    // Initialize hash values for this chunk
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    // Main loop
    for (i = 0; i < 64; ++i) {
        // Calculate temporary values
        t1 = h + EP1(e) + CH(e, f, g) + sha256_k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        // Update working variables
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // Update hash values for the next chunk
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}


// Initialize the SHA-256 context with default values
void sha256_init(SHA256_CTX *ctx) {
    // Reset data length and bit length
    ctx->datalen = 0;
    ctx->bitlen = 0;
    
    // Initialize hash state with initial values
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}


// Update the SHA-256 context with input data
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len) {
    // Iterate over input data
    for (size_t i = 0; i < len; ++i) {
        // Append data byte to the context
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        // Check if the data block is full (64 bytes)
        if (ctx->datalen == 64) {
            // Perform SHA-256 transformation on the data block
            sha256_transform(ctx, ctx->data);
            // Update the total bit length processed
            ctx->bitlen += 512;
            // Reset data length for the next block
            ctx->datalen = 0;
        }
    }
}


// Finalize the SHA-256 hash computation and output the hash value
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]) {
    uint32_t i = ctx->datalen;

    // Append a single bit '1' to the data
    ctx->data[i++] = 0x80;

    // Pad with zeros until the data length reaches 56 bytes (448 bits)
    if (ctx->datalen < 56) {
        while (i < 56)
            ctx->data[i++] = 0x00;
    } else {
        while (i < 64)
            ctx->data[i++] = 0x00;
        // Perform SHA-256 transformation on the padded data block
        sha256_transform(ctx, ctx->data);
        // Clear the remaining data block
        memset(ctx->data, 0, 56);
    }

    // Append the bit length of the original message as a 64-bit integer
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;

    // Perform a final SHA-256 transformation
    sha256_transform(ctx, ctx->data);

    // Extract the hash values from the final state
    for (i = 0; i < 4; ++i) {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

// Compute the SHA-256 hash of the input data
void sha256(const uint8_t *data, size_t len, uint8_t *hash) {
    SHA256_CTX ctx;
    // Initialize the SHA-256 context
    sha256_init(&ctx);
    // Update the context with the input data
    sha256_update(&ctx, data, len);
    // Finalize the SHA-256 hash computation and obtain the hash value
    sha256_final(&ctx, hash);
}

// Print the hash value in hexadecimal format
void print_key(const uint8_t key[], int key_size) {
    // Iterate over each byte in the key array
    for (int i = 0; i < key_size; i++) {
        // Print each byte in hexadecimal format with two characters
        printf("%02x ", key[i]);
    }
    printf("\n");
}

//------------------------ FUNCTIONS RELATED TO AES PRIME ------------------------

// Function to convert 1D array to 2D matrix
void Array2Matrix(uint8_t Array[16], uint8_t Matrix[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Matrix[i][j] = Array[i + 4*j];
        }
    }
}

// Function to convert 2D matrix to 1D array
void Matrix2Array(uint8_t Matrix[4][4], uint8_t Array[16]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Array[i + 4*j] = Matrix[i][j];
        }
    }
}

// Function to perform AES multiplication under modulo x^8 + x^4 + x^3 + x + 1
uint8_t aes_multiply(uint8_t x1, uint8_t x2) {
    uint8_t ans = 0; // answer variable
    uint8_t carry = 0; // carry variable
    for (int i = 0; i < 8; i++) {
        if (x2 & 1) 
            ans ^= x1;
        
        carry = x1 & 0x80; 
        x1 <<= 1;
        if (carry) 
            x1 ^= 0x1B;
        
        x2 >>= 1;
    }
    return ans;
}

//------------------------ PRE-DEFINED VARIABLES FOR AES PRIME ------------------------

// S-Box for AES Encryption
uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; 

// S-Box for AES_prime
uint8_t sbox_new[256] = {0};

// Rcon for AES
const uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

//------------------------ FUNCTIONS FOR ENCRYPTION AES PRIME ------------------------

// Subbyte as defined in the question
uint8_t Sub(uint8_t x) {
    uint8_t temp = aes_multiply(0xDD, x);
    temp = temp ^ 125;
    return sbox[temp];
}

// Function for obtaining value of Subbyte
void SubBytes(uint8_t input[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = Sub(input[i][j]);
        }
    }
}

// Function for shifting rows
void ShiftRows(uint8_t input[4][4]) {
    // Create a copy for shifting
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = input[i][(j + i) % 4]; // % to take care of cyclic order
        }
    }

    // Transfer back things to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

// Function for mix coloumns as given in question
void MixColumns(uint8_t input[4][4]) {
    // Create a copy for mixing
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        temp[0][i] = aes_multiply(0x01, input[0][i]) ^ aes_multiply(0x04, input[1][i]) ^ aes_multiply(0x04, input[2][i]) ^ aes_multiply(0x05, input[3][i]);
        temp[1][i] = aes_multiply(0x05, input[0][i]) ^ aes_multiply(0x01, input[1][i]) ^ aes_multiply(0x04, input[2][i]) ^ aes_multiply(0x04, input[3][i]);
        temp[2][i] = aes_multiply(0x04, input[0][i]) ^ aes_multiply(0x05, input[1][i]) ^ aes_multiply(0x01, input[2][i]) ^ aes_multiply(0x04, input[3][i]);
        temp[3][i] = aes_multiply(0x04, input[0][i]) ^ aes_multiply(0x04, input[1][i]) ^ aes_multiply(0x05, input[2][i]) ^ aes_multiply(0x01, input[3][i]);
    }
    
    // Transfer back things to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

//------------------------ FUNCTIONS FOR DECRYPTION AES PRIME ------------------------

// Function to get inverse of Subbyte
uint8_t inv_Sub(uint8_t x) {
    for(int i = 0; i < 256; i++) {
        uint8_t temp = Sub(i);
        sbox_new[i] = temp; //using sbox_new table
        // sbox_new table has been created to store the new subbyte mappings
    }
    // performing inverse
    for(int i = 0; i < 256; i++) {
        if(sbox_new[i] == x) {
            return i;
        }
    }
    return 0;
}

// Function for obtaining value of Inverse Subbyte
void inv_SubBytes(uint8_t input[4][4]){
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            input[i][j] = inv_Sub(input[i][j]);
        }
    }
}

// Function for inverse of shifting rows
void inv_ShiftRows(uint8_t input[4][4]){
    // Create a copy for inverse shifting
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][(j + i) % 4] = input[i][j]; // % to take care of cyclic order
        }
    }

    // Transfer back things to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

// Function for inverse of mix coloumns as given in question
void inv_MixColumns(uint8_t input[4][4]){
    // Create a copy for inverse mixing columns
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        temp[0][i] = aes_multiply(0xa5, input[0][i]) ^ aes_multiply(0x07, input[1][i]) ^ aes_multiply(0x1A, input[2][i]) ^ aes_multiply(0x73, input[3][i]);
        temp[1][i] = aes_multiply(0x73, input[0][i]) ^ aes_multiply(0xa5, input[1][i]) ^ aes_multiply(0x07, input[2][i]) ^ aes_multiply(0x1a, input[3][i]);
        temp[2][i] = aes_multiply(0x1a, input[0][i]) ^ aes_multiply(0x73, input[1][i]) ^ aes_multiply(0xa5, input[2][i]) ^ aes_multiply(0x07, input[3][i]);
        temp[3][i] = aes_multiply(0x07, input[0][i]) ^ aes_multiply(0x1a, input[1][i]) ^ aes_multiply(0x73, input[2][i]) ^ aes_multiply(0xa5, input[3][i]);
    }

    // Transfer back things to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }

}

//------------------------ FUNCTIONS FOR KEY-SCHEDULING OF AES PRIME ------------------------

// Function for Subword as defined for AES
uint32_t SubWord(uint32_t word) {
    uint32_t result = 0;
    for (int i = 0; i < 4; ++i) {
        uint8_t temp = (word >> (i * 8)) & 0xFF; // Extracting the 8 bits from ith position
        temp = sbox[temp]; // Applying the S-Box
        result |= (uint32_t)temp << (i * 8); // note that |= is bitwise OR assignment
    }
    return result;
}

// Function for Rotword as defined for AES
uint32_t RotWord(uint32_t word) {
    uint32_t temp = word;
    temp = (temp << 8) | (temp >> 24); // Left circular shift by 1 byte
    return temp;
}

// Key Scheduling Algorithm as defined for AES
uint32_t* key_scheduling_algo(uint8_t *key) {
    // Array to store the words after all operations are done
    uint32_t* words = malloc(44 * sizeof(uint32_t)); 

    // First 4 words are same as the key
    for(int i = 0; i < 4; i++)  {
        uint32_t temp = 0;
        // Extracting the 4 bytes from the 32 bit key
        temp = temp | key[4 * i];  
        temp = temp << 8;
        temp = temp | key[(4 * i) + 1];
        temp = temp << 8;
        temp = temp | key[(4 * i) + 2]; 
        temp = temp << 8;
        temp = temp | key[(4 * i) + 3];
        words[i] = temp; 
    }
    
    // Generating the rest of the words using the first four words
    for(int i = 4; i < 44; i++) {
        uint32_t temp = words[i - 1]; // Taking the previous word
        // If i is a multiple of 4, then we need to perform extra operations
        if(i % 4 == 0) {
            uint32_t var = Rcon[i / 4] << 24; // Getting the Rcon value
            temp = SubWord(RotWord(temp)) ^ var; // Performing SubWord, RotWord and XOR operations
        }
        // Otherwise, we just need to perform a simple XOR operation
        words[i] = words[i - 4] ^ temp; // XOR operation
    }
    return words;
}

// Function to get round key from expanded key array and index
void get_round_key(uint32_t keys[], int i, uint8_t result[16]) {
    // Extracting the 4 bytes from the 32 bit key at given index
    result[0] = keys[i] >> 24;
    result[1] = keys[i] >> 16;
    result[2] = keys[i] >> 8;
    result[3] = keys[i] >> 0;
    result[4] = keys[i + 1] >> 24;
    result[5] = keys[i + 1] >> 16;
    result[6] = keys[i + 1] >> 8;
    result[7] = keys[i + 1] >> 0;
    result[8] = keys[i + 2] >> 24;
    result[9] = keys[i + 2] >> 16;
    result[10] = keys[i + 2] >> 8;
    result[11] = keys[i + 2] >> 0;
    result[12] = keys[i + 3] >> 24;
    result[13] = keys[i + 3] >> 16;
    result[14] = keys[i + 3] >> 8;
    result[15] = keys[i + 3]>> 0;
}

// Function to perform XOR operation with round key on input
void XOR_round_key(uint8_t input[4][4], uint32_t keys[], int start) {
    uint8_t temp[16] = {0}; // Temporary array to store the round key
    // Getting the round key
    get_round_key(keys, start, temp);
    // Performing XOR operation
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] ^= temp[4 * i + j];
        }
    }
}

//------------------------ AES PRIME ENCRYPTION FUNC ------------------------

// Function to perform AES encryption
void AESprime_encrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    // Initial XOR operation with round key
    XOR_round_key(input, keys, 0);

    // Performing 10 rounds of encryption
    for (int round = 1; round <= 10; round++) {
        SubBytes(input); // SubBytes operation
        ShiftRows(input); // ShiftRows operation
        MixColumns(input); // MixColumns operation
        XOR_round_key(input, keys, round * 4); // XOR operation with round key
    }

    // Final round of encryption
    SubBytes(input); // SubBytes operation
    ShiftRows(input); // ShiftRows operation
    XOR_round_key(input, keys,11); // XOR operation with round key
    // No mix columns operation in final round

    // Copying the output to the output array
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i][j] = input[i][j];
        } 
    }
}

//------------------------ AES PRIME DECRYPTION FUNC ------------------------

// Function to perform AES decryption
void AESprime_decrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    XOR_round_key(input, keys, 11); // Initial XOR operation with round key
    inv_ShiftRows(input); // Inverse ShiftRows operation
    inv_SubBytes(input); // Inverse SubBytes operation

    // Performing 10 rounds of decryption
    for (int round = 10; round >= 1; round--) {
        XOR_round_key(input, keys, round * 4); // XOR operation with round key
        inv_MixColumns(input); // Inverse MixColumns operation
        inv_ShiftRows(input); // Inverse ShiftRows operation
        inv_SubBytes(input); // Inverse SubBytes operation
    }

    // Final round of decryption
    XOR_round_key(input, keys, 0);

    // Copying the output to the output array
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i][j] = input[i][j];
        }
    }
}

// Print the contents of a 4x4 matrix in hexadecimal format
void print_matrix(uint8_t matrix[4][4], int row, int col) {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            // Print each element of the matrix in hexadecimal format with two characters
            printf("%02x ", matrix[j][i]);
        }
    }
}

//------------------------ TRIPLE ENCRYPTION IMPLEMENTATION USING AES PRIME ------------------------

// Performing triple AES prime encryption on a 128-bit message
// CA = TEncAES′-128(MA, KA) = EncAES′−128(DecAES′−128(EncAES′−128(MA, K1), K2), K1)

void Triple_AESprime_encrypt(uint8_t MA[16], uint8_t KA[SHA256_DIGEST_SIZE], uint8_t CA[16]){
    // Separate KA into K1 and K2
    uint8_t K1[(SHA256_DIGEST_SIZE / 2)];
    uint8_t K2[(SHA256_DIGEST_SIZE / 2)];
    memcpy(K1, KA, SHA256_DIGEST_SIZE / 2);
    memcpy(K2, KA + SHA256_DIGEST_SIZE / 2, SHA256_DIGEST_SIZE / 2);

    // Initialize matrices for input and output
    uint8_t enc_input[4][4] = {0}; // 4x4 matrix for plaintext
    uint8_t enc_output1[4][4] = {0}; // Intermediate ciphertext 1
    uint8_t enc_output2[4][4] = {0}; // Intermediate ciphertext 2
    uint8_t enc_output3[4][4] = {0}; // Final ciphertext
    
    // Generate round keys using K1 and K2 as master key
    uint32_t* enc_key_K1 = key_scheduling_algo(K1);
    uint32_t* enc_key_K2 = key_scheduling_algo(K2);
   
    // Convert the input message from array to 4x4 matrix
    Array2Matrix(MA, enc_input);
    
    // Perform AES prime encryption, decryption, and encryption sequentially
    AESprime_encrypt(enc_input, enc_output1, enc_key_K1); // First encryption with K1
    AESprime_decrypt(enc_output1, enc_output2, enc_key_K2); // Decryption with K2
    AESprime_encrypt(enc_output2, enc_output3, enc_key_K1); // Second encryption with K1

    // Convert the final ciphertext from 4x4 matrix to array
    Matrix2Array(enc_output3, CA);
}

//------------------------ TRIPLE DECRYPTION IMPLEMENTATION USING AES PRIME ------------------------

// Perform triple AES prime decryption on a 128-bit ciphertext
// MB = TDecAES′-128(CA, KB) = DecAES′−128(EncAES′−128(DecAES′−128(CA, K1), K2), K1)

void Triple_AESprime_decrypt(uint8_t CB[16], uint8_t KB[SHA256_DIGEST_SIZE], uint8_t MB[16]){
    // Separate KB into K1 and K2
    uint8_t K1[(SHA256_DIGEST_SIZE / 2)];
    uint8_t K2[(SHA256_DIGEST_SIZE / 2)];
    memcpy(K1, KB, SHA256_DIGEST_SIZE / 2);
    memcpy(K2, KB + SHA256_DIGEST_SIZE / 2, SHA256_DIGEST_SIZE / 2);

    // Initialize matrices for input and output
    uint8_t dec_input[4][4] = {0}; // 4x4 matrix for ciphertext
    uint8_t dec_output1[4][4] = {0}; // Intermediate plaintext
    uint8_t dec_output2[4][4] = {0}; // Intermediate plaintext
    uint8_t dec_output3[4][4] = {0}; // Final plaintext

    // Generate round keys using K1 and K2 as master key
    uint32_t* dec_key_K1 = key_scheduling_algo(K1);
    uint32_t* dec_key_K2 = key_scheduling_algo(K2);

    // Convert the input ciphertext from array to 4x4 matrix
    Array2Matrix(CB, dec_input);

    // Perform AES prime decryption, encryption, and decryption sequentially
    AESprime_decrypt(dec_input, dec_output1, dec_key_K1); // First decryption with K1
    AESprime_encrypt(dec_output1, dec_output2, dec_key_K2); // Encryption with K2
    AESprime_decrypt(dec_output2, dec_output3, dec_key_K1); // Second decryption with K1

    // Convert the final plaintext from 4x4 matrix to array
    Matrix2Array(dec_output3, MB);
}

//------------------------ MAC GENERATION IMPLEMENTATION ------------------------

// Generate MACA using SHA-256 hash function
// MAC = SHA-256((K ⊕ 125) || SHA-256((K ⊕ 215) || M))

void generate_mac(uint8_t K[SHA256_DIGEST_SIZE], uint8_t M[16], uint8_t MAC[2 * SHA256_DIGEST_SIZE]) {

    // Formula given is converted to following form for computation

    // MAC = SHA-256((K ⊕ 125) || SHA-256((K ⊕ 215) || M))    
    // MAC = SHA-256((K ⊕ constant1) || SHA-256((K ⊕ constant2) || M))
    // MAC = SHA-256(xor_result1 || SHA-256(xor_result2 || M))
    // MAC = SHA-256(xor_result1 || SHA-256(p2))
    // MAC = SHA-256(p1)
    // Convert constants to bytes (decimal values)

    // Constants for XOR operations
    uint8_t constant1 = 125;
    uint8_t constant2 = 215;

    // Performing XOR operation with constant1 and constant2
    uint8_t xor_result1[SHA256_DIGEST_SIZE];
    uint8_t xor_result2[SHA256_DIGEST_SIZE];

    // Copy the first 31 bytes of K to xor_result1 and xor_result2
    for (int i = 0; i < SHA256_DIGEST_SIZE - 1; i++) {
        xor_result1[i] = K[i];
        xor_result2[i] = K[i];
    }

    // Perform XOR operation with the last byte of K and constant1 and constant2 respectively
    // This is because only 1 byte is needed to store any of the two constants
    xor_result1[SHA256_DIGEST_SIZE - 1] = K[SHA256_DIGEST_SIZE - 1] ^ constant1;
    xor_result2[SHA256_DIGEST_SIZE - 1] = K[SHA256_DIGEST_SIZE - 1] ^ constant2;
    
    // Performing p2 = xor_result2 || M
    uint8_t p2[SHA256_DIGEST_SIZE + 16];
    memcpy(p2, xor_result2, SHA256_DIGEST_SIZE);
    memcpy(p2 + SHA256_DIGEST_SIZE, M, 16);

    // Performing SHA-256 hash for p2, p2_SHA = SHA-256(p2)
    uint8_t p2_SHA[SHA256_DIGEST_SIZE];
    sha256(p2, sizeof(p2), p2_SHA);

    // Concatenate p1 = xor_result1 || p2_SHA
    uint8_t p1[SHA256_DIGEST_SIZE + SHA256_DIGEST_SIZE];
    memcpy(p1, xor_result1, SHA256_DIGEST_SIZE);
    memcpy(p1 + SHA256_DIGEST_SIZE, p2_SHA, SHA256_DIGEST_SIZE);

    // Performing SHA-256 hash for p1, MAC = SHA-256(p1)
    sha256(p1, sizeof(p1), MAC);
    // MAC is the final output
}

//------------------------ VALIDATING OUTPUT OBTAINED ------------------------

// Validate the output of the cryptographic operations
void validate_output(const uint8_t *KA, const uint8_t *KB, const uint8_t *MA, const uint8_t *MB, const uint8_t *MACA, const uint8_t *MACB) {
    // Compare KA and KB
    int keys_equal = memcmp(KA, KB, SHA256_DIGEST_SIZE) == 0;
    if (!keys_equal) {
        printf("KA and KB are not equal.\n");
    } 

    // Compare MACA and MACB
    int mac_equal = memcmp(MACA, MACB, SHA256_DIGEST_SIZE) == 0;
    if (!mac_equal) {
        printf("MACA and MACB are not equal.\n");
    }

    // Compare MA and MB
    int message_equal = memcmp(MA, MB, 16) == 0;
    if (!message_equal) {
        printf("MA and MB are not equal.\n");
    }

    // Final validation result
    if (keys_equal && mac_equal && message_equal) {
        printf("\nValidation: PASSED! KA = KB, MA = MB, and MACA = MACB.");
    } else {
        printf("\nValidation: FAILED");
    }
}

//------------------------ MAIN FUNCTION ------------------------

int main() {
    printf("-----------------------------------------------------------------\n\n");
    printf("Welcome to the Cryptography Lab Assignment 04\nMade by Sanidhya Kumar (202151138)\n\n");
    printf("-----------------------------------------------------------------\n\n");

    // Initialize the random number generator
    srand(time(NULL));

    //------------------- Generating random alpha -------------------

    Point alphas[PRIME]; // Find all valid points alpha on the curve and store them in an array named alphas
    int num_alphas; // stores the number of valid points
    find_alphas(alphas, &num_alphas); // Find all valid points alpha on the curve

    // Choose a random alpha from the list of valid points on the curve
    Point alpha = choose_random_alpha(alphas, num_alphas);
    // Print the randomly chosen alpha point
    printf("Randomly chosen Alpha (obtained inside code): (%d, %d)\n\n", alpha.x, alpha.y);
    printf("-----------------------------------------------------------------\n\n");

    //------------------- Generating shared secret key SK -------------------

    // Input private keys for Alice and Bob
    int nA, nB;
    printf("Enter Alice's private key nA [1 to 330]: ");
    scanf("%d", &nA);
    printf("\nEnter Bob's private key nB [1 to 330]: ");
    scanf("%d", &nB);

    // Perform Diffie-Hellman key exchange to generate shared secret key SK
    Point public_key_Alice = ec_scalar_mul(nA, alpha); // Computing nA * alpha
    Point public_key_Bob = ec_scalar_mul(nB, alpha); // Computing nB * alpha

    // Public Key of Bob is given to Alice and then Alice calculates shared secret key SK
    Point shared_key_AB = ec_scalar_mul(nA, public_key_Bob); // Computing nA * (nB * alpha)

    // Public Key of Alice is given to Bob and then Bob calculates shared secret key SK
    Point shared_key_BA = ec_scalar_mul(nB, public_key_Alice); // Computing nB * (nA * alpha)

    // Checking if the shared secret keys are equal or not
    Point SK;
    // if shared_key_AB == shared_key_BA, then SK = shared_key_AB = shared_key_BA
    if (shared_key_AB.x == shared_key_BA.x && shared_key_AB.y == shared_key_BA.y) {
        SK.x = shared_key_AB.x;
        SK.y = shared_key_AB.y;
    }
    // if the shared secret keys are not equal, then print an error message and exit
    else {
        printf("Shared secret keys (SK) are not equal\n");
        printf("\n-----------------------------------------------------------------\n\n");
        return 0;
    }

    printf("\n-----------------------------------------------------------------\n\n");
    // Print the public key of Alice and Bob, along with shared secret key SK
    printf("Public Key of Alice: (%d, %d)\n", public_key_Alice.x, public_key_Alice.y);
    printf("Public Key of Bob: (%d, %d)\n", public_key_Bob.x, public_key_Bob.y);
    printf("\nShared Secret Key SK ( nA * nB * alpha ) : (%d, %d)\n", SK.x, SK.y);
    printf("\n-----------------------------------------------------------------\n");

    //------------------- Performing SHA-256 hash function -------------------

    uint8_t KA[SHA256_DIGEST_SIZE]; // 32 bytes for Alice's key
    uint8_t KB[SHA256_DIGEST_SIZE]; // 32 bytes for Bob's key

    // Extract x and y coordinates of SK for further computation
    uint32_t x1 = SK.x;
    uint32_t y1 = SK.y;

    // Concatenate x1 and y1 into x1y1 which will then be passed to the SHA
    // x1y1 = x1 || y1
    uint8_t x1y1[8];
    memcpy(x1y1, &x1, sizeof(uint32_t));
    memcpy(x1y1 + sizeof(uint32_t), &y1, sizeof(uint32_t));

    // Calculate SHA-256 hash for Alice
    // KA = SHA-256(x1y1) where x1y1 = x1 || y1
    sha256(x1y1, sizeof(x1y1), KA);
    printf("\nAlice's Key KA: ");
    print_key(KA, SHA256_DIGEST_SIZE); // Print the key KA in hexadecimal format

    // Calculate SHA-256 hash for Bob
    // KB = SHA-256(x1y1) where x1y1 = x1 || y1
    sha256(x1y1, sizeof(x1y1), KB);
    printf("Bob's Key KB: ");
    print_key(KB, SHA256_DIGEST_SIZE); // Print the key KB in hexadecimal format
    printf("\n-----------------------------------------------------------------\n");

    //------------------- Obtaining MA and Encrypting it into CA -------------------

    printf("\nEnter Alice's 128-bit message MA (16 space-separated bytes in hexadecimal): \n\n");
    uint8_t MA[16]; // 16 bytes for Alice's message
    for (int i = 0; i < 16; i++) {
        scanf("%2hhx", &MA[i]); // Read the 16 bytes of Alice's message
    }
    printf("\n-----------------------------------------------------------------\n");

    uint8_t CA[16] = {0}; // 1D array for output of encryption
    Triple_AESprime_encrypt(MA, KA, CA); // Perform triple AES prime encryption on MA with KA
    printf("\nEncryption of Alice's message (CA): ");
    for(int i = 0; i < 16; i++) {
        printf("%02x ", CA[i]); // Print the encrypted message CA in hexadecimal format
    }
    printf("\n\n-----------------------------------------------------------------\n");

    //------------------------- Generating MACA -------------------------

    uint8_t MACA[SHA256_DIGEST_SIZE]; // 32 bytes for Alice's MAC - MACA
    generate_mac(KA, MA, MACA); // Generate MACA
    printf("\nGenerated MACA: "); 
    for(int i = 0; i < sizeof(MACA); i++) {
        printf("%02x ", MACA[i]); // Print MACA in hexadecimal format
    }
    printf("\n");
    printf("\n-----------------------------------------------------------------\n");

    //------------------- Decrypting ciphertext CA back into plaintext MB -------------------

    // Bob recieves encrypted message CB on his end.
    // Below code simulates the transfer of CA from Alice to Bob.
    uint8_t CB[16] = {0}; // 1D array for CB
    memcpy(CB, CA, sizeof(CB));

    uint8_t MB[16] = {0}; // To store the decrypted message MB
    Triple_AESprime_decrypt(CB, KB, MB);
    printf("\nDecrypted message by Bob (MB): ");
    for(int i = 0; i < 16; i++) {
        printf("%02x ", MB[i]);
    }
    printf("\n\n-----------------------------------------------------------------\n");

    //------------------------- Generating MACB -------------------------

    // Bob recieves MACB on his end.
    // Below code simulates the transfer of MAC code from Alice to Bob.
    uint8_t MACB[SHA256_DIGEST_SIZE]; // 32 bytes for Bob's MAC - MACB
    memcpy(MACB, MACA, SHA256_DIGEST_SIZE);
    
    generate_mac(KB, MB, MACB); // Generate MACB
    printf("\nGenerated MACB: ");
    for(int i = 0; i < sizeof(MACB); i++) {
        printf("%02x ", MACB[i]); // Print MACB in hexadecimal format
    }
    printf("\n\n-----------------------------------------------------------------\n");
    
    //------------------- Validating the output for ensurity -------------------

    validate_output(KA, KB, MA, MB, MACA, MACB);
    printf("\n\n-----------------------------------------------------------------\n\n");

    //------------------- End of main() -------------------
    
    return 0;
}

//------------------------ END OF CODE ------------------------