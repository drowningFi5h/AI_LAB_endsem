/*
Name: Sanidhya Kumar
Roll No: 202151138
Lab Assignment 01
Date: 11th Feb 2024
*/

#include <stdio.h>
#include <string.h>

//Function to adjust the length of the plaintext and handle the repetition of letter (if present) according to the rule of the Playfair cipher. Here I, J are not considered as same.
void adjustPlaintext(char plaintext[], char adjustedPlaintext[]){
    int n = strlen(plaintext);
    int j = 0;
    //Add an 'X' after every repeated letter
    for (int i=0; i<n-1; i++){
        if (plaintext[i] == plaintext[i+1]){
            adjustedPlaintext[j++] = plaintext[i];
            adjustedPlaintext[j++] = 'X';
        }
        else{
            adjustedPlaintext[j++] = plaintext[i];
        }
    }
    adjustedPlaintext[j++] = plaintext[n-1];
    //Add an 'X' if the length of the adjustedPlaintext is odd
    if (j%2 != 0){
        adjustedPlaintext[j++] = 'X';
    }
    //Add a null character at the end of the string to mark the end
    adjustedPlaintext[j]='\0';
}

//Function to check if the plaintext entered is valid or not using the validset[] array. This function would be used to check plaintext and k1. If the plaintext or k1 is not valid then print an error message and exit the program.
int checkValidity(char plaintext[], char validset[]){
    for (int i=0; i<strlen(plaintext); i++){
        int valid = 0;
        for (int j=0; j<strlen(validset); j++){
            if (plaintext[i] == validset[j]){
                valid = 1;
                break;
            }
        }
        if (valid == 0){
            return 0;
        }
    }
    return 1;
}

//Check if k3 is between 0 and 29 or not.
int checkk3(int k3){
    if (k3>=0 && k3<=29){
        return 1;
    }
    else{
        return 0;
    }
}

//Check if the character c is present in the matrix or not. If not present then return 1 else return 0.
int checkInMatrix(char matrix[6][5], char c){
    for (int i=0; i<6; i++){
        for (int j=0; j<5; j++){
            if (matrix[i][j] == c){
                return 0;
            }
        }
    }
    return 1;
}

//Print the Playfair matrix generated from the inputted keyword.
void printPlayfairMatrix(char playfairMatrix[6][5]){
    for (int i=0; i<6; i++){
        for (int j=0; j<5; j++){
            printf("%c ", playfairMatrix[i][j]);
        }
        printf("\n");
    }
}

//Generate the 6 × 5 key matrix similar to the Playfair cipher from k1.
void generatePlayfairMatrix(char k1[], char playfairMatrix[6][5], char validset[]){
    int n = strlen(k1);
    int k = 0;
    int r = 0;
    int c = 0;
    //Emptying the matrix first
    for (int i=0; i<6; i++){
        for (int j=0; j<5; j++){
            playfairMatrix[i][j]=0;
        }
    }
    //Filling elements from the key k1 one by one, taking care of repetition.
    for (int i=0; i<n; i++){
        if(checkInMatrix(playfairMatrix, k1[i])){
            playfairMatrix[r][c] = k1[i];
            c++;
            //Add to new row if the current row is full
            if(c==5){
                c=0;
                r++;
            }
        }
    }
    //Filling the remaining elements from the validset[] array.
    for (int i=0; i<strlen(validset); i++){
        if (checkInMatrix(playfairMatrix, validset[i])){
            playfairMatrix[r][c] = validset[i];
            c++;
            //Add to new row if the current row is full
            if(c==5){
                c=0;
                r++;
            }
        }
    }
}

//Find position of a character in the Playfair matrix and return the row and column of the character.
void findPosition(char playfairMatrix[6][5], char element, int *r, int *c){
    for (int i=0; i<6; i++){
        for (int j=0; j<5; j++){
            if (playfairMatrix[i][j] == element){
                *r = i;
                *c = j;
                return;
            }
        }
    }
}

//Encrypt adjustedPlaintext (∆) using the rule of the Playfair cipher and store the result in ciphertext1.
void encryptPlayfair(char adjustedPlaintext[], char playfairMatrix[6][5], char ciphertext1[100]){
    int n = strlen(adjustedPlaintext);
    //Work in blocks of 2
    for (int i=0; i<n; i+=2){
        int r1, c1, r2, c2;
        //Find position of the two characters in the matrix
        findPosition(playfairMatrix, adjustedPlaintext[i], &r1, &c1);
        findPosition(playfairMatrix, adjustedPlaintext[i+1], &r2, &c2);

        //If the two characters are in the same row, replace them with the characters to their right, keeping in mind row wrapping
        if (r1 == r2){
            c1 = (c1+1)%5;
            c2 = (c2+1)%5;
        }
        //If the two characters are in the same column, replace them with the characters below them, keeping in mind column wrapping
        else if (c1 == c2){
            r1 = (r1+1)%6;
            r2 = (r2+1)%6;
        }
        //If the two characters are in different rows and columns, replace them with the characters in the same row but at the other end of the rectangle defined by the original pair of characters
        else{
            int temp = c1;
            c1 = c2;
            c2 = temp;
        }
        //Store the result in ciphertext1
        ciphertext1[i] = playfairMatrix[r1][c1];
        ciphertext1[i+1] = playfairMatrix[r2][c2];
    }
    //Add a null character at the end of the string to mark the end
    ciphertext1[n]='\0';
}

//Encrypt ciphertext1 using the Affine cipher where the key is in k2 array and store the result in ciphertext2.
void encryptAffine(char ciphertext1[], int k2[], char ciphertext2[], char validset[]) {
    int n = strlen(ciphertext1);
    //Loop through each character in ciphertext1
    for (int i=0; i<n; i++){
        //Find the index of each character in the validset[] array and then apply the affine transformation to it
        int index;
        for (int j=0; j<strlen(validset); j++){
            if (ciphertext1[i] == validset[j]){
                index = j;
                break;
            }
        }
        //Applying affine transformation according to the given question
        ciphertext2[i]=validset[(k2[0]*index+k2[1])%30];
    }
    //Add a null character at the end of the string to mark the end
    ciphertext2[n]='\0';
}

//Encrypt ciphertext2 using the Shift cipher where the key is k3 and store the result in ciphertext3.
void encryptShift(char ciphertext2[], char ciphertext3[], int k3, char validset[]){
    int n = strlen(ciphertext2);
    //Loop through each character in ciphertext2
    for (int i=0; i<n; i++){
        //Find the index of each character in the validset[] array and then apply the shift transformation to it
        int index;
        for (int j=0; j<strlen(validset); j++){
            if (ciphertext2[i] == validset[j]){
                index = j;
                break;
            }
        }
        //Applying shift transformation according to the given question
        ciphertext3[i] = validset[(index+k3)%30];
    }
    //Add a null character at the end of the string to mark the end
    ciphertext3[n]='\0';
}

//Decrypt ciphertext3 using the inverse of shift cipher where the key is k3 and store the result in decryptedCiphertext1.
void decryptShift(char ciphertext3[], char decryptedCiphertext1[], int k3, char validset[]){
    int n = strlen(ciphertext3);
    //Loop through each character in ciphertext3
    for (int i=0; i<n; i++){
        //Find the index of each character in the validset[] array and then apply the inverse of shift transformation to it
        int index;
        for (int j=0; j<strlen(validset); j++){
            if (ciphertext3[i] == validset[j]){
                index = j;
                break;
            }
        }
        //Applying inverse of shift transformation according to the given question
        decryptedCiphertext1[i] = validset[(index-k3+30)%30];
    }
    //Add a null character at the end of the string to mark the end
    decryptedCiphertext1[n]='\0';
}

//Decrypt decryptedCiphertext1 using the inverse of the Affine cipher where the key is k2 and store the result in decryptedCiphertext2.
void decryptAffine(char decryptedCiphertext1[], int k2[], char decryptedCiphertext2[], char validset[]){
    int n = strlen(decryptedCiphertext1);
    //Loop through each character in decryptedCiphertext1
    for (int i=0; i<n; i++){
        //Find the index of each character in the validset[] array and then apply the inverse of affine transformation to it
        int index;
        for (int j=0; j<strlen(validset); j++){
            if (decryptedCiphertext1[i] == validset[j]){
                index = j;
                break;
            }
        }
        //Find a_inverse
        int a_inverse = 0;
        for (int j=1; j<30; j++){
            if ((k2[0]*j)%30 == 1){
                a_inverse = j;
                break;
            }
        }
        //Applying the inverse of affine transformation according to the given question
        decryptedCiphertext2[i] = validset[(a_inverse*(index-k2[1]+30))%30];
    }
    //Add a null character at the end of the string to mark the end
    decryptedCiphertext2[n]='\0';
}

//Decrypt decryptedCiphertext2 using the inverse of the Playfair cipher where the key is k1 and store the result in decryptedCiphertext3.
void decryptPlayfair(char decryptedCiphertext2[], char playfairMatrix[6][5], char decryptedCiphertext3[]){
    int n = strlen(decryptedCiphertext2);
    //Work in blocks of 2
    for (int i=0; i<n; i+=2){
        int r1, c1, r2, c2;
        //Find position of the two characters in the matrix
        findPosition(playfairMatrix, decryptedCiphertext2[i], &r1, &c1);
        findPosition(playfairMatrix, decryptedCiphertext2[i+1], &r2, &c2);

        //If the two characters are in the same row, replace them with the characters to their right, keeping in mind row wrapping
        if (r1 == r2){
            c1 = (c1-1+5)%5;
            c2 = (c2-1+5)%5;
        }
        //If the two characters are in the same column, replace them with the characters below them, keeping in mind column wrapping
        else if (c1 == c2){
            r1 = (r1-1+6)%6;
            r2 = (r2-1+6)%6;
        }
        //If the two characters are in different rows and columns, replace them with the characters in the same row but at the other end of the rectangle defined by the original pair of characters
        else{
            int temp = c1;
            c1 = c2;
            c2 = temp;
        }
        //Store the result in decryptedCiphertext3
        decryptedCiphertext3[i] = playfairMatrix[r1][c1];
        decryptedCiphertext3[i+1] = playfairMatrix[r2][c2];
    }
    //Add a null character at the end of the string to mark the end
    decryptedCiphertext3[n]='\0';
}

/*----------------------------- MAIN FUNCTION -----------------------------*/

int main(){

    printf("-------------------------------------\n\n");
    printf("Welcome to the Cryptography Lab Assignment 01\nMade by Sanidhya Kumar (202151138)\n\n");
    printf("-------------------------------------\n\n");

    /*------------------------- Defining Variables -------------------------*/

    //Define all the needed variables
    char plaintext[100];
    char adjustedPlaintext[100];

    char k1[100];
    int k3;
    char playfairMatrix[6][5];

    char ciphertext1[100];
    char ciphertext2[100];
    char ciphertext3[100];

    char decryptedCiphertext1[100];
    char decryptedCiphertext2[100];
    char decryptedCiphertext3[100];

    //Set of valid characters: The Plaintext space P, Key space K and Ciphertext Space C
    char validset[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ/\\[]";

    /*------------------------- Encryption Process -------------------------*/

    //Input a word (without any space) as a plaintext, where every element of the plaintext is from P.
    printf("Enter the Plaintext: ");
    scanf("%s", &plaintext);

    //Check if the plaintext entered is valid or not. If it's not valid then print an error message and exit the program.
    if (!checkValidity(plaintext, validset)){
        printf("\nWRONG INPUT - Invalid plaintext entered! Only valid characters are A-Z along with ,.?;\n\n");
        printf("-------------------------------------\n\n");
        return 0;
    };

    //Adjust the length of the plaintext and handle the repetition of letter (if present) according to the rule of the Playfair cipher. Here I, J are not considered as same. Let the final word be adjustedPlaintext (∆). Print the word ∆.
    adjustPlaintext(plaintext, adjustedPlaintext);
    printf("The adjusted plaintext is: %s\n\n", adjustedPlaintext);

    //Consider a word (without any space) as input and that word is the first key k1, where every element of k1 is from K.
    printf("Enter key k1: ");
    scanf("%s", &k1);

    //Check if the key k1 entered is valid or not. If it's not valid then print an error message and exit the program.
    if (!checkValidity(k1, validset)){
        printf("\nWRONG INPUT - Invalid k1 entered! Only valid characters are A-Z along with ,.?;\n\n");
        printf("-------------------------------------\n\n");

        return 0;
    }

    //Generate the 6 × 5 key matrix similar to the Playfair cipher from k1 and print it.
    generatePlayfairMatrix(k1, playfairMatrix, validset);
    printf("The Playfair Matrix generated from the key k1 is: \n");
    printPlayfairMatrix(playfairMatrix);

    //Encrypt adjustedPlaintext (∆) using the rule of the Playfair cipher where the key is k1. Let the ciphertext be ciphertext1.
    encryptPlayfair(adjustedPlaintext, playfairMatrix, ciphertext1);
    printf("Encrypted message using Playfair Cipher: %s \n\n", ciphertext1);

    //Encrypt ciphertext1 using the Affine cipher where the key is k2 = (11, 15) ∈ Z30 × Z30. Let the ciphertext be ciphertext2.
    printf("k2 is already fixed to (11, 15)\n");
    int k2[2] = {11, 15};
    encryptAffine(ciphertext1, k2, ciphertext2, validset);
    printf("Encrypted message using Affine Cipher: %s \n\n", ciphertext2);

    // Encrypt ciphertext2 using the Shift cipher where the key is k3 (0 ≤ k3 ≤ 29). Let the ciphertext be ciphertext3.
    printf("Enter key k3: ");
    scanf("%d", &k3);

    //Check if the key k3 entered is valid or not. If it's not valid then print an error message and exit the program.
    if(!checkk3(k3)){
        printf("\nWRONG INPUT - Invalid k3 entered! Only valid k3 is between 0 and 29\n\n");
        printf("-------------------------------------\n\n");
        return 0;
    }

    encryptShift(ciphertext2, ciphertext3, k3, validset);
    printf("The final encrypted message using Shift Cipher: %s \n\n", ciphertext3);

    /*------------------------- Decryption Process -------------------------*/
    
    printf("-------------------------------------\n\n");

    //Decrypt ciphertext3 using the Shift cipher where the key is k3. Let the decrypted ciphertext be decryptedCiphertext1.
    decryptShift(ciphertext3, decryptedCiphertext1, k3, validset);
    printf("Decrypted message using Shift Cipher: %s \n", decryptedCiphertext1);

    //Decrypt decryptedCiphertext1 using the Affine cipher where the key is k2. Let the decrypted ciphertext be decryptedCiphertext2.
    decryptAffine(decryptedCiphertext1, k2, decryptedCiphertext2, validset);
    printf("Decrypted message using Affine Cipher: %s \n", decryptedCiphertext2);

    //Decrypt decryptedCiphertext2 using the Playfair cipher where the key is k1. Let the decrypted ciphertext be decryptedCiphertext3.
    decryptPlayfair(decryptedCiphertext2, playfairMatrix, decryptedCiphertext3);
    printf("Decrypted message using Playfair Cipher: %s \n", decryptedCiphertext3);

    /*------------------------- Checking our Process -------------------------*/

    printf("\n-------------------------------------\n\n");

    //Check if the decrypted message is same as the original message (∆) or not.
    if (strcmp(adjustedPlaintext, decryptedCiphertext3) == 0){
        printf("Yayy! The decrypted message is same as the adjustedPlaintext (delta)\n\n");
    }
    else{
        printf("OOPS! The decrypted message is not same as the adjustedPlaintext (delta)\n\n");
    }
    printf("-------------------------------------\n\n");

    /*------------------------- END -------------------------*/

    return 0;
}