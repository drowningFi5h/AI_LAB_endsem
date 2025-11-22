#include <stdio.h>

int isCoprime(int a) {
    for (int i = 2; i < 26; ++i) {
        if ((a * i) % 26 == 1) {
            return 1;
        }
    }
    return 0;
}

int modInverse(int a) {
    for (int i = 1; i < 26; ++i) {
        if ((a * i) % 26 == 1) {
            return i;
        }
    }
    return -1;
}

int strlen(char message[]){
    int len;
    for (int i = 0; message[i] != '\0'; i++){
        len = i;
    }
    return len+1;
}

void encryptAffine(char message[], char encryptedMessage[], int a, int b) {
    for (int i = 0; message[i] != '\0'; ++i) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            encryptedMessage[i] = (char)(((a * (message[i] - 'A') + b) % 26) + 'A');
        } 
        else if (message[i] >= 'a' && message[i] <= 'z') {
            encryptedMessage[i] = (char)(((a * (message[i] - 'a') + b) % 26) + 'a');
        }
    }
    encryptedMessage[strlen(message)] = '\0';
}

void decryptAffine(char encryptedMessage[], char decryptedMessage[], int a, int b) {
    int aInverse = modInverse(a);
    if (aInverse == -1) {
        printf("Invalid key 'a'. Inverse doesn't exist.\n");
        return;
    }
    for (int i = 0; encryptedMessage[i] != '\0'; ++i) {
        if (encryptedMessage[i] >= 'A' && encryptedMessage[i] <= 'Z') {
            decryptedMessage[i] = (char)(((aInverse * (encryptedMessage[i] - 'A' - b + 26)) % 26) + 'A');
        }
        else if (encryptedMessage[i] >= 'a' && encryptedMessage[i] <= 'z') {
            decryptedMessage[i] = (char)(((aInverse * (encryptedMessage[i] - 'a' - b + 26)) % 26) + 'a');
        }
    }
    decryptedMessage[strlen(encryptedMessage)] = '\0';
}

int main() {
    char plaintext[100];
    char encryptedText[100];
    char decryptedText[100];
    int a, b;

    printf("Enter the message to be encrypted: ");
    scanf("%s",plaintext);

    printf("Enter the key a: ");
    scanf("%d", &a);
    if (!isCoprime(a)) {
        printf("Invalid key a. It must be coprime with 26.\n");
        return 1;
    }

    printf("Enter the key b: ");
    scanf("%d", &b);

    encryptAffine(plaintext, encryptedText, a, b);
    printf("Encrypted message: %s\n", encryptedText);

    decryptAffine(encryptedText, decryptedText, a, b);
    printf("Decrypted message: %s\n", decryptedText);

    return 0;
}