#include<stdio.h>
#include <time.h>
#include <stdlib.h>


//Subbyte function (Sub) as given in the question (Original Subbyte)
unsigned char Sub[256] = {
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
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

//Function to implement multiplication in a field (x^8 + x^4 + x^3 + x + 1) 
//It will take two unsigned char(hexadecimal numbers) as input and will return the 
//multiplication modulo (x^8 + x^4 + x^3 + x + 1) 
unsigned char multiply_field(unsigned char p, unsigned char q) {
    unsigned char ans = 0; 
    while (p && q) { 
        if (q & 1) {//checking if the LSB of b is 1 
            ans ^= p; 
        }
        if (p & 0x80){ //checking if MSB of a is 1 
            p = (p << 1) ^ 0x1B;  // if MSB is 1 then we will multiply a by x and then xor with 0x1B (x^8 + x^4 + x^3 + x + 1)
        }
        else{
            p <<= 1; //if MSB is 0 then we will multiply a by x i.e., left shift a by 1
        }
            q >>= 1; //right shift b by 1
    }
    return ans; 
}



unsigned char calculate_subbyte(unsigned char num)
{
    unsigned char var = multiply_field(0xc9, num);
    var = var^39;
    return Sub[var];
}


// Modified subbyte function Subbyte'
void subbyte(unsigned int s[4][4]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int res = calculate_subbyte(s[i][j]);
            s[i][j] = res;
        }
    }

}

unsigned char inv_Sub[256]={0};

unsigned char calculate_inv_subbyte(unsigned char num)
{
    for(int i=0;i<256;i++){
        unsigned char var = calculate_subbyte(i);
        inv_Sub[i] = var;
    }

    for( int i=0;i<256; i++){
        if(inv_Sub[i] == num){
            return i;
        }
    }
    return 0;
}

//Function to implement inverse subbyte operation
void inv_subbyte(unsigned int s[4][4]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int res = calculate_inv_subbyte(s[i][j]);
            s[i][j] = res;
        }
    }
}


//Function for implementing shift rows operation 
//We will take a 4x4 matrix as input and will apply shift rows operation as :
//In shift rows operation, we shift the ith row by i positions to the left
//So, the first row will be unchanges(0th row) and similarly the second row will be shifted by 1 position to the left
//and similarly 3rd and 4th row will be shifted by 2 and 3 positions to the left respectively

void shift_rows(unsigned char s[4][4]){
    int shifts[4]={0,1,2,3}; //shift values for each row
    unsigned char var; 
    for(int i=1;i<4;i++){ // here i =row number
        for(int j=0;j<shifts[i];j++){ 
            var=s[i][0]; //store the first element of the row
            for(int k=0;k<3;k++){ 
                s[i][k]=s[i][(k+1)%4]; //shifting the elements of the row to the left by 1 position
            }

            s[i][3]=var; // keep the first(after each shift) element at the last position
        }
    }

}

//Function for implementing inverse shift rows operation
//In inverse shift rows operation, we will shift the ith row by i positions to the right
//So, the first row will be unchanged(0th row) and similarly the second row will be shifted by 1 position to the right
//and similarly 3rd and 4th row will be shifted by 2 and 3 positions to the right respectively
void inv_shift_rows(unsigned char s[4][4]){
    int shifts[4]={0,1,2,3}; //shift values for each row in reverse order
    unsigned char var;
    for(int i=1;i<4;i++){ // here i =row number
        for(int j=0;j<shifts[i];j++){ 
            var=s[i][3]; //store the last element of the row
            for(int k=3;k>0;k--){ 
                s[i][k]=s[i][(k-1)%4]; //shifting the elements of the row to the right by 1 position
            }

            s[i][0]=var; // keep the last(after each shift) element at the first position
        }
    }

}


// Mix column matrix M as given in the question
unsigned char M[4][4] = {
    {0x01, 0x04, 0x04, 0x05},
    {0x05, 0x01, 0x04, 0x04},
    {0x04, 0x05, 0x01, 0x04},
    {0x04, 0x04, 0x05, 0x01}
};

// Inverse mix column matrix M_inv as given in the question
unsigned char M_inv[4][4] = {
    {0xa5, 0x07, 0x1a, 0x73},
    {0x73, 0xa5, 0x07, 0x1a},
    {0x1a, 0x73, 0xa5, 0x07},
    {0x07, 0x1a, 0x73, 0xa5}
};


//Function to implement mixColumn operation of AES 
//Here, we will take a 4x4 matrix as input and we will multiply the matrix with the mix column matrix M
//The multiplication will be done in the field (x^8 + x^4 + x^3 + x + 1) using function multiply_field
void mixColumn(unsigned char s[4][4]) {
    unsigned char var[4][4]; //temporary matrix to store the result of multiplication

    //Now, we will multiply the s with the mix column matrix M
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            var[i][j] = 0; 
            for (int k = 0; k < 4; k++) {
                //Performing matrix multiplication of s and M 
                var[i][j] ^= multiply_field(M[i][k], s[k][j]); 
            }
        }
    }

    //we will store the result back to s matrix 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s[i][j] = var[i][j];
        }
    }
}

//Function to implement inverse mixColumn operation of AES
//Here, we will take a 4x4 matrix as input and we will multiply the matrix with the inverse mix column matrix M_inv
//The multiplication will be done in the field (x^8 + x^4 + x^3 + x + 1) using function multiply_field
void inv_mixColumn(unsigned char s[4][4]) {
    unsigned char var[4][4]; //temporary matrix to store the result of multiplication

    //Now we will multiply the s with the inverse mix column matrix M_inv
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            var[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                //Performing matrix multiplication of s and M_inv
                var[i][j] ^= multiply_field(M_inv[i][k], s[k][j]);
            }
        }
    }

    //we will store the result back to s matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s[i][j] = var[i][j];
        }
    }
}


//Function to implement the SUBWORD operation
//Here, we will take a 32-bit word as input and will break it into 4 bytes
//then we will apply the subbyte operation on each byte and then concatenate the bytes to form a 32-bit word
unsigned int SUBWORD(unsigned int word){

//     unsigned char b[4]; //array to store the bytes of the word x
//     b[0] = (word >> 24) & 0xFF; //first byte (MSB)
//     b[1] = (word >> 16) & 0xFF; //second byte
//     b[2] = (word >> 8) & 0xFF; //third byte
//     b[3] = word & 0xFF; //fourth byte (LSB)
    

//     //applying subbyte operation on each byte and then concatenating the bytes
//     //return the 32-bit word formed by concatenating the bytes
//     unsigned int ans=(subbyte(b[0]) << 24) | (subbyte(b[1]) << 16) | (subbyte(b[2]) << 8) | subbyte(b[3]);
//     return ans;
// }

    unsigned char b[4]; //array to store the bytes of the word x
    b[0] = (word >> 24) & 0xFF; //first byte (MSB)
    b[1] = (word >> 16) & 0xFF; //second byte
    b[2] = (word >> 8) & 0xFF; //third byte
    b[3] = word & 0xFF; //fourth byte (LSB)

    //applying subbyte operation on each byte and then concatenating the bytes
    //return the 32-bit word formed by concatenating the bytes
    unsigned int ans = 0;
    ans = calculate_subbyte(b[0]);
    ans = (ans << 8) | calculate_subbyte(b[1]);
    ans = (ans << 8) | calculate_subbyte(b[2]);
    ans = (ans << 8) | calculate_subbyte(b[3]);
    return ans;

}

//Function to implement the ROTWORD operation
//Here, we will take a 32-bit word as input and will rotate the word to the left by 8 bits
unsigned int ROTWORD(unsigned int word){
    
    //we will rotate the word to the left by 8 bits
    //Here x<<8 (left shift by 8 bits)will give the most significant 24 bits of the word 
    //and x>>24 (right shift by 24 bits) will give the least significant 8 bits of the word
    unsigned int var=word;
    unsigned int ans= (var<< 8) | (var >> 24);
    return ans;
}

//RCON table (constants used in key scheduling algorithm for AES) 
unsigned char RCON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

//Function to implement AES-128 key scheduling algorithm
void key_scheduling_algo(unsigned char keys[16], unsigned char round_keys[11][4][4]){

    //key scheduling algorithm generates 44 words, the first four words are the key itself
    //The remaining 40 words are generated from the previous word

    unsigned int w[44]; //array to store the 44 words

    //first 4 words will be same as the key
    for(int i=0;i<4;i++){ 
        w[i] = (keys[4*i] << 24) | (keys[4*i+1] << 16) | (keys[4*i+2] << 8) | keys[4*i+3];
    }

    //remaining words will be generated from the previous word using SubWord,Rotword and  RCON operations
    for(int i=4;i<44;i++){
        unsigned int var=w[i-1];
        if(i%4==0){
            var=SUBWORD(ROTWORD(var)) ^ RCON[i/4];
        }
        w[i]=w[i-4]^var;
        
    }

    //Now, we will store the 44 words in the round_keys array
    //There will be 11 round keys, each round key will be a 4x4 matrix
    //for each 4x4 matrix we will take 4 words from the w array and store them in the matrix
    for(int i=0;i<11;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                //we are taking 4 words from the w array each of 32 bits and storing them in the round_keys array
                //24-8*k will give the kth byte of the jth word
                //&0xFF will give the byte value
                round_keys[i][k][j] = (w[4*i+j] >> 24-8*k) & 0xFF;
            }
        }
    }
    
}

//Function for implementing round function
//Here, we will take a 4x4 matrix and a 4x4 round key as input
//We will apply the subbyte operation on each element of the matrix
//Then we will apply the shift rows operation on the matrix
//Then we will apply the mix column operation on the matrix
//Then we will apply the add round key operation on the matrix
void round_function(unsigned char s[4][4], unsigned char round_key[4][4]){
    //here we will apply the subbyte operation on each element of the matrix
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] = calculate_subbyte(s[i][j]);
        }
    }

    //Shift rows operation
    shift_rows(s);

    //Mix column operation
    mixColumn(s);

    //here we will apply the add round key operation in which we will xor the matrix with the round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_key[i][j];
        }
    }
}


//Function for implementing inverse round function
//Here, we will take a 4x4 matrix and a 4x4 round key as input
//We will apply the inverse subbyte operation on each element of the matrix
//Then we will apply the inverse shift rows operation on the matrix
//Then we will apply the inverse mix column operation on the matrix
//Then we will apply the add round key operation on the matrix
void inv_round_function(unsigned char s[4][4], unsigned char round_key[4][4]){
    //here we will apply the inverse subbyte operation on each element of the matrix
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] = calculate_inv_subbyte(s[i][j]);
        }
    }

    //Inverse shift rows operation
    inv_shift_rows(s);

    //Inverse mix column operation
    inv_mixColumn(s);

    //here we will apply the add round key operation in which we will xor the matrix with the round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_key[i][j];
        }
    }
}

//Function to implement AES encryption for 10 rounds
//we will take a 16 byte plaintext, 16 byte key and 16 byte ciphertext as input
//then we will generate the round keys using key scheduling algorithm
//then we will apply the initial round key addition
//then we will apply 9 rounds of round function
//then we will apply the final round
void Encryption_AES(unsigned char plaintext[16], unsigned char key[16], unsigned char ciphertext[16]){
    unsigned char round_keys[11][4][4]; //round keys array to store the 11 round keys

    //Applying key scheduling algorithm to generate the round keys
    key_scheduling_algo(key, round_keys); 

    //Here we will store the plaintext in a 4x4 matrix
    unsigned char s[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[j][i] = plaintext[4*i+j];
        }
    }


    //Initial round key addition - Here we will xor the matrix with the first round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_keys[0][i][j];
        }
    }

    //First 9 rounds 
    for(int i=1;i<10;i++){
        round_function(s, round_keys[i]);
    }

    //Final round 
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] = calculate_subbyte(s[i][j]);
        }
    }

    //Shift rows operation
    shift_rows(s);

    //Add round key operation - XOR the s with the last round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_keys[10][i][j];
        }
    }


    //Stroing the ciphertext
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            ciphertext[4*i+j] = s[j][i];
        }
    }
}


//Function to implement AES decryption for 10 rounds
//we will take a 16 byte ciphertext, 16 byte key and 16 byte plaintext as input
//then we will generate the round keys using key scheduling algorithm
//then we will apply the initial round key addition
//then we will apply 9 rounds of inverse round function
//then we will apply the final round
void Decryption_AES(unsigned char ciphertext[16], unsigned char key[16], unsigned char plaintext[16]){
    unsigned char round_keys[11][4][4]; //round keys array to store the 11 round keys

    //Applying key scheduling algorithm to generate the round keys
    key_scheduling_algo(key, round_keys); 

    //Here we will store the ciphertext in a 4x4 matrix
    unsigned char s[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[j][i] = ciphertext[4*i+j];
        }
    }

    //Initial round key addition - Here we will xor the matrix with the last round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_keys[10][i][j];
        }
    }

    //First 9 rounds 
    for(int i=9;i>0;i--){
        inv_round_function(s, round_keys[i]);
    }

    //Final round 
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] = calculate_inv_subbyte(s[i][j]);
        }
    }

    //Inverse shift rows operation
    inv_shift_rows(s);

    //Add round key operation - XOR the s with the first round key
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            s[i][j] ^= round_keys[0][i][j];
        }
    }

    //Stroing the plaintext
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            plaintext[4*i+j] = s[j][i];
        }
    }
}


int main(){

    //Q-1*************************************

    // Here m1 is the first 16 byte message and m2 is the second 16 byte message
    // for the AES encryption, m2 will act as the key and m1 will act as the plaintext
    unsigned char m1[16], m2[16];
    printf("Enter the 16 bytes(128 bits) hexadecimal value of m1: ");
    for(int i=0;i<16;i++){
        scanf("%hhx", &m1[i]);
    }
    printf("\n\nEnter the 16 byte(128 bits) hexadecimal value of m2: ");
    for(int i=0;i<16;i++){
        scanf("%hhx", &m2[i]);
    }

    unsigned char ciphertext[16]; 

    //calling the encryption function to get the value of h(m1||m2)
    Encryption_AES(m1, m2, ciphertext);


//Q-2*************************************


    //printing the value of h(m1||m2)
    printf("\n\nThe value of the compression function h(m1||m2) is: ");
    for(int i=0;i<16;i++){
        printf("%02x", ciphertext[i]);
    }
    printf("\n\n");



//Q-3*************************************

// we will take a random input m2'(key) and we will use the ciphertext as m1' and then we will decrypt the ciphertext to get m1'

    unsigned char m2_p[16]; 
    //generating random values for m2_p using rand function
    srand(time(0));
    for(int i=0;i<16;i++){
        m2_p[i] = rand()%256;
    }
    //Calling the decryption function to get the value of m1' corresponding to the random input m2'
    unsigned char plaintext[16]; 
    Decryption_AES(ciphertext, m2_p, plaintext);

     //The value of m1' is
    printf("\n\nThe value of m1' is: ");
    for(int i=0;i<16;i++){
        printf("%02x", plaintext[i]);
    }
    printf("\n\n");

//Q-4*************************************

   
    //Calculating compressed output h(m1'||m2')
    unsigned char ciphertext_p[16]; 
    Encryption_AES(plaintext, m2_p, ciphertext_p);
    printf("\n\nCompression of h(m1'||m2') is: ");
    for(int i=0;i<16;i++){
        printf("%02x", ciphertext_p[i]);
    }
    printf("\n\n");


    return 0;
}