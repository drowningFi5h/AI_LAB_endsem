//*************************************
// Name: Riya
// Roll No: 202151132
// Lab Assignment: 2
//*************************************

#include<stdio.h>

//S1-box as given in question
unsigned char S_box(unsigned char num) {
    unsigned char sbox[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
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
    
    //returning the value corresponding to num from sbox
    return sbox[num];
}
        

//function to generate round keys
//we will first split the initial key into 4 parts of 8 bits each (y0,y1,y2,y3)
//then we will apply s-box to each part as specified in the question
//round key for that i th round will be concatenation of y3,y2,y1,y0 after applying s-box
//then we will perform left circular shift on the key by i times for the ith round key
//we will repeat the process for 16 rounds to get 16 round keys

void generate_round_keys(unsigned int key, unsigned int round_keys[]){
    unsigned int y=key; //Intial key
    for(int i=0;i<16;i++){
        //splitting the key into 4 parts
        unsigned int y0=y & ((1<<8)-1); //y0 will contain the last 8 (least significant) bits of y
        unsigned int y1=(y>>8) & ((1<<8)-1); //y1 will contain the next 8 bits of y
        unsigned int y2=(y>>16) & ((1<<8)-1); //y2 will contain the next 8 bits of y
        unsigned int y3=(y>>24) & ((1<<8)-1); //y3 will contain the first 8 (most significant) bits of y
         
        //applying s-box to each part
        y0=S_box(y0);
        y1=S_box(y1);
        y2=S_box(y2);
        y3=S_box(y3);

        //concatenating the parts to get the round key
        round_keys[i]= (y3 << 24) | (y2 << 16) | (y1 << 8) | y0;

        //left circular shift by i times (here i+1 as loop starts from 0)
        y = ((y << (i + 1)) | (y >> (32 - (i + 1)))) & 0xFFFFFFFF;
    }
}


//function to perform round function
//it will take Ri(right half of the plaintext) and Ki(round key) as input
//then it will perform XOR of Ri and Ki as specified in question
//then we will split the result into 4 parts of 8 bits each (x0,x1,x2,x3)
//then we will apply s-box to each part
//then we will concatenate the parts to get the output of round function

unsigned int roundFunction(unsigned int Ri, unsigned int Ki) {
    // XOR Ri with Ki
    unsigned int XOR = Ri ^ Ki;

    //splitting the result into 4 parts
    int  x0 = XOR >> 24; //x0 will contain the first 8 (most significant) bits of XOR
    int  x1 = (XOR >> 16) & ((1<<8)-1); //x1 will contain the next 8 bits of XOR
    int  x2 = (XOR >> 8) & ((1<<8)-1); //x2 will contain the next 8 bits of XOR
    int x3 = XOR & ((1<<8)-1);  //x3 will contain the last 8 (least significant) bits of XOR

    //applying s-box to each part and then concatenating each part to get the output
    unsigned int S_X = (S_box(x0) << 24) | (S_box(x1) << 16) | (S_box(x2) << 8) | S_box(x3);

    return S_X;
}

//encryption function (decimal output)
//it will take plaintext and key and round_keys array as input
//we will generate the initial left and right halves of the plaintext
//then we will find the corresponding left and right halves after each round using the roundFunction
//we will repeat this process for 16 rounds to get the final left and right halves
//then we will concatenate the left and right halves to get the final ciphertext

unsigned long long encryption(unsigned long long plaintext, unsigned int key, unsigned int round_keys[]){

    //generating initial left and right halves of the plaintext
    unsigned long long l = plaintext >> 32; //most significant 32 bits
    unsigned long long r = plaintext & 0xFFFFFFFF; //least significant 32 bits

    //performing 16 rounds encryption
    for(int i=0;i<16;i++){
        unsigned int temp = r;
        r= l ^ roundFunction(r, round_keys[i]); //right half after ith round
        l = temp; //left half after ith round
    }

    //concatenating the left and right halves to get the final ciphertext
    //we tool left and right as 64 bits integer because , for concatenate them,we will need
    //to left shift the left half by 32 bits and then OR it with the right half
    //if we take 32 bits then left shift will make l as 0
    unsigned long long ciphertext = (l << 32) | r;

    return ciphertext;
}

//decryption function (decimal output)
//it will take ciphertext and key and round_keys array as input
//we will generate the initial left and right halves of the ciphertext
//then we will find the corresponding left and right halves after each round using the roundFunction
//we will repeat this process for 16 rounds to get the final left and right halves
////then we will concatenate the left and right halves to get the final plaintext

unsigned long long decryption(unsigned long long ciphertext, unsigned int key, unsigned int round_keys[]){

    //generating initial left and right halves of the ciphertext
    unsigned long long  left = ciphertext >> 32; //most significant 32 bits
    unsigned long long right = ciphertext & 0xFFFFFFFF; //least significant 32 bits

    //performing 16 rounds decryption
    //whole process will be same as encryption but we will use round keys in reverse order
    for(int i=15;i>=0;i--){
        unsigned int temp = left;
        left = right ^ roundFunction(left, round_keys[i]); //left half after ith round
        right = temp; //right half after ith round
    }

    //concatenating the left and right halves to get the final plaintext
    //we tool left and right as 64 bits integer because , for concatenate them,we will need
    //to left shift the left half by 32 bits and then OR it with the right half
    //if we take 32 bits then left shift will make l as 0
    unsigned long long decrypted_plaintext = (left << 32) | right;

    return decrypted_plaintext;
}

int main(){

    //taking 64 bit plaintext as input (in decimal)
    unsigned long long plaintext;
    printf("Enter plaintext: ");
    scanf("%llu", &plaintext);

    //taking 32 bit key as input (in decimal)
    unsigned int key;
    printf("Enter key: ");
    scanf("%u", &key);

    unsigned int round_keys[16];  //array to store round keys
    generate_round_keys(key, round_keys); //function to generate round keys

    //printing generated round keys for each round
    printf("Round keys:\n");
    for(int i=0;i<16;i++){
        printf("K%d = %u \n", i+1, round_keys[i]);
    }
    printf("\n");
    

    //encryption function to generate ciphertext
    unsigned long long ciphertext = encryption(plaintext, key, round_keys);
    printf("Final Ciphertext after 16 rounds(in decimal): %llu\n", ciphertext);

     printf("\n");

    //decrypting function to get back plaintext
    unsigned long long decrypted_plaintext = decryption(ciphertext, key, round_keys);
    printf("Decrypted plaintext(in decimal): %llu\n", decrypted_plaintext);

     printf("\n");

    //checking if both plaintext and decrypted plaintext are same
    if(plaintext == decrypted_plaintext){
        printf("Balle Balle!! Plaintext and Decrypted plaintext are same.\n");
    }
    else{
        printf("Plaintext and Decrypted plaintext are not same.\n");
    }


    return 0;
}