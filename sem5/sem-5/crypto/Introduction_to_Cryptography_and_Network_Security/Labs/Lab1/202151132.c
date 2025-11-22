//*************************************
// Name: Riya
// Roll No: 202151132
// Lab Assignment: 1
//*************************************


#include<stdio.h>
#include<string.h>
#include<ctype.h>

//function to handle the plaintext if it is odd length or contains repeating letters
char* handle_Plaintext(char plaintext[],char new_plaintext[100]){ 
  int n=strlen(plaintext);
  int j=0;

  //first we will check if there are any repeating letters in the plaintext
  //for every repeating letter, we will add 'X' after one of the letters

  //we will also convert all the letters to uppercase as it is required for playfair cipher
  for(int i=0;i<n-1;i++){   //loop till n-2 so we can get i and i+1
    if(plaintext[i]==plaintext[i+1]){
        new_plaintext[j++] = toupper(plaintext[i]);
        new_plaintext[j++] = 'X';   
    }
    else{
      new_plaintext[j++]=toupper(plaintext[i]);
    }

  }
  new_plaintext[j++] = toupper(plaintext[n - 1]); //last letter (as i<=n-2 in the loop)

  //Now, we will check if after handling the repeating letters, the length of the plaintext
  //is odd.If it is odd, we will add 'X' at the end of the plaintext
    if(j%2!=0){
        new_plaintext[j++]='X';
    }

        new_plaintext[j]='\0';  //adding null character at the end of the new_plaintext array
    
return new_plaintext;  //returning the adjusted new plaintext
}


//function to generate playfair key matrix
//Acording to the question, here the matrix will be 6x5 instead of 5x5 as we are not treating
//i and j as same letter and also,there are 30 letters as specified in the question.

void create_playfair_matrix(char K1[],char playfair_matrix[6][5]){

    //these are the given letters that will we used in the matrix (Given in the question)
    char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    int n=strlen(K1);

    //firstly,we will add the letters of the secret key to the matrix.If any letter is repeating \
    //we will skip it.Then, we will fill the remaining matrix with the remaining letters
    //in the letters array(repeating letters will be skipped)

   int row=0;int col=0;  //row and column index of matrix

   for(int i=0;i<n;i++){
    int already_present=0;  //variable to check if any letter is already present(repeating)
    for(int j=0;j<i;j++){
        if(toupper(K1[i])==toupper(K1[j])){ //toupper because it is needed in playfair cipher
            already_present=1;
            break;
        }
    }

    //if the letter is not already present, we will add it to the matrix and
    //increment the column idx as we are filling row wise
    if(already_present==0){
        playfair_matrix[row][col++]=toupper(K1[i]); 
    }
    if(row==6 && col==5) {  //if the matrixis completely filled,we are done and we will break the loop
        break;
    } 
    else if (col == 5) { //after filling one row,we will move to the next row
        row++;
        col = 0;
    }
    
   }

    //Now, we will the remaining matrix with the remaining letters(not already present) from the letters array
    //we will code it similar to the above code for filling the secret key letters into the matrix

    for(int i=0;i<30;i++){  //traversing the letters array
        int already_present=0;
        for(int j=0;j<n;j++){
            if(toupper(letters[i])==toupper(K1[j])){
                already_present=1;
                break;
            }
        }

        //if the letter is not already present, we will add it to the matrix and
    //increment the column idx as we are filling row wise
        if(already_present==0){
            playfair_matrix[row][col++]=toupper(letters[i]);
        }
        if(row==6 && col==5) { //if the matrixis completely filled,we are done and we will break the loop
            break;
        } else if (col == 5) { //after filling one row,we will move to the next row
            row++;
            col = 0;
        }
    }

}

//function for encryption of plaintext using playfair cipher
//We will take pairs of letters from the adjusted plaintext and then encrypt them using the playfair matrix

char* encryption_playfair(char new_plaintext[],char playfair_matrix[6][5],char C1[]){
    int n=strlen(new_plaintext);
    for(int i=0;i<n;i+=2){

        //for each pair of letters in the plaintext, we will find the row and column index of the
        //letters in the playfair matrix into the varibles row1,col1 and row2,col2 respectively
        int row1,col1,row2,col2;  

        for(int j=0;j<6;j++){  //traversing the playfair key matrix
            for(int k=0;k<5;k++){
                //if the first letter is found,we will store in index
                if(playfair_matrix[j][k]==new_plaintext[i]){ 
                    row1=j;
                    col1=k;
                }
                //if the second letter is found,we will store in index
                if(playfair_matrix[j][k]==new_plaintext[i+1]){
                    row2=j;
                    col2=k;
                }
            }
        }

        //if both the letters(in the pair) are in the same row, we will take the next letter to the right
        //of each letter to get the encrypted letter
        if(row1==row2){
            C1[i]=playfair_matrix[row1][(col1+1)%5];
            C1[i+1]=playfair_matrix[row2][(col2+1)%5];
        }

        //if both the letters are in the same column, we will take the next letter in the bottom
        //for each letter to get the encrypted letter
        else if(col1==col2){
            C1[i]=playfair_matrix[(row1+1)%6][col1];
            C1[i+1]=playfair_matrix[(row2+1)%6][col2];
        }

        //if both row and column are different for the two letters,we will form a rectangle or square
        //with the two letters and take the opposite corners to get the encrypted letters
        else{
            C1[i]=playfair_matrix[row1][col2];
            C1[i+1]=playfair_matrix[row2][col1];
        }

    }
    C1[n]='\0'; //adding null character at the end of the C1 array
    return C1;

}

//function to encrypt C1 using affine cipher where the key is K2 = (11, 15) ∈ Z30 × Z30
char* encryption_affine(char C1[],int K2[],char C2[]){
        //these are the given letters that will we used (given in question)
        char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
        int n=strlen(C1);

        //C1 will act as plaintext for the affine cipher
        //for each letter in C1,we will first find the index of the letter in the letters array
        //then, we will apply the encryption formula for affine cipher to get the encrypted letter
        //then we will store the encrypted letter in the C2 array
        for(int i=0;i<n;i++){
            int idx; //index of C1[i] in the letters array
            for(int j=0;j<30;j++){
                if(C1[i]==letters[j]){
                    idx=j; //index found,so we will break the loop
                    break;
                }
            }

            //taking a and b from the key K2
            //In the question, a is given as 11, so gcd(a,30)=1.This condition is very important
            //as we will only be able to find the inverse(during decryption) if this holds.
            int a=K2[0];
            int b=K2[1];

            //applying the encryption formula for affine cipher which is:
            //E(x, k) = (ax + b) mod 30 where x is the index of the letter in the letters array
            //and k is the key value.
            C2[i]=letters[(a*idx+b)%30];
        }
        C2[n]='\0'; //adding null character at the end of the C2 array
        return C2;
}

//function to encrypt C2 using shift cipher where the key is K3 (0 ≤ K3 ≤ 29)
char* encryption_shift(char C2[],int K3,char C3[]){
    //these are the given letters that will we used (given in question)
    char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    int n=strlen(C2);

    //C2 will act as plaintext for the shift cipher.
    //For each letter in C2, we will first find the index of the letter in the letters array
    //then, we will apply the encryption formula for shift cipher to get the encrypted letter
    //then we will store the encrypted letter in the C3 array
    for(int i=0;i<n;i++){
        int idx; //index of C2[i] in the letters array
        for(int j=0;j<30;j++){
            if(C2[i]==letters[j]){
                idx=j; //index found,so we will break the loop
                break;
            }
        }

        //applying the encryption formula for shift cipher which is:
        //E(x, k) = (x + k) mod 30 where x is the index of the letter in the letters array
        //and k is the key value.
        C3[i]=letters[(idx+K3)%30];
    }
    C3[n]='\0'; //adding null character at the end of the C3 array
    return C3;
}

//function to decrypt C3 using shift cipher to get back C2
char* decryption_shift(char C3[],int K3,char C2[]){
    //these are the given letters that will we used (given in question)
    char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    int n=strlen(C3);

    //For each letter in C3,We will apply the decryption formula for shift cipher to get the decrypted letter
    //then we will store the decrypted letter in the C2 array
    //the logic will be same as we did in the encryption_shift function
    //the only difference is we are applying the decryption formula here
    for(int i=0;i<n;i++){
        int idx; //index of C3[i] in the letters array
        for(int j=0;j<30;j++){
            if(C3[i]==letters[j]){
                idx=j; //index found,so we will break the loop
                break;
            }
        }

        //applying the decryption formula for shift cipher which is:
        //D(x, k) = (x - k) mod 30 where x is the index of the letter in the letters array
        //and k is the key value.
        //we will use 30+idx here to avoid negative values and to get values between 0 and 29.
        C2[i]=letters[(30+idx-K3)%30];
    }
    C2[n]='\0'; //adding null character at the end of the C2 array
    return C2;
}

//function to decrypt C2 using affine cipher to get back C1
char* decryption_affine(char C2[],int K2[],char C1[]){
    //these are the given letters that will we used (given in question)
    char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    int n=strlen(C2);

    //For each letter in C2,We will apply the decryption formula for affine cipher to get the decrypted letter
    //then we will store the decrypted letter in the C1 array
    for(int i=0;i<n;i++){
        int idx; //index of C2[i] in the letters array
        for(int j=0;j<30;j++){
            if(C2[i]==letters[j]){
                idx=j; //index found,so we will
                break;
            }
        }

        //taking a and b from the key K2
        int a=K2[0];
        int b=K2[1];

        //applying the decryption formula for affine cipher which is:
        //D(x, k) = (a_inv * (x - b)) mod 30 where x is the index of the letter in the letters array
        //and k is the key value.
        //we will first find the inverse of a(multiplicative inverse) and then apply the formula.
        //the logic is same as we did in the encryption_affine function
        //the only difference is we are applying the decryption formula here

        //important point to note here is we are able to find the inverse of a because
        //as given in the question a=11, so gcd(a,30)=1.That's why we are able to find the inverse of a.

        int a_inv;  //for storing the inverse of a 
        for(int j=1;j<30;j++){
            if((a*j)%30==1){  //finding the multiplicative inverse of a
                a_inv=j; //inverse found,so we will break the loop
                break;
            }
        }
        //now we will apply the decryption formula as described above
        C1[i]=letters[(a_inv*(30+idx-b))%30];
    }
    C1[n]='\0'; //adding null character at the end of the C1 array
    return C1;
}

//function to decrypt C1 using playfair cipher to get back the plaintext
char* decryption_playfair(char C1[],char playfair_matrix[6][5],char secret_key[],char plaintext_decrypted[]){
    int n=strlen(C1);

    //for each pair of letters in the C1, we will find the row and column index of the
    //letters in the playfair matrix into the varibles row1,col1 and row2,col2 respectively
    //then we will apply the decryption formula for playfair cipher to get the decrypted letters
    //then we will store the decrypted letters in the plaintext_decrypted array

    for(int i=0;i<n;i+=2){  //traversing each pair of letters in C1
        int row1,col1,row2,col2;
        for(int j=0;j<6;j++){ //traversing the playfair key matrix
            for(int k=0;k<5;k++){
                if(playfair_matrix[j][k]==C1[i]){ //if the first letter is found,we will store the index
                    row1=j;
                    col1=k;
                }
                if(playfair_matrix[j][k]==C1[i+1]){ //if the second letter is found,we will store the index
                    row2=j;
                    col2=k;
                }
            }
        }

        //applying the decryption formula for playfair cipher
        //the logic will be same as we did in the encryption_playfair function
        //the only difference is we are applying the decryption formula here

        //if both the letters(in the pair) are in the same row, we will take the previous letter to the left
        //of each letter to get the decrypted letter
        //we will use 30+col1 to get the values between 0 to 4 and avoid negative values
        if(row1==row2){
            plaintext_decrypted[i]=playfair_matrix[row1][(30+col1-1)%5];
            plaintext_decrypted[i+1]=playfair_matrix[row2][(30+col2-1)%5];
        }

        //if both the letters are in the same column, we will take the previous letter in the top
        //for each letter to get the decrypted letter
        //we will use 30+row1 to get the values between 0 to 5 and avoid negative values
        else if(col1==col2){
            plaintext_decrypted[i]=playfair_matrix[(30+row1-1)%6][col1];
            plaintext_decrypted[i+1]=playfair_matrix[(30+row2-1)%6][col2];
        }

        //if both row and column are different for the two letters,we will form a rectangle or square
        //with the two letters and take the opposite corners to get the decrypted letters
        else{
            plaintext_decrypted[i]=playfair_matrix[row1][col2];
            plaintext_decrypted[i+1]=playfair_matrix[row2][col1];
        }

    }
    plaintext_decrypted[n]='\0'; //adding null character at the end of the plaintext_decrypted array
    return plaintext_decrypted;

}

int main(){

    //we will input the plaintext from the user
    char plaintext[100];
    printf("Enter the plaintext: ");
    scanf("%s",plaintext);

    //checking if every element of the plaintext is from letters array or not:
    char letters[30]="ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    int n=strlen(plaintext);
    for(int i=0;i<n;i++){
        int found=0;
        for(int j=0;j<30;j++){
            if(plaintext[i]==letters[j]){ //letter is found in the letters array
                found=1;
                break;
            }
        }
        if(found==0){
            printf("Invalid plaintext\n"); //letter is not found in the letters array
            return 0;
        }
    }

    //array for adjusted_plaintext according to playfair cipher
    char new_plaintext[100];
    printf("The adjusted plaintext is: %s\n",handle_Plaintext(plaintext,new_plaintext));

    //input the secret key(K1) for the playfair cipher from the user
    char K1[100];
    printf("Enter the secret key: ");
    scanf("%s",K1);

    //playfair matrix will be generated using the secret key
    char playfair_matrix[6][5];
    create_playfair_matrix(K1,playfair_matrix);

    //printing the playfair matrix
    printf("The playfair matrix is: \n");
    for(int i=0;i<6;i++){
        for(int j=0;j<5;j++){
            printf("%c ",playfair_matrix[i][j]);
        }
        printf("\n");
    }

    //C1 will be the ciphertext generated by playfair cipher
    char C1[100];
    printf("The ciphertext generated by playfair cipher(C1) is: %s\n",encryption_playfair(new_plaintext,playfair_matrix,C1));

    //C2 will be the ciphertext generated by affine cipher
    //K2 is the key for affine cipher given as (11,15) in the question
    int K2[2]={11,15};
    char C2[100];
    printf("The ciphertext generated by affine cipher is: %s\n",encryption_affine(C1,K2,C2));

    //C3 will be the ciphertext generated by shift cipher
    //K3 is the key for shift cipher which will be input by the user.It should be between 0 and 29 
    //as specified in the question
    int K3;
    printf("Enter the key for shift cipher(between 0 and 29): ");
    scanf("%d",&K3);

    //checking if the key is between 0 and 29 or not
    if(K3<0 || K3>29){
        printf("Invalid key\n");
        return 0;
    }

    
    char C3[100];
    printf("The ciphertext generated by shift cipher is: %s\n",encryption_shift(C2,K3,C3));

    //Now, we will decrypt all the ciphertexts one by one to get back the original plaintext

    //First, we will decrypt C3 using shift cipher to get back C2
    //the decrypted C2 will be stored in C2_decrypted
    char C2_decrypted[100];
    printf("The decrypted C2 is: %s\n",decryption_shift(C3,K3,C2_decrypted));

    //Now, we will decrypt C2 using affine cipher to get back C1
    //the decrypted C1 will be stored in C1_decrypted
    char C1_decrypted[100];
    printf("The decrypted C1 is: %s\n",decryption_affine(C2_decrypted,K2,C1_decrypted));

    //Now, we will decrypt C1 using playfair cipher to get back the original plaintext
    //the decrypted plaintext will be stored in plaintext_decrypted
    char plaintext_decrypted[100];
    printf("The decrypted plaintext is: %s\n",decryption_playfair(C1_decrypted,playfair_matrix,K1,plaintext_decrypted));

    //After performing all these, we will see that the decrypted plaintext will be same as the original plaintext
    //So, our implementation is correct

    return 0;
}