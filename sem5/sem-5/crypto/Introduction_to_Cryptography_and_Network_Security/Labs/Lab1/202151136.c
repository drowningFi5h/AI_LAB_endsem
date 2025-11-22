////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// PACKAGES REQUIRED ///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_CHARS 30
#define ROWS 6
#define COLS 5

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// DECLARING GLOBAL VARIABLES - THE PLAYFAIR MATRIX AND THE CHARACTER SET USED IN THE PROBLEM /////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Define the size of the custom character set
#define CHARSET_SIZE  30

// Custom character set with '.', ',', '?', ';'
const char characterSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,?;";

// Global variable to store the Playfair cipher matrix
char matrix[ROWS][COLS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// PLAYFAIR CIPHER REQUIRES EVEN LENGTH PLAINTEXT, AND THE PLAINTEXT ALSO SHOULDNT HAVE  //////////////
//////// CONSECUTIVE CHARACTERS THAT ARE SIMILAR. TO FIX ODD LENGTH PLAINTEXT, A DELIMETER     //////////////
//////// 'X' HAS BEEN ADDED AT THE END, AND THE SAME DELIMETER HAS ALSO BEEN PLACED BETWEEN    //////////////
//////// CHARACTERS THAT ARE CONSECUTIVE AND SIMILAR. SO PREPROCESSING IS NEEDED (STARTS HERE) //////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Function to preprocess the plaintext for encryption
void preprocess(char *text) {
    int len = strlen(text);
    // Convert all characters to uppercase
    for (int i = 0; i < len; i++) {
        text[i] = toupper(text[i]);
    }
    // Remove any spaces
    for (int i = 0; i < len; i++) {
        if (text[i] == ' ') {
            strcpy(&text[i], &text[i + 1]);
            len--;
            i--;
        }
    }
    // If there are consecutive identical characters, add an 'X' between them
    for (int i = 0; i < len - 1; i++) {
        if (text[i] == text[i + 1]) {
            // Shift characters to the right to make space for 'X'
            for (int j = len; j > i + 1; j--) {
                text[j] = text[j - 1];
            }
            text[i + 1] = 'X';
            len++; // Increase the length of the string
            i++; // Skip the next character since it's already processed
        }
    }

    // Adjusting the length if it's odd and no 'X' has been added yet, add an 'X' at the end
    len--;
    if (len % 2 != 0 && text[len - 1] != 'X') {
        text[len] = 'X';
        text[len + 1] = '\0';
    }
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// PLAYFAIR CIPHER MATRIX CONTAINS ALL THE ELEMENTS THAT ARE IN OUR SET. ACCORDING TO THE     /////////
//////// RULES, UNIQUE CHARACTERS FROM THE SECRET KEY TAKEN AS INPUT ARE FILLED IN THE MATRIX       /////////
//////// MAINTAINING STRICT ORDER OF OCCURENCE. ONCE EXHAUSTED, THE REMAINING PLACES ARE OCCUPIED   /////////
//////// BY THE CHARACETRS FROM THE SET FOLLOWING STRICT ORDER OF OCCURENCE. MATRIX FORMATION       /////////
//////// STARTS HERE, ALONG WITH ALGORITHM FOR ENCRYPTION AND DECRYPTION.                           /////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Function to check if a character is already present in the matrix
bool isCharPresent(char matrix[ROWS][COLS], char ch) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] == ch) {
                return true;
            }
        }
    }
    return false;
}

// Function to fill the Playfair cipher matrix with characters from the key
void fillMatrix(char* characters){
    int charIndex = 0;
    for (int i = 0; characters[i] != '\0'; i++) {
        if (!isCharPresent(matrix, characters[i])) {
            matrix[charIndex / COLS][charIndex % COLS] = characters[i];
            charIndex++;
        }
    }

    // Fill the remaining characters from the set
    char currentChar = 'A';
    while (charIndex < MAX_CHARS) {
        if (!isCharPresent(matrix, currentChar) && currentChar != 'Z' + 1) {
            matrix[charIndex / COLS][charIndex % COLS] = currentChar;
            charIndex++;
        }
        if (currentChar == 'Z') {
            currentChar = '.';
        } else if (currentChar == '.') {
            currentChar = ',';
        } else if (currentChar == ',') {
            currentChar = '?';
        } else if (currentChar == '?') {
            currentChar = ';';
        } else {
            currentChar++;
        }
    }
}

// Function to print the Playfair cipher matrix
void printMatrix() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Function to find the row and column of a character in the matrix
void findPosition(char matrix[ROWS][COLS], char ch, int *row, int *col) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
    *row = -1; // Character not found
    *col = -1;
}

// Function to encrypt a plaintext character pair
void encryptPair(char matrix[ROWS][COLS], char *plainPair, char *cipherPair) {
    int row1, col1, row2, col2;
    findPosition(matrix, plainPair[0], &row1, &col1);
    findPosition(matrix, plainPair[1], &row2, &col2);

    // If characters are in the same row, replace them with the characters to their right
    if (row1 == row2) {
        cipherPair[0] = matrix[row1][(col1 + 1) % COLS];
        cipherPair[1] = matrix[row2][(col2 + 1) % COLS];
    }
    // If characters are in the same column, replace them with the characters below them
    else if (col1 == col2) {
        cipherPair[0] = matrix[(row1 + 1) % ROWS][col1];
        cipherPair[1] = matrix[(row2 + 1) % ROWS][col2];
    }
    // If characters form a rectangle, replace them with the characters on the same row but at opposite corners of the rectangle
    else {
        cipherPair[0] = matrix[row1][col2];
        cipherPair[1] = matrix[row2][col1];
    }
}

// Function to decrypt a ciphertext character pair
void decryptPair(char matrix[ROWS][COLS], char *cipherPair, char *plainPair) {
    int row1, col1, row2, col2;
    findPosition(matrix, cipherPair[0], &row1, &col1);
    findPosition(matrix, cipherPair[1], &row2, &col2);

    // If characters are in the same row, replace them with the characters to their left
    if (row1 == row2) {
        plainPair[0] = matrix[row1][(col1 + COLS - 1) % COLS];
        plainPair[1] = matrix[row2][(col2 + COLS - 1) % COLS];
    }
    // If characters are in the same column, replace them with the characters above them
    else if (col1 == col2) {
        plainPair[0] = matrix[(row1 + ROWS - 1) % ROWS][col1];
        plainPair[1] = matrix[(row2 + ROWS - 1) % ROWS][col2];
    }
    // If characters form a rectangle, replace them with the characters on the same row but at opposite corners of the rectangle
    else {
        plainPair[0] = matrix[row1][col2];
        plainPair[1] = matrix[row2][col1];
    }
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// IN AFFINE CIPHER, AN ALPHABET IS MAPPED TO ITS NUMERIC EQUIVALENT, ENCRYPTED USING A       /////////
//////// SIMPLE MATHEMATICAL FUNCTION, AND THEN CONVERTED BACK TO A LETTER. T EMPLOYS THE FORMULA   /////////
//////// E(X)=(aX+b) mod m FOR ENCRYPTION (a=11,b=15,m=30 (30 elements)) AND D(X)= a^-1(X-b) mod m  /////////
//////// FINDING a^-1 ISNT ALWAYS EASY, IT CAN BE DONE USING EXTENDED EUCLIDEAN ALGORITHM. THIS     /////////
//////// ALGORITHM, ALONG WITH ALGORITHM FOR ENCRYPTION AND DECRYPTION OF AFFINE CIPHER HAS BEEN    /////////
//////// IMPLEMENTED HERE                                                                           /////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Function to find the index of a character in the character set
int charsetIndex(char c) {
    int index;
    for (index = 0; index < CHARSET_SIZE; index++) {
        if (characterSet[index] == c) {
            return index;
        }
    }
    return -1; // Character not found
}

// Function to calculate the greatest common divisor (GCD) of two numbers
int gcd(int a, int m) {
    if (a == 0)
        return m;
    return gcd(m % a, a);
}

// Function to find the modular multiplicative inverse of a under modulo m
int modInverse(int a, int m) {
    int m0 = m;
    int y = 0, x = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        int q = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0)
        x += m0;
    return x;
}

// Encrypt a message using Affine cipher
char* encryptAffine(char* msg, int a, int b) {
    int length = strlen(msg);
    char* cipher = malloc(length + 1); // Allocate memory for the ciphertext
    for (int i = 0; i < length; i++) {
        int cIndex = charsetIndex(msg[i]);
        if (cIndex >= 0) {
            // Apply the Affine cipher formula: E(x) = (ax + b) mod m
            int eIndex = (a * cIndex + b) % CHARSET_SIZE;
            cipher[i] = characterSet[eIndex];
        } else {
            cipher[i] = msg[i]; // Non-alphabet characters remain unchanged
        }
    }
    cipher[length] = '\0'; // Null-terminate the string
    return cipher;
}

// Decrypt a message using Affine cipher
char* decryptAffine(char* cipher, int a, int b) {
    int length = strlen(cipher);
    char* msg = malloc(length + 1); // Allocate memory for the decrypted message
    int aInv = modInverse(a, CHARSET_SIZE); // Calculate the modular multiplicative inverse of 'a'
    for (int i = 0; i < length; i++) {
        int cIndex = charsetIndex(cipher[i]);
        if (cIndex >= 0) {
            // Apply the Affine cipher decryption formula: D(x) = a^-1(x - b) mod m
            int dIndex = (aInv * (cIndex - b + CHARSET_SIZE)) % CHARSET_SIZE;
            msg[i] = characterSet[dIndex];
        } else {
            msg[i] = cipher[i]; // Non-alphabet characters remain unchanged
        }
    }
    msg[length] = '\0'; // Null-terminate the string
    return msg;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// THE SHIFT CIPHER IS A SIMPLE SUBSTITUTION CIPHER WHERE EACH LETTER IN THE PLAINTEXT IS     /////////
//////// SHIFTED A CERTAIN NUMBER OF POSITIONS DOWN OR UP THE ALPHABET. IT USES THE FORMULA         /////////
//////// E(x)=(x+k) mod 30 FOR ENCRYPTION AND D(X)= (x-k) mod 30 FOR DECRYPTION. WHERE k IS THE     /////////
//////// NUMBER OF POSITIONS TO SHIFT. IN MY CODE, I HAVE USED CAESAR CIPHER WITH k = 3.            /////////
//////// ALGORITHM FOR ENCRYPTION AND DECRYPTION OF CAESAR CIPHER HAS BEEN IMPLEMENTED HERE.        /////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////





// Function to shift a character by n positions within the character set
char shiftCharacter(char c, int n) {
    int index = charsetIndex(c);
    if (index != -1) {
        // Shift the character by n positions, wrapping around if necessary
        index = (index + n) % CHARSET_SIZE;
        return characterSet[index];
    }
    return c; // Return the character unchanged if it's not in the character set
}

// Function to encrypt a message using Caesar cipher
void encryptCaesar(char* msg, int shift, char* cipher) {
    for (int i = 0; i < strlen(msg); ++i) {
        // Shift each character in the message using the shiftCharacter function
        cipher[i] = shiftCharacter(msg[i], shift);
    }
    cipher[strlen(msg)] = '\0'; // Null-terminate the string
}

// Function to decrypt a message using Caesar cipher
void decryptCaesar(char* cipher, int shift, char* msg) {
    for (int i = 0; i < strlen(cipher); ++i) {
        // Decrypt each character in the ciphertext using the shiftCharacter function
        msg[i] = shiftCharacter(cipher[i], CHARSET_SIZE - shift);
    }
    msg[strlen(cipher)] = '\0'; // Null-terminate the string
}



////// MAIN STARTS HERE ///////////////////////////////////////////////////////////////////////////////////



int main() {
    char plaintext[100];
    char ciphertext_playfair[100];
    char decrypted_playfair[100];

    // Input plaintext
    printf("Enter plaintext P: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    preprocess(plaintext);
    printf("Plaintext Δ after processing: %s\n", plaintext);

    char characters[MAX_CHARS + 1];
    // Input key for Playfair cipher
    printf("Enter a string formed from the character set (A-Z, ,, ., ?, ;): ");
    scanf("%s", characters);

    // Fill the Playfair matrix
    fillMatrix(characters);
    printf("Playfair Cipher Matrix: \n");
    printMatrix();

    // Encrypting the plaintext using Playfair cipher
    for (int i = 0; i < strlen(plaintext); i += 2) {
        encryptPair(matrix, &plaintext[i], &ciphertext_playfair[i]);
    }
    ciphertext_playfair[strlen(plaintext)] = '\0';
    printf("Encrypted Cipher text C1 (using Playfair Cipher Encryption on Plaintext P): %s\n", ciphertext_playfair);

    int a = 11, b = 15; // Affine cipher keys

    // Ensure 'a' and CHARSET_SIZE are coprime for Affine cipher
    if (gcd(a, CHARSET_SIZE) != 1) {
        printf("Keys are not valid for Affine cipher.\n");
        return 1;
    }

    // Encrypting the Playfair ciphertext using Affine cipher
    char* cipher_affine = encryptAffine(ciphertext_playfair, a, b);
    printf("Encrypted Cipher text C2 (after applying Affine Cipher Encryption on C1): %s\n", cipher_affine);

    int shift;
    printf("Enter key for Shift Cipher: ");
    scanf("%d", &shift);
    
    if (shift>=0 && shift<=29){
        char cipher_shift[strlen(cipher_affine) + 1];
        char decryptedMsg_shift[strlen(cipher_affine) + 1];

        // Encrypting the Affine ciphertext using Caesar cipher
        encryptCaesar(cipher_affine, shift, cipher_shift);
        printf("\nEncrypted Cipher text C3 (after applying Shift Cipher Encryption on C2): %s\n", cipher_shift);
        
        
        printf("\n\n");
        // Encryption was done in the order: Playfair -> Affine -> Shift
        // Therefore Decryption will be done in the opposite order: Shift -> Affine -> Playfair
        
        
        

        // Decrypting the Caesar ciphertext
        decryptCaesar(cipher_shift, shift, decryptedMsg_shift);
        printf("Decrypted cipher text C2 on applying Shift Cipher Decryption function to C3: %s\n", decryptedMsg_shift);

        // Decrypting the Affine ciphertext
        char* decryptedMsg_affine = decryptAffine(decryptedMsg_shift, a, b);
        printf("Decrypted cipher text C1 on applying Affine Cipher Decryption function to C2: %s\n", decryptedMsg_affine);

        // Decrypting the Playfair ciphertext
        for (int i = 0; i < strlen(decryptedMsg_affine); i += 2) {
            decryptPair(matrix, &decryptedMsg_affine[i], &decrypted_playfair[i]);
        }
        decrypted_playfair[strlen(ciphertext_playfair)] = '\0';
        printf("Plaintext Δ  on applying Playfair Cipher Decryption function to C1: %s\n", decrypted_playfair);

        for (int i=0; i<strlen(plaintext); i++){
            if(plaintext[i] != decrypted_playfair[i]){
                printf("%c", plaintext[i+1]);
                printf("The processed plaintext and the decrypted text are not similar, so the process was unsuccessfull.\n");
                break;
            }
            else if(i==strlen(plaintext)-1){
                printf("The processed plaintext and the decrypted text are similar, so the process was successfull.\n");
            
            }
        }
        if(strcmp(plaintext, decrypted_playfair)==0){
            printf("The processed plaintext and the decrypted text are similar, so the process was successfull.\n");
        }
        else {
            printf("Something went wrong!\n");
        }
    }
    else{
        printf("The value should be between 0 and 29 (both inclusive).\n");
    }
    

    return 0;
}