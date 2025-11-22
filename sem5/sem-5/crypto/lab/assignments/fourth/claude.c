#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <openssl/sha.h>
#include <string.h>

// Prime number and curve parameters
#define PRIME 1223
#define A 439
#define B 133


// Elliptic Curve Point Structure
typedef struct {
    int x;
    int y;
    int is_infinity;
} Point;

// Function to perform modular arithmetic
int mod(int a, int m) {
    return ((a % m) + m) % m;
}

// Function to compute modular multiplicative inverse
int mod_inverse(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; x++) {
        if (mod(a * x, m) == 1) {
            return x;
        }
    }
    return -1; // No modular inverse exists
}

// Check if a point is on the elliptic curve
int is_point_on_curve(Point P) {
    if (P.is_infinity) return 1;
    
    // Verify y² = x³ + 439x + 133 (mod 1223)
    int left = mod(P.y * P.y, PRIME);
    int right = mod(P.x * P.x * P.x + A * P.x + B, PRIME);
    
    return left == right;
}
void compute_sha512_key(Point shared_secret, unsigned char* key) {
    // Convert shared secret point coordinates to string for hashing
    char point_data[100];
    sprintf(point_data, "%d%d", shared_secret.x, shared_secret.y);

    // Compute SHA-512 hash
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512((unsigned char*)point_data, strlen(point_data), hash);

    // Copy hash to key
    memcpy(key, hash, SHA512_DIGEST_LENGTH);
}

// Function to print key in byte format
void print_key(unsigned char* key, const char* label) {
    printf("%s: ", label);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        printf("%02x ", key[i]);
    }
    printf("\n");
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

// Print a point
void print_point(Point P, const char* label) {
    if (P.is_infinity) {
        printf("%s: Point at Infinity\n", label);
    } else {
        printf("%s: (%d, %d)\n", label, P.x, P.y);
    }
}

// Diffie-Hellman key exchange on elliptic curve
Point diffie_hellman_key_exchange(Point base_point, int private_key) {
    return point_multiply(base_point, private_key);
}

int main() {
    // Setup initial conditions
    Point alpha = find_random_point();
    print_point(alpha, "Random Point α");
    
    // Input private keys for Alice and Bob
    int nA, nB;
    printf("\nEnter Alice's private key (1-533): ");
    scanf("%d", &nA);
    
    printf("Enter Bob's private key (1-533): ");
    scanf("%d", &nB);
    
    // Validate private keys
    if (nA < 1 || nA > 533 || nB < 1 || nB > 533) {
        printf("Error: Private keys must be between 1 and 533.\n");
        return 1;
    }
    
    // Perform Diffie-Hellman key exchange
    Point AlicePublic = diffie_hellman_key_exchange(alpha, nA);
    Point BobPublic = diffie_hellman_key_exchange(alpha, nB);
    
    // Compute shared secret key
    Point SharedSecret = point_multiply(AlicePublic, nB);
    
    // Print results
    print_point(AlicePublic, "Alice's Public Key");
    print_point(BobPublic, "Bob's Public Key");
    print_point(SharedSecret, "Shared Secret Key SK");
    
    // Verify they compute the same shared secret
    Point BobSharedSecret = point_multiply(BobPublic, nA);
    
    if (BobSharedSecret.x == SharedSecret.x && 
        BobSharedSecret.y == SharedSecret.y && 
        BobSharedSecret.is_infinity == SharedSecret.is_infinity) {
        printf("\nDiffie-Hellman key exchange successful!\n");
    } else {
        printf("\nDiffie-Hellman key exchange failed.\n");
    }
    unsigned char Ka[SHA512_DIGEST_LENGTH];
    unsigned char Kb[SHA512_DIGEST_LENGTH];

    // Derive keys from shared secret
    compute_sha512_key(SharedSecret, Ka);
    compute_sha512_key(SharedSecret, Kb);

    // Print keys
    print_key(Ka, "Alice's Key Ka");
    print_key(Kb, "Bob's Key Kb");

    // Verify keys are identical
    if (memcmp(Ka, Kb, SHA512_DIGEST_LENGTH) == 0) {
        printf("\nKeys are identical!\n");
    } else {
        printf("\nKeys do not match!\n");
    }
    
    return 0;
}