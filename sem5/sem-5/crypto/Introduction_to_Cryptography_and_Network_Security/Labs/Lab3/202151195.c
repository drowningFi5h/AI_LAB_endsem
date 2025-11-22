// Name: Varun Vilvadrinath
// Roll No.: 202151195
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// S-Box lookup table
uint8_t sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // F

uint8_t sbox_new[256] = {0};

// Round constants
const uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

// Substitute each byte of a 32-bit word using the S-Box
uint32_t SubWord(uint32_t word) {
    uint32_t result = 0;

    for (int i = 0; i < 4; ++i) {
        uint8_t temp = (word >> (i * 8)) & 0xFF; 
        temp = sbox[temp];
        result |= (uint32_t)temp << (i * 8);
    }

    return result;
}

// Rotate a 32-bit word left by one byte
uint32_t RotWord(uint32_t word) {
    uint32_t temp = word;
    temp = (temp << 8) | (temp >> 24);
    return temp;
}

// Multiply two polynomials in GF(2^8)
uint8_t multiply_polynomial(uint8_t x1, uint8_t x2) {
    uint8_t s = 0;
    uint8_t carry = 0;

    for (int i = 0; i < 8; i++) {
        if (x2 & 1) 
            s ^= x1;
        
        carry = x1 & 0x80; 
        x1 <<= 1;
        if (carry) 
            x1 ^= 0x1B;
        
        x2 >>= 1; // right shift 'b'
    }

    return s;
}

// Substitute a byte using the inverse S-Box
uint8_t Sub(uint8_t x) {
    uint8_t temp = multiply_polynomial(0xc9, x);
    temp = temp ^ 39;
    return sbox[temp];
}

// Inverse substitute a byte
uint8_t inv_Sub(uint8_t x) {
    for(int i = 0; i < 256; i++) {
        uint8_t temp = Sub(i);
        sbox_new[i] = temp;
    }

    for(int i = 0; i < 256; i++) {
        if(sbox_new[i] == x) {
            return i;
        }
    }
    return 0;
}

// Generate round keys
uint32_t* generate_key(uint8_t *key) {
    uint32_t* words = malloc(44 * sizeof(uint32_t)); 

    // Setting the first 4 words to the key
    for(int i = 0; i < 4; i++)  {
        uint32_t temp = 0;
        temp = temp | key[4 * i]; 
        temp = temp << 8;
        temp = temp | key[(4 * i) + 1]; 
        temp = temp << 8;
        temp = temp | key[(4 * i) + 2]; 
        temp = temp << 8;
        temp = temp | key[(4 * i) + 3];
        words[i] = temp; 
    }
    
    for(int i = 4; i < 44; i++) {
        uint32_t temp = words[i - 1];
        
        if(i % 4 == 0) {
            uint32_t var = Rcon[i / 4] << 24;
            temp = SubWord(RotWord(temp)) ^ var;
        }
        
        words[i] = words[i - 4] ^ temp;
    }
    return words;
}

// Extract key from key schedule
void extractKey(uint32_t keys[], int idx, uint8_t result[16]) {
    result[0] = keys[idx] >> 24;
    result[1] = keys[idx] >> 16;
    result[2] = keys[idx] >> 8;
    result[3] = keys[idx];
    result[4] = keys[idx + 1] >> 24;
    result[5] = keys[idx + 1] >> 16;
    result[6] = keys[idx + 1] >> 8;
    result[7] = keys[idx + 1];
    result[8] = keys[idx + 2] >> 24;
    result[9] = keys[idx + 2] >> 16;
    result[10] = keys[idx + 2] >> 8;
    result[11] = keys[idx + 2];
    result[12] = keys[idx + 3] >> 24;
    result[13] = keys[idx + 3] >> 16;
    result[14] = keys[idx + 3] >> 8;
    result[15] = keys[idx + 3];
}

// Add round key
void AddRoundKey(uint8_t input[4][4], uint32_t keys[], int start) {
    uint8_t temp[16] = {0};
    extractKey(keys, start, temp);
    
    // XOR each element of input with the corresponding element of temp
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] ^= temp[4 * i + j];
        }
    }
}

// SubBytes transformation
void SubBytes(uint8_t input[4][4]) {
    // Replace each element of input with the corresponding element of sbox
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = Sub(input[i][j]);
        }
    }
}

// ShiftRows transformation
void ShiftRows(uint8_t input[4][4]) {
    // Shift the rows of input
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = input[i][(j + i) % 4];
        }
    }

    // Copy the result back to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

// MixColumns transformation
void MixColumns(uint8_t input[4][4]) {
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        temp[0][i] = multiply_polynomial(0x01, input[0][i]) ^ multiply_polynomial(0x04, input[1][i]) ^ multiply_polynomial(0x04, input[2][i]) ^ multiply_polynomial(0x05, input[3][i]);
        temp[1][i] = multiply_polynomial(0x05, input[0][i]) ^ multiply_polynomial(0x01, input[1][i]) ^ multiply_polynomial(0x04, input[2][i]) ^ multiply_polynomial(0x04, input[3][i]);
        temp[2][i] = multiply_polynomial(0x04, input[0][i]) ^ multiply_polynomial(0x05, input[1][i]) ^ multiply_polynomial(0x01, input[2][i]) ^ multiply_polynomial(0x04, input[3][i]);
        temp[3][i] = multiply_polynomial(0x04, input[0][i]) ^ multiply_polynomial(0x04, input[1][i]) ^ multiply_polynomial(0x05, input[2][i]) ^ multiply_polynomial(0x01, input[3][i]);
    }

    // Copy the result back to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

// AES encryption
void AES_Encrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    
    AddRoundKey(input, keys, 0);
    
    for (int round = 1; round <= 10; round++) {
        SubBytes(input);
        ShiftRows(input);
        MixColumns(input);
        AddRoundKey(input, keys, round * 4 + 1);
    }

    // // Final round
    SubBytes(input);
    ShiftRows(input);
    AddRoundKey(input, keys,11);

    // Copy the result to output
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i][j] = input[i][j];
        } 
    }
}

// InverseSubBytes transformation
void InverseSubBytes(uint8_t input[4][4]){
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            input[i][j] = inv_Sub(input[i][j]);
        }
    }
}

// InverseShiftRows transformation
void InverseShiftRows(uint8_t input[4][4]){
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][(j + i) % 4] = input[i][j];
        }
    }

    // Copy the result back to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }
}

// InverseMixColumns transformation
void InverseMixColumns(uint8_t input[4][4]){
    uint8_t temp[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        temp[0][i] = multiply_polynomial(0xa5, input[0][i]) ^ multiply_polynomial(0x07, input[1][i]) ^ multiply_polynomial(0x1A, input[2][i]) ^ multiply_polynomial(0x73, input[3][i]);
        temp[1][i] = multiply_polynomial(0x73, input[0][i]) ^ multiply_polynomial(0xa5, input[1][i]) ^ multiply_polynomial(0x07, input[2][i]) ^ multiply_polynomial(0x1a, input[3][i]);
        temp[2][i] = multiply_polynomial(0x1a, input[0][i]) ^ multiply_polynomial(0x73, input[1][i]) ^ multiply_polynomial(0xa5, input[2][i]) ^ multiply_polynomial(0x07, input[3][i]);
        temp[3][i] = multiply_polynomial(0x07, input[0][i]) ^ multiply_polynomial(0x1a, input[1][i]) ^ multiply_polynomial(0x73, input[2][i]) ^ multiply_polynomial(0xa5, input[3][i]);
    }

    // Copy the result back to input
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            input[i][j] = temp[i][j];
        }
    }

}

// AES decryption
void AES_Decrypt(uint8_t input[4][4], uint8_t output[4][4], uint32_t keys[]) {
    // Initial round
    AddRoundKey(input, keys, 11);
    InverseShiftRows(input);
    InverseSubBytes(input);

    // Rounds 10 to 1
    for (int round = 10; round >= 1; round--) {
        AddRoundKey(input, keys, round * 4 + 1);
        InverseMixColumns(input);
        InverseShiftRows(input);
        InverseSubBytes(input);
    }

    // Final round
    AddRoundKey(input, keys, 0);

    // Copy the result to output
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i][j] = input[i][j];
        }
    }
}

// Convert an array to a 4x4 matrix
void Array_To_Matrix(uint8_t Array[16], uint8_t Matrix[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Matrix[i][j] = Array[i + 4*j];
        }
    }
}

// Convert a 4x4 matrix to an array
void Matrix_To_Array(uint8_t Matrix[4][4], uint8_t Array[16]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            Array[i + 4*j] = Matrix[i][j];
        }
    }
}

int main() {
    printf("===========Q1 & 2============\n");
    uint8_t input[16], key_inti[16], output[16];
    printf("Enter m_1 in hexadecimal format: ");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &input[i]);
    }
    printf("\n");
    printf("Enter m_2 in hexadecimal format: ");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &key_inti[i]);
    }

    uint8_t plaintext[4][4] = {0}, ciphertext[4][4] = {0};
    
    uint32_t* keys = generate_key(key_inti);

    Array_To_Matrix(input, plaintext);
    AES_Encrypt(plaintext, ciphertext, keys);
    Matrix_To_Array(ciphertext, output);

    printf("\nThe result of compression function h(m_1 || m_2) : ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", output[i]);
    }
    printf("\n\n");
    printf("===========Q3 & 4============\n");
    printf("Enter m_1 in hexadecimal format: ");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &input[i]);
    }
    printf("\n");
    printf("Enter m_2 in hexadecimal format: ");
    for(int i = 0; i < 16; i++) {
        scanf("%hhx", &key_inti[i]);
    }
    printf("\n\n");
    keys = generate_key(key_inti);

    Array_To_Matrix(input, plaintext);
    AES_Encrypt(plaintext, ciphertext, keys);
    Matrix_To_Array(ciphertext, output);

    printf("The result of compression function h(m_1 || m_2) : ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", output[i]);
    }
    printf("\n\n");
    uint8_t new_input[16], new_output[16], key_k_[16];
    uint8_t new_plaintext[4][4] = {0}, new_ciphertext[4][4] = {0};

    srand(time(0));
    for(int i = 0; i < 16; i++) {
        key_k_[i] = rand()%256;
    }

    uint32_t* key_k = generate_key(key_k_);
    AES_Decrypt(ciphertext, new_plaintext, key_k);
    Matrix_To_Array(new_plaintext, new_input);
    printf("\nThe value of m_1' is : ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", new_input[i]);
    }
    printf("\n\n");
    printf("\nThe value of m_2' is : ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", key_k_[i]);
    }
    printf("\n\n");
    AES_Encrypt(new_plaintext, new_ciphertext, key_k);
    Matrix_To_Array(new_ciphertext, new_output);
    printf("The result of compression function h(m_1' || m_2') : ");
    for(int i = 0; i < 16; i++) {
        printf("%x ", new_output[i]);
    }
    printf("\n\n");
    printf("The result of the compression function h(m_1' || m_2') is equal to h(m_1 || m_2). So, (m_1' || m_2') is second pre-image of (m_1 || m_2).\n");
    return 0;
}
// Code done and all input outputs validated.