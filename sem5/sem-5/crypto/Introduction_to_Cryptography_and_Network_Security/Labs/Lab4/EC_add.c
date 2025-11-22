//include
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Defining things that would be used throughout the code
#define PRIME 173 // prime number given = 1021
// Elliptical curve given: y^2 = x^3 + 449x + 233 over Zp
#define A_COEFFICIENT 23
#define B_COEFFICIENT 11

// Point structure representing a point on the elliptic curve
typedef struct {
    int x;
    int y;
} Point;

// Extended Euclidean Algorithm to find the modular inverse
int modInverse(int a, int m) {
    int m0 = m, t, q; // m0 is the original m value
    int x0 = 0, x1 = 1;
    
    if (m == 1) return 0; // If m is 1, then the modular inverse is 0

    // Apply extended Euclid Algorithm
    while (a > 1) {
        q = a / m; // q is quotient
        t = m;
        m = a % m, a = t; // m is remainder now, process same as euclid's algo
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0; // Make x1 positive

    return x1;
}

// Addition operation on the elliptic curve
Point ec_add(Point P, Point Q) {
    Point result; // Resultant point
    int slope; // Slope of the line passing through points P and Q

    //Lets consider each possible case which can arise while adding two points on the elliptic curve

    // Case 1: P is the point at infinity (0,1)
    if (P.x == 0 && P.y == 1) {
        return Q;
    }
    // Case 2: Q is the point at infinity (0,1)
    if (Q.x == 0 && Q.y == 1) {
        return P;
    }

    // Case 3: P and Q, both are point at infinity (0,1)
    if (P.x == 0 && P.y == 1 && Q.x == 0 && Q.y == 1) {
        return P;
    }

    // Case 4: Q is mirror image of P in x-axis
    if (P.x == Q.x && P.y == (-Q.y + PRIME) % PRIME) {
        result.x = 0;
        result.y = 1;
        return result;
    }

    // Case 5: P = Q, basically tangent at P
    if (P.x == Q.x && P.y == Q.y) {
        slope = ((((3 * P.x * P.x) % PRIME + A_COEFFICIENT) % PRIME) * (modInverse(2 * P.y, PRIME) % PRIME)) % PRIME;
    }

    // Case: P != Q, both are seperate points on the elliptical curve
    else {
        slope = (((Q.y - P.y + PRIME) % PRIME) * modInverse((Q.x - P.x + PRIME) % PRIME, PRIME)) % PRIME;
    }

    // Calculate the x and y coordinates of the resultant point using slope which we obtained fromcabove cases
    result.x = ((slope * slope) % PRIME + (PRIME - P.x) + (PRIME - Q.x)) % PRIME;
    result.y = ((slope * (P.x - result.x + PRIME)) % PRIME + (PRIME - P.y)) % PRIME;
    // IMP NOTE - I incorporated making R to -R in the above formulae itself, so that I don't have to handle it separately

    // Handle negative coordinates if they arise
    if (result.x < 0) {
        result.x += PRIME;
    }
    if (result.y < 0) {
        result.y += PRIME;
    }

    return result; // Return answer of P+Q
}

int main() {
    // Example points P and Q
    Point P = {28, 109};
    Point Q = {88, 147};

    // Call ec_add function to add points P and Q
    Point result = ec_add(P, Q);

    // Output the result
    printf("Resultant point after addition: (%d, %d)\n", result.x, result.y);

    return 0;
}