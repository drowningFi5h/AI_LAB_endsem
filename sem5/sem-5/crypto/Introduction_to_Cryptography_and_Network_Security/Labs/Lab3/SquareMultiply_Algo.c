#include <stdio.h>

int squareAndMultiply(int x, int c, int p) {
    int bin_c[32];
    int i = 0;
    while (c > 0) {
        bin_c[i] = c % 2;
        c = c / 2;
        i++;
    }
    
    int z = 1;
    int newExp = bin_c[0];
    
    if (bin_c[0] == 1) {
        z = x;
    }
    
    for (int j = 1; j < i; j++) {
        int ci = bin_c[j];
        z = (z * z) % p;
        if (ci == 1) {
            z = (z * x) % p;
        }
    }

    return z;
}

int main() {
    int x;
    int c;
    int p;
    printf("\nCalculate x^c (mod p)\n\n");
    printf("Enter x: ");
    scanf("%d", &x);
    printf("Enter c: ");
    scanf("%d", &c);
    printf("Enter p: ");
    scanf("%d", &p);

    int result = squareAndMultiply(x, c, p);
    printf("\nx^c (mod p) is: %d\n\n", result);
    
    return 0;
}