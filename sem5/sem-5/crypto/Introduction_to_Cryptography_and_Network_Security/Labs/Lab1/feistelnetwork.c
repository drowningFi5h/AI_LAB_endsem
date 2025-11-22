#include<stdio.h>

int main(){
    unsigned long long P;
    unsigned long long P_decrypted;
    unsigned long long C;
    unsigned int L1;
    unsigned int R1;
    unsigned int L0;
    unsigned int R0;
    unsigned int K;
    unsigned long long one = 1;
    unsigned int temp = (one<<32)-1;

    printf("Enter Plaintext P: ");
    scanf("%llu", &P);

    printf("Enter Key K: ");
    scanf("%u", &K);

    R0 = (P&temp);
    L0 = (P>>32);
    L1 = R0;
    R1 = L0 ^ (R0 ^ K);
    C = L1;
    C = (C<<32) | R1;
    printf("Encryption: ");
    printf("%llu", C);

    printf("\n");

    L1 = (C>>32);
    R1 = (C&temp);
    R0 = L1;
    L0 = R1 ^ (L1 ^ K);
    P_decrypted = L0;
    P_decrypted = (P_decrypted<<32) | R0;
    printf("Decryption: ");
    printf("%llu", P_decrypted);

    return 0;
}