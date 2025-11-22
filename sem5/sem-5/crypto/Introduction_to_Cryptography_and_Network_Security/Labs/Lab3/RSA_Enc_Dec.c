#include<stdio.h>

int gcd(int a, int b){
    if(b==0){
        return a;
    }
    return gcd(b,a%b);
}

int generate_e(int phi){
    for(int e=2; e<phi; e++){
        if(gcd(e, phi) == 1){
            return 941; //hard code e here
        }
    }
    return -1;
}

int mod_inverse(int a, int m){
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

int RSA_enc(int p, int q, int plaintext){
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    
    int e = generate_e(phi);
    if(e == -1){
        printf("Error: Unable to find suitable 'e' for encryption.\n");
        return -1;
    }
    
    int ciphertext = 1;
    for(int i=0; i<e; i++){
        ciphertext = (ciphertext * plaintext) % n;
    }
    return ciphertext;
}

int RSA_dec(int p, int q, int ciphertext){
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    
    int e = generate_e(phi);
    if(e == -1){
        printf("Error: Unable to find suitable 'e' for decryption.\n");
        return -1;
    }
    
    int d = mod_inverse(e, phi);
    if(d == -1){
        printf("Error: Unable to calculate decryption key 'd'.\n");
        return -1;
    }
    //print d
    printf("d: %d\n", d);
    
    int plaintext = 1;
    for(int i=0; i<d; i++){
        plaintext = (plaintext * ciphertext) % n;
    }
    return plaintext;
}

int main(){
    int p, q, plaintext, ciphertext;
    printf("Enter two prime numbers (p and q): ");
    scanf("%d %d", &p, &q);
    
    printf("Enter the plaintext (an integer): ");
    scanf("%d", &plaintext);
    
    ciphertext = RSA_enc(p, q, plaintext);
    if(ciphertext != -1){
        printf("Ciphertext: %d\n", ciphertext);
        
        int decrypted_plaintext = RSA_dec(p, q, ciphertext);
        if(decrypted_plaintext != -1){
            printf("Decrypted plaintext: %d\n", decrypted_plaintext);
        }
    }
    
    return 0;
}