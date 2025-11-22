#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate (base^exp) % mod
int mod_pow(int base, int exp, int mod) {
    int result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    // Publicly known prime number and primitive root
    int prime = 3319; // Example prime number
    int primitive_root = 6; // Example primitive root

    // Alice's private key
    int alice_private_key = 1197; // Example private key for Alice

    // Bob's private key
    int bob_private_key = 62; // Example private key for Bob

    // Calculate public keys for Alice and Bob
    int alice_public_key = mod_pow(primitive_root, alice_private_key, prime);
    int bob_public_key = mod_pow(primitive_root, bob_private_key, prime);

    // Simulate exchange of public keys

    // Calculate shared secret for Alice and Bob
    int shared_secret_alice = mod_pow(bob_public_key, alice_private_key, prime);
    int shared_secret_bob = mod_pow(alice_public_key, bob_private_key, prime);

    printf("Alice's public key: %d\n", alice_public_key);
    printf("Bob's public key: %d\n", bob_public_key);
    printf("Shared secret computed by Alice: %d\n", shared_secret_alice);
    printf("Shared secret computed by Bob: %d\n", shared_secret_bob);

    return 0;
}
