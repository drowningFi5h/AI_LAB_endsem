#include <stdio.h>
#include <stdlib.h>

int main() {
    int a, b, n;
    printf("\nEnter the value of a (the coefficient of x): ");
    scanf("%d", &a);
    printf("Enter the value of b (the constant): ");
    scanf("%d", &b);
    printf("Enter the value of n (the modulo): ");
    scanf("%d", &n);
    printf("\nThe points on EL are: \n\n");

    int num_points = 0; // Initialize a variable to count the number of points

    // Iterate through all possible x values (from 0 to n-1)
    for(int i = 0; i < n; i++) {
        int LHS = (i * i) % n; // Compute LHS of EL
        // Iterate through all possible y values (from 0 to n-1)
        for(int j = 0; j < n; j++) {
            int RHS = ((j * j * j) + (a * j) + b) % n; // Compute RHS of EL
            // If the left-hand side equals the right-hand side, it's a valid point
            if(RHS == LHS) {
                num_points++; // Increment the number of points found
                printf("x = %d\t\ty = %d\n", j, i);
            }
        }
    }
    
    printf("\nNumber of points: %d\n\n", num_points); // Print the total number of points found
    return 0;
}