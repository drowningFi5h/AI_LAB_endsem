#include<stdio.h>
#include <stdint.h>

#define uint16_t unsigned short int
#define uint32_t unsigned long int
#define uint64_t unsigned long long int
#define uchar_t unsigned char


const uint16_t G = 0x011b;

unsigned char custom_subbyte_values[16][16];
void inverseMixColumns(uchar_t s[4][4]);

uchar_t subbyte_table[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

// the round constants for AES-prime, they are exactly similar to AES-128.
const uint32_t RCON[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

uint16_t product(uchar_t a, uchar_t b){
    uint16_t prod = 0; //stores the product

    for(int i = 0; i < 8; i++){  //running the loop from 0 to 7 as max degree of a is 7
        if((a >> i) & 1){        //checking if a has x^i, i.e., its ith bit is set
            uint16_t x = b << i;  //if x^i is in a, then multiply it with b
            prod = prod ^ x;      //and add it in the product
        }
    }
    return prod;
}

uchar_t subbyte(uchar_t x){
    uint16_t t2 = x & 15;    //least significant 4 bits as column number
    uint16_t t1 = x >> 4;    //most significant 4 bits as row number
    return custom_subbyte_values[t1][t2]; //table look-up
}

uchar_t inverseSubbyte(uchar_t x){
    uchar_t inv = 0;

    //finding x = subInp in the look-up table and calcuating y = inv (as describe above)
    for(uchar_t i = 0; i < 16; i++){
        for(uchar_t j = 0; j < 16; j++){
            if(custom_subbyte_values[i][j] == x){ 
                inv = ((i << 4) | j);
            }
        }
    }
    return inv;
}








//Shift Row function, left circular shifts the i^th row by i positions.
void shiftRow(uchar_t s[4][4]){
    for(int i = 1; i < 4; i++){ //0th row has shift of 0, therefore starting the loop from row 1
        uchar_t temp[4];    
        for(int j = 0; j < 4; j++) temp[j] = s[i][j];  //storing the ith row in temp array
        for(int j = 0; j < 4; j++){
            s[i][j] = temp[(j + i) % 4]; //left circular shifting the row ith by i positions
        }
    }
}



//Inverse Shift Row function, right circular shifts the i^th row by i positions
void invShiftRow(uchar_t s[4][4]){
    for(int i = 1; i < 4; i++){     //0th row has shift of 0, therefore starting the loop from row 1
        uchar_t temp[4];
        for(int j = 0; j < 4; j++) temp[j] = s[i][j];  //storing the ith row in temp array
        for(int j = 0; j < 4; j++){
            s[i][j] = temp[(j + 4 - i) % 4];  //right circular shifting the ith row by i positions
        }
    }
}







unsigned char multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char carry = 0;

    for (int i = 0; i < 8; i++) {
        if (b & 1) // if LSB of b is 1, add 'a' to result
            result ^= a;
        
        carry = a & 0x80; // check if MSB of a is 1
        a <<= 1; // left shift 'a'
        
        if (carry) // if MSB of a was 1, XOR with the polynomial x^8
            a ^= 0x1B; // 0x1B is the irreducible polynomial x^8 + x^4 + x^3 + x + 1
        
        b >>= 1; // right shift 'b'
    }

    return result;
}



// // Function to perform multiplication in GF(2^8) field
// uchar_t multiply(uchar_t a, uchar_t b) {
//     uchar_t result = 0;
//     uchar_t mask = 0x01;

//     for (int i = 0; i < 8; i++) {
//         if (b & mask) {
//             result ^= a;
//         }
//         uchar_t high_bit_set = a & 0x80;
//         a <<= 1;
//         if (high_bit_set) {
//             a ^= 0x1B; // XOR with 0x1B if the high bit is set
//         }
//     }
//     return result;
// }


void mixColumns(uchar_t s[4][4]){
    for(int i = 0; i < 4; i++){ //for each column of input matrix
        // Store the original column values
        uchar_t t0 = s[0][i];
        uchar_t t1 = s[1][i];
        uchar_t t2 = s[2][i];
        uchar_t t3 = s[3][i];
        
        // Calculate the new values using the provided mix column matrix
        s[0][i] = multiply(0x01, t0) ^ multiply(0x04, t1) ^ multiply(0x04, t2) ^ multiply(0x05, t3);
        s[1][i] = multiply(0x05, t0) ^ multiply(0x01, t1) ^ multiply(0x04, t2) ^ multiply(0x04, t3);
        s[2][i] = multiply(0x04, t0) ^ multiply(0x05, t1) ^ multiply(0x01, t2) ^ multiply(0x04, t3);
        s[3][i] = multiply(0x04, t0) ^ multiply(0x04, t1) ^ multiply(0x05, t2) ^ multiply(0x01, t3);
    }
}




//the matrix used to calculate the inverse of mix columns in AES-prime
const uchar_t M_inv[4][4] = {
    {0xa5, 0x07, 0x1a, 0x73},
    {0x73, 0xa5, 0x07, 0x1a},
    {0x1a, 0x73, 0xa5, 0x07},
    {0x07, 0x1a, 0x73, 0xa5}
};

void inverseMixColumns(uchar_t s[4][4]){
    for(int i = 0; i < 4; i++){ //for each column of input matrix
        uchar_t t0 = s[0][i];
        uchar_t t1 = s[1][i];
        uchar_t t2 = s[2][i];
        uchar_t t3 = s[3][i];
        
        s[0][i] = multiply(M_inv[0][0], t0) ^ multiply(M_inv[0][1], t1) ^ multiply(M_inv[0][2], t2) ^ multiply(M_inv[0][3], t3);
        s[1][i] = multiply(M_inv[1][0], t0) ^ multiply(M_inv[1][1], t1) ^ multiply(M_inv[1][2], t2) ^ multiply(M_inv[1][3], t3);
        s[2][i] = multiply(M_inv[2][0], t0) ^ multiply(M_inv[2][1], t1) ^ multiply(M_inv[2][2], t2) ^ multiply(M_inv[2][3], t3);
        s[3][i] = multiply(M_inv[3][0], t0) ^ multiply(M_inv[3][1], t1) ^ multiply(M_inv[3][2], t2) ^ multiply(M_inv[3][3], t3);
    }
}


//function to left circular shift a 32-bit word by 8 bits (or 1 byte)
uint32_t rotWord(uint32_t x){
    uchar_t z = 0xff; 
    uchar_t temp = (x >> 24) & z; //taking out the most significant byte
    // left shifting by 8 bits, here we will lose the most signifcant 8-bits from input (but, stored in temp) which
    //should have come at the least significant 8-bits position in the output.
    //all the least significant 8-bits of (x << 8) will be zero. we will or (x << 8) with temp to get the temp bits at LS byte positon.
    x = (x << 8) | temp; 
    return x;
}

//the original subbyte function of AES-128, its inverse will not be required as key remains the same
//during encryption as well as decryption
uchar_t subbyte_org(uchar_t x){
    uint16_t t2 = x & 15;    //least significant 4 bits as column number
    uint16_t t1 = x >> 4;    //most significant 4 bits as row number
    return subbyte_table[t1][t2]; //table look-up
}


//function to perform subbytes of each byte of the 32-bit word.
//Each word contains 4-bytes and we have to perform subbytes of each of them
uint32_t subWord(uint32_t x){
    uchar_t z = 0xff;
    uchar_t x0, x1, x2, x3; //x = x0 || x1 || x2 || x3, x0, x1, x2, x3 are bytes of the 32-bit word.
    x0 = (x >> 24) & z; 
    x1 = (x >> 16) & z; 
    x2 = (x >> 8) & z;
    x3 = x & z;

    //performing subbyte on each byte, since Key Scheduling uses original subbyte function.
    x0 = subbyte_org(x0);
    x1 = subbyte_org(x1);
    x2 = subbyte_org(x2);
    x3 = subbyte_org(x3);

    //output = subbyte(x0) || subbyte(x1) || subbyte(x2) || subbyte(x3)
    x = (x0 << 24) | (x1 << 16) | (x2 << 8) | x3;
    return x;
}

//this function generates the round keys for AES-prime, and is exactly similar to AES-128
void keyScheduling(uchar_t key[16], uchar_t roundKeys[11][4][4]){
    uint32_t words[44]; //44 words that will be generated will be stored here
    uchar_t z = 0xff;
    for(int i = 0; i < 4; i++){  //the first 4 words are similar to key, i.e, if we concatenate the first 4 words, we will get the key
        words[i] = (key[4*i] << 24) | (key[4*i+1] << 16) | (key[4*i+2] << 8) | (key[4*i+3]);
    }

    //finding remaining words according to the key scheduling algorithm
    for(int i = 4; i < 44; i++){
        uint32_t temp = words[i-1]; 
        if(i % 4 == 0) temp = subWord(rotWord(temp)) ^ (RCON[i/4 - 1]); //subWord function uses original subbyte function of AES-128
        words[i] = words[i-4] ^ temp;
    }
    
    //the 11 round keys are stored as 4*4 matrix in column-wise manner
    //each roundKey[i] is a round key.
    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                roundKeys[i][k][j] = (words[4*i+j] >> (24 - 8 * k)) & z; //breaking each word into 4 bytes
            }
        }
    }
}

//the round function f of AES-prime
void roundFunction(int r, uchar_t s[4][4]){
    //the variable r stores which round it is
    // perform subbyte(input)
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            s[i][j] = subbyte(s[i][j]);
        }
    }
    //perform shift row
    shiftRow(s);
    //if it is not the last,i.e. 10th round, perform mix columns.
    if(r < 10) mixColumns(s);
}

//the inverse round function f of AES-prime
void inverseRoundFunction(int r, uchar_t s[4][4]){
    //if it is the 10th round, we don't need to to mix column inverse, else we do.
    if(r != 10) inverseMixColumns(s);
    //perform shift row inverse
    invShiftRow(s);
    //perform subbyte inverse
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            s[i][j] = inverseSubbyte(s[i][j]);
        }
    }
}

//this function encrypts using the AES-prime algorithm
void encryptAES(uchar_t plaintext[16], uchar_t key[16], uchar_t ciphertext[16]){
    uchar_t s[4][4];
    uchar_t k[4][4];

    //first just store the key and plaintext in 4*4 matrices
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            s[j][i] = plaintext[i * 4 + j];
            k[j][i] = key[i * 4 + j];
        }
    }
    
    uchar_t roundKeys[11][4][4];
    keyScheduling(key, roundKeys); //generate roung keys

    //perform encryption
    for(int i = 0; i < 11; i++){
        //first mix the round keys
        for(int j = 0; j < 4; j++){
            for(int x = 0; x < 4; x++){
                s[j][x] = s[j][x] ^ roundKeys[i][j][x];
            }
        }
        
        //then perform the round function, since, there are only 10 rounds, therefore
        //round function is called for 10 times only
        if(i < 10) roundFunction(i+1, s);
        //in the last iteration of the loop, only the last round key is mixed (round function is not called)
    }

    //store the generated ciphertext in a 1-D array.
    int index = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            ciphertext[index++] = s[j][i];
        }
    }
}

//this function decrypts using the AES-prime algorithm
void decryptAES(uchar_t ciphertext[16], uchar_t key[16], uchar_t decrypted_text[16]){
    uchar_t s[4][4];
    uchar_t k[4][4];

    //first just store the key and ciphertext in 4*4 matrices
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            s[j][i] = ciphertext[i * 4 + j];
            k[j][i] = key[i * 4 + j];
        }
    }
    
    uchar_t roundKeys[11][4][4]; 
    keyScheduling(key, roundKeys); //generate round keys

    //perform decryption
    for(int i = 10; i >= 0; i--){
        //first mix the round keys
        for(int j = 0; j < 4; j++){
            for(int x = 0; x < 4; x++){
                s[j][x] = s[j][x] ^ roundKeys[i][j][x];
            }
        }

        //then perform the  inverse round function, since, there are only 10 rounds, therefore
        //inverse round function is called for 10 times only
        if(i > 0) inverseRoundFunction(i, s);
        //in the last iteration of the loop, only the last round key is mixed (inverse round function is not called)
    }

    //store the generated plaintext in a 1-D array
    int index = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            decrypted_text[index++] = s[j][i];
        }
    }
}

//helper function to print the array
void print(uchar_t s[16]){
    for(int i = 0; i < 16; i++){
        printf("%02x", s[i]);
    }
    printf("\n");
}














// AES S-Box lookup table
const unsigned char aes_sbox[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};



// Function to perform AES multiplication
unsigned char aes_multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    unsigned char carry = 0;

    for (int i = 0; i < 8; i++) {
        if (b & 1) // if LSB of b is 1, add 'a' to result
            result ^= a;
        
        carry = a & 0x80; // check if MSB of a is 1
        a <<= 1; // left shift 'a'
        
        if (carry) // if MSB of a was 1, XOR with the polynomial x^8
            a ^= 0x1B; // 0x1B is the irreducible polynomial x^8 + x^4 + x^3 + x + 1
        
        b >>= 1; // right shift 'b'
    }

    return result^39;
}



// Function to create custom SubByte table
void create_custom_subbyte_table(const unsigned char aes_sbox[16][16], unsigned char custom_subbyte_values[16][16], unsigned char mult) {
    unsigned char input1;

    // Loop through all possible input values
    for (int input = 0; input < 256; input++) {
        // Perform AES multiplication
        input1 = input;
        unsigned char output = aes_multiply(input1, mult);
        int output1 = output;

        // Look up the result in the AES S-Box to generate the custom SubByte value
        unsigned char custom_subbyte_value = aes_sbox[(output1 >> 4) & 0x0F][output1 & 0x0F];

        // Store the custom SubByte value in the array
        custom_subbyte_values[(input>>4) & 0x0F][input & 0x0F] = custom_subbyte_value;
    }
}













int main(){
    
    create_custom_subbyte_table(aes_sbox, custom_subbyte_values, 201);

    // Print the custom SubByte values for all inputs
    printf("Custom SubByte Table:\n");
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%02X ", custom_subbyte_values[i][j]);
        }
        printf("\n");
    }
    
    
    printf("\n\n\n");
    
    
    
    
    // uchar_t plaintext[20] = {81,82,9c,a6,d3,81,67,c9,f2,ff,67,8a,e0,ed,bb,12};
    

    // // printf("Enter 128-bit plaintext as input (in hexadecimal, each pair of hex digits separated by spaces): ");
    // // for(int i = 0; i < 16; i++) {
    // //     scanf("%hhx", &plaintext[i]);    //take plaintext as input
    // // }
 
    // uchar_t key[20] = {ab ,12 ,8c ,e9 ,ba ,d7 ,53 ,26 ,b0 ,97 ,b6 ,b1 ,24 ,39 ,ac ,12};
    // // printf("Enter 128-bit key as input (in hexadecimal, each pair of hex digits separated by spaces): ");
    // // for(int i = 0; i < 16; i++){
    // //     scanf("%hhx", &key[i]); //take key as input
    // // }
    
    uchar_t plaintext[16] = {0x81, 0x82, 0x9C, 0xA6, 0xD3, 0x81, 0x67, 0xC9, 0xF2, 0xFF, 0x67, 0x8A, 0xE0, 0xED, 0xBB, 0x12};
    uchar_t key[16] = {0xAB, 0x12, 0x8C, 0xE9, 0xBA, 0xD7, 0x53, 0x26, 0xB0, 0x97, 0xB6, 0xB1, 0x24, 0x39, 0xAC, 0x12};
    
    
    
    
    printf("\n\n\n");
    printf("Plaintext: "); //print plaintext
    print(plaintext);
    printf("Key: ");  //print key
    print(key);

    uchar_t ciphertext[16];
    encryptAES(plaintext, key, ciphertext); //generate ciphertext
    
    printf("Ciphertext: ");
    print(ciphertext); //print ciphertext

    uchar_t decrypted_text[16];
    decryptAES(ciphertext, key, decrypted_text); //generate decrypted text
    printf("Decrypted Text: "); //print decrypted text
    print(decrypted_text);

    return 0;
}

//Example Input:
// Plaintext: 81 82 9c a6 d3 81 67 c9 f2 ff 67 8a e0 ed bb 12
// Key: ab 12 8c e9 ba d7 53 26 b0 97 b6 b1 24 39 ac 12