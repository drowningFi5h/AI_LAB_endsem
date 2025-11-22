/*
Name: Sanidhya Kumar
Roll No: 202151138
Lab Assignment 02
Date: 26th Feb 2024
*/

#include <stdio.h>

// S1-box as given in question
unsigned char S1_box[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// S1-inverse box
unsigned char S1_inverse[256];

// Function to generate S1-inverse
void generateS1Inverse()
{
    for (int i = 0; i < 256; ++i)
    {
        unsigned char index = S1_box[i];
        S1_inverse[index] = i;
    }
}

// Function to perform left circular shift
unsigned int left_circular_shift(unsigned int value, int shift)
{
    // Shifting the value to left by shift + 1 and then ORing it with the value shifted to right by 32 - (shift + 1)
    return ((value << (shift + 1)) | (value >> (32 - (shift + 1)))) & 0xFFFFFFFF;
}

// Function to generate round keys
void key_scheduling_algo(unsigned int k, unsigned int round_keys[])
{
    unsigned int y = k;
    // Generating 16 round keys using the algorithm mentioned in the question
    for (int i = 0; i < 16; i++)
    {
        // Extracting the 4 bytes from the 32 bit key
        unsigned int y3 = y & ((1 << 8) - 1);         // Extracting the last 8 bits
        unsigned int y2 = (y >> 8) & ((1 << 8) - 1);  // Extracting the next 8 bits
        unsigned int y1 = (y >> 16) & ((1 << 8) - 1); // Extracting the next 8 bits
        unsigned int y0 = (y >> 24) & ((1 << 8) - 1); // Extracting the first 8 bits

        // Applying S1-box to each of the 4 bytes
        y0 = S1_box[y0];
        y1 = S1_box[y1];
        y2 = S1_box[y2];
        y3 = S1_box[y3];

        // Generating the round key using the 4 bytes
        round_keys[i] = (y0 << 24) | (y1 << 16) | (y2 << 8) | y3;

        // Left circular shifting the key by i
        y = left_circular_shift(y, i);
    }
}

// Function to perform round function
unsigned int round_fn(unsigned int ri, unsigned int ki)
{
    // Performing XOR of ri and ki
    unsigned int rixorki = ri ^ ki;

    // Extracting the 4 bytes from the 32 bit value
    int x0 = rixorki >> 24;                    // Extracting the first 8 bits
    int x1 = (rixorki >> 16) & ((1 << 8) - 1); // Extracting the next 8 bits
    int x2 = (rixorki >> 8) & ((1 << 8) - 1);  // Extracting the next 8 bits
    int x3 = rixorki & ((1 << 8) - 1);         // Extracting the last 8 bits

    // Applying S1-inverse to each of the 4 bytes
    return (S1_inverse[x0] << 24) | (S1_inverse[x1] << 16) | (S1_inverse[x2] << 8) | S1_inverse[x3];
}

// Function to perform encryption
unsigned long long encryption(unsigned long long p, unsigned int round_keys[])
{
    // Extracting the left and right 32 bits from the 64 bit plaintext
    unsigned long long l = p >> 32;
    unsigned long long r = p & 0xFFFFFFFF;

    // Performing 16 rounds of encryption
    for (int i = 0; i < 16; i++)
    {
        unsigned int temp = r;
        r = l ^ round_fn(r, round_keys[i]);
        l = temp;
    }

    // Combining the left and right 32 bits to form the 64 bit ciphertext
    unsigned long long c = (l << 32) | r;
    return c;
}

// Function to perform decryption
unsigned long long decryption(unsigned long long c, unsigned int round_keys[])
{
    // Extracting the left and right 32 bits from the 64 bit ciphertext
    unsigned long long l = c >> 32;
    unsigned long long r = c & 0xFFFFFFFF;

    // Performing 16 rounds of decryption
    for (int i = 15; i >= 0; i--)
    {
        unsigned int temp = l;
        l = r ^ round_fn(l, round_keys[i]);
        r = temp;
    }

    // Combining the left and right 32 bits to form the 64 bit decrypted plaintext
    unsigned long long decrypted_p = (l << 32) | r;
    return decrypted_p;
}

/*----------------------------- MAIN FUNCTION -----------------------------*/

int main()
{
    printf("-----------------------------------------------\n\n");
    printf("Welcome to the Cryptography Lab Assignment 02\nMade by Sanidhya Kumar (202151138)\n\n");
    printf("-----------------------------------------------\n\n");

    /*------------------------- Defining Variables -------------------------*/

    // Define all the needed variables
    unsigned long long p;        // Since we are using 64 bits, we need to use unsigned long long
    unsigned int k;              // Since we are using 32 bits, we need to use unsigned int
    unsigned int round_keys[16]; // Since we are using 16 rounds, we need to use unsigned int

    // Generating S1-inverse
    generateS1Inverse();

    /*------------------------- Encryption Process -------------------------*/

    // Taking input of plaintext and key k from the user
    printf("Enter Plaintext P (in decimal): ");
    scanf("%llu", &p);
    printf("Enter key K: ");
    scanf("%u", &k);

    printf("\n-----------------------------------------------\n\n");

    // Generating round keys using the key scheduling algorithm
    key_scheduling_algo(k, round_keys);

    // Printing the round keys
    for (int i = 0; i < 16; i++)
    {
        printf("Round key %d: %u\n", i, round_keys[i]);
    }

    printf("\n-----------------------------------------------\n\n");

    unsigned long long int c = encryption(p, round_keys);
    printf("Encryption of Plaintext P using K is: %llu\n", c);

    printf("\n-----------------------------------------------\n\n");

    /*------------------------- Decryption Process -------------------------*/

    unsigned long long decrypted_p = decryption(c, round_keys);
    printf("Decrypted Plaintext (in decimal): %llu\n", decrypted_p);
    printf("\n-----------------------------------------------\n\n");

    /*------------------------- Checking our Process -------------------------*/

    // Checking if the plaintext and decrypted plaintext are same or not.
    if (p == decrypted_p)
    {
        printf("Yayy! Plaintext and Decrypted plaintext are same.\n");
    }
    else
    {
        printf("OOPS! Plaintext and Decrypted plaintext are not same.\n");
    }
    printf("\n-----------------------------------------------\n\n");

    /*------------------------- END -------------------------*/

    return 0;
}