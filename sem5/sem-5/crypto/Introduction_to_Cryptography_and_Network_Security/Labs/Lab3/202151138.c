/*
Name: Sanidhya Kumar
Roll No: 202151138
Lab Assignment 03
Date: 10th April 2024
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

//------------------------ MISC FUNCTIONS ------------------------

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

//------------------------ PRE-DEFINED VARIABLES ------------------------

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

//------------------------ FUNCTIONS FOR ENCRYPTION ------------------------

// Subbyte as defined in the question
uint8_t Sub(uint8_t x) {
    uint8_t temp = aes_multiply(0xc9, x);
    temp = temp ^ 39;
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

//------------------------ FUNCTIONS FOR DECRYPTION ------------------------

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

//------------------------ FUNCTIONS FOR KEY-SCHEDULING ------------------------

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
    result[3] = keys[i];
    result[4] = keys[i + 1] >> 24;
    result[5] = keys[i + 1] >> 16;
    result[6] = keys[i + 1] >> 8;
    result[7] = keys[i + 1];
    result[8] = keys[i + 2] >> 24;
    result[9] = keys[i + 2] >> 16;
    result[10] = keys[i + 2] >> 8;
    result[11] = keys[i + 2];
    result[12] = keys[i + 3] >> 24;
    result[13] = keys[i + 3] >> 16;
    result[14] = keys[i + 3] >> 8;
    result[15] = keys[i + 3];
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

//------------------------ AES ENCRYPTION FUNC ------------------------

// Function to perform AES encryption
void AESprime_encrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    // Initial XOR operation with round key
    XOR_round_key(input, keys, 0);

    // Performing 10 rounds of encryption
    for (int round = 1; round <= 10; round++) {
        SubBytes(input); // SubBytes operation
        ShiftRows(input); // ShiftRows operation
        MixColumns(input); // MixColumns operation
        XOR_round_key(input, keys, round * 4 + 1); // XOR operation with round key
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

//------------------------ AES DECRYPTION FUNC ------------------------

// Function to perform AES decryption
void AESprime_Decrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    XOR_round_key(input, keys, 11); // Initial XOR operation with round key
    inv_ShiftRows(input); // Inverse ShiftRows operation
    inv_SubBytes(input); // Inverse SubBytes operation

    // Performing 10 rounds of decryption
    for (int round = 10; round >= 1; round--) {
        XOR_round_key(input, keys, round * 4 + 1); // XOR operation with round key
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

//------------------------ MAIN FUNCTION ------------------------

int main() {
    printf("-----------------------------------------------\n\n");
    printf("Welcome to the Cryptography Lab Assignment 03\nMade by Sanidhya Kumar (202151138)\n\n");
    printf("-----------------------------------------------\n\n");

    //------------------- Defining Variables -------------------

    uint8_t input[16];
    uint8_t key_inti[16]; // Initial key
    uint8_t output[16];
    uint8_t plaintext[4][4] = {0}; // 4x4 matrix for plaintext
    uint8_t ciphertext[4][4] = {0}; // 4x4 matrix for ciphertext
    uint8_t new_input[16];
    uint8_t new_output[16];
    uint8_t key_k_[16];
    uint8_t new_plaintext[4][4] = {0};
    uint8_t new_ciphertext[4][4] = {0};

    //------------------- Question 1 and 2 -------------------

    printf("        Question 1 and Question 2");
    printf("\n\n-----------------------------------------------\n");


    printf("\nEnter m_1 (in hex with each byte on a new line): \n");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &input[i]); // Taking input in hex
    }
    printf("\nEnter m_2 (in hex with each byte on a new line): \n");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &key_inti[i]); // Taking input in hex
    }
    
    // Getting round keys from Key Schedule Function
    uint32_t* keys = key_scheduling_algo(key_inti);

    Array2Matrix(input, plaintext);
    AESprime_encrypt(plaintext, ciphertext, keys); // Encrypting the plaintext using AES algorithm
    Matrix2Array(ciphertext, output);

    // Printing the output of Q1 and Q2
    printf("\nAnswer obtained from compression function h(m_1 || m_2) is: ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", output[i]);
    }

    //------------------- Question 3 and 4 -------------------

    printf("\n\n-----------------------------------------------\n\n");
    printf("        Question 3 and Question 4");
    printf("\n\n-----------------------------------------------\n");

    // setting seed for random function
    srand(time(0));
    for(int i = 0; i < 16; i++) {
        key_k_[i] = rand()%256;
    }

    // Getting round keys from Key Schedule Function
    uint32_t* key_k = key_scheduling_algo(key_k_);

    printf("\nValues of m1 and m2 are taken same as m_1 and m_2 from previous question.\n");

    // Decrypting the ciphertext using AES algorithm
    AESprime_Decrypt(ciphertext, new_plaintext, key_k);
    Matrix2Array(new_plaintext, new_input);

    // Printing the obtained values of m_1'
    printf("\nThe obtained value for m_1' is: ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", new_input[i]);
    }
    // Printing the obtained values of m_2'
    printf("\n\nThe obtained value for m_2' is: ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", key_k_[i]);
    }

    // Checking if compression function h on m1' || m2' is same as that on m1 || m2
    AESprime_encrypt(new_plaintext, new_ciphertext, key_k);
    Matrix2Array(new_ciphertext, new_output);

    // Printing the answer obtained from m_1' and m_2'
    printf("\n\nAnswer obtained from compression function h(m_1' || m_2') is: ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", new_output[i]);
    }

    //------------------- Conclusions -------------------

    // Conclusions based on the results of questions 1, 2, 3 and 4.
    printf("\n\n-----------------------------------------------\n\n");
    printf("Answers of compression function h(m_1' || m_2') = h(m_1 || m_2).");
    printf("\n\nTherefore, (m_1' || m_2') is second pre-image of (m_1 || m_2).\n");
    printf("\n-----------------------------------------------\n\n");

    //------------------- END -------------------

    return 0;
}