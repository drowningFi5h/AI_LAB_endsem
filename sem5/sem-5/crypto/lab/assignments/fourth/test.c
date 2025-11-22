#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h> // Include SHA-512 from OpenSSL
#include <string.h>


#define F 283
#define P 1223
#define A 439
#define B 133
#define THETA_X 0
#define THETA_Y 1
// S-Box
int subBytes_table[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}};
int invSubBytes_table[16][16] = {
    {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};    

// Mix Column Matrices
int mix_column[4][4] = {
    {1, 4, 4, 5}, {5, 1, 4, 4}, {4, 5, 1, 4}, {4, 4, 5, 1}};
int mix_column_inv[4][4] = {
    {165, 7, 26, 115}, {115, 165, 7, 26}, {26, 115, 165, 7}, {7, 26, 115, 165}};

// Round Constants
int RCON[] = {0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000};

// Helper Function: Calculate Bit Length
int bit_len(int num)
{
    int bit = 0;
    while (num > 0)
    {
        bit++;
        num >>= 1;
    }
    return bit;
}

// Helper Function: Modular Reduction
int mod(int dividend)
{
    int divisor = F;
    while (bit_len(dividend) >= bit_len(divisor))
    {
        int diff = bit_len(dividend) - bit_len(divisor);
        dividend ^= (divisor << diff);
    }
    return dividend;
}
// Function to perform modular addition
int mod_add(int a, int b)
{
    return (a + b) % P;
}

// Function to perform modular subtraction
int mod_sub(int a, int b)
{
    return (a - b + P) % P;
}

// Function to perform modular multiplication
int mod_mul(int a, int b)
{
    return (a * b) % P;
}
// Helper Function: Multiplication in GF(2^8)
int multiplication(int q1, int q2)
{
    int ans = 0;
    while (q2)
    {
        if (q2 & 1)
            ans ^= q1;
        q1 <<= 1;
        if (q1 & 0x100)
            q1 ^= F; // Apply reduction
        q2 >>= 1;
    }
    return ans;
}
// Helper Function : inv_mod
int mod_inverse(int a, int p)
{
    int m0 = p, t, q;
    int x0 = 0, x1 = 1;
    if (p == 1)
        return 0;
    while (a > 1)
    {
        q = a / p;
        t = p;
        p = a % p, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}
// Function to check if a point lies on the elliptic curve
int is_on_curve(int x, int y)
{
    int left = (y * y) % P;
    int right = (x * x * x + A * x + B) % P;
    return left == right;
}

// Function to generate a random point on the curve
void generate_random_point(int *x, int *y)
{
    srand(time(NULL)); // Seed for random number generation
    do
    {
        *x = rand() % P;
        *y = rand() % P;
    } while (!is_on_curve(*x, *y)); // Repeat until the point lies on the curve
}
int is_point_at_infinity(int x, int y)
{
    return (x == THETA_X && y == THETA_Y);
}
void generate_random_alpha(int *x, int *y)
{
    do
    {
        generate_random_point(x, y); // Generate a random point on the curve
    } while (is_point_at_infinity(*x, *y)); // Ensure it is not Θ
}
int input_private_key(const char *name)
{
    int key;
    do
    {
        printf("Enter %s's private key (1 to 533): ", name);
        scanf("%d", &key);
        if (key < 1 || key > 533)
        {
            printf("Invalid key! Please enter a value between 1 and 533.\n");
        }
    } while (key < 1 || key > 533);
    return key;
}
// Function to perform point doubling on the elliptic curve
void point_double(int x, int y, int *rx, int *ry)
{
    if (y == 0)
    {
        *rx = THETA_X;
        *ry = THETA_Y; // Point at infinity
        return;
    }

    int m = mod_mul(3 * x * x + A, mod_inverse(2 * y, P));
    *rx = mod_sub(mod_mul(m, m), mod_mul(2, x));
    *ry = mod_sub(mod_mul(m, mod_sub(x, *rx)), y);
}
// Function to perform point addition on the elliptic curve
void point_add(int x1, int y1, int x2, int y2, int *rx, int *ry)
{
    if (is_point_at_infinity(x1, y1))
    {
        *rx = x2;
        *ry = y2;
        return;
    }
    if (is_point_at_infinity(x2, y2))
    {
        *rx = x1;
        *ry = y1;
        return;
    }
    if (x1 == x2 && y1 == mod_sub(0, y2))
    {
        *rx = THETA_X;
        *ry = THETA_Y; // Point at infinity
        return;
    }

    int m = (x1 == x2) ? mod_mul(3 * x1 * x1 + A, mod_inverse(2 * y1, P))
                       : mod_mul(mod_sub(y2, y1), mod_inverse(mod_sub(x2, x1), P));
    *rx = mod_sub(mod_mul(m, m), mod_add(x1, x2));
    *ry = mod_sub(mod_mul(m, mod_sub(x1, *rx)), y1);
}

// Function to perform scalar multiplication on the elliptic curve
void scalar_mult(int n, int x, int y, int *rx, int *ry)
{
    *rx = THETA_X;
    *ry = THETA_Y; // Start at infinity
    int tx = x, ty = y;

    while (n > 0)
    {
        if (n & 1)
        {
            point_add(*rx, *ry, tx, ty, rx, ry);
        }
        point_double(tx, ty, &tx, &ty);
        n >>= 1;
    }
}
void compute_sha512(int x, int y, unsigned char *output)
{
    // Concatenate x and y
    char input[64];
    snprintf(input, sizeof(input), "%d%d", x, y);

    // Compute SHA-512
    SHA512((unsigned char *)input, strlen(input), output);
}

// Function to print hash in 64-byte format
void print_hash(unsigned char *hash)
{
    for (int i = 0; i < 64; i++)
    {
        printf("%02x ", hash[i]);
    }
    printf("\n");
}
int cal_inv_sub(int y)
{
    // Modular subtraction: (y - 122) in GF(2^8)
    int temp = mod(y ^ 122); // Subtraction is XOR in GF(2^8)
    printf("after subtracting 122 = %x\n",temp);
    
    // Modular multiplication: 211^(-1) * temp in GF(2^8)
    int inv_211 = mod_inverse(211, F); // Compute 211^(-1)
    printf("origninal input = %x\n",inv_211);
    int x = multiplication(inv_211, temp); // Multiply in GF(2^8)
    
    // Use the inverse S-Box for the result
    int row = (x >> 4) & 0xF; // Extract high nibble
    int col = x & 0xF;        // Extract low nibble
    return invSubBytes_table[row][col];
}
// SubBytes Transformation
int cal_sub(int x)
{
    x = mod(x * 211); // Multiply by 211 in GF(2^8)
    x ^= 122;
    x = mod(x);         // XOR with 122
    int row = (x >> 4) & 0xF;
    int col = x & 0xF;
    return subBytes_table[row][col];
}
// shift rows
int **shift_rows(int pt[4][4])
{ // assuming that pt will be 2D array of 4x4 where each entry is of 1byte
    int **result = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        result[i] = (int *)malloc(4 * sizeof(int));
    }
    for (int i = 0; i < 4; i++)
    {
        result[0][i] = pt[0][i];
        result[1][i] = pt[1][(i + 1) % 4];
        result[2][i] = pt[2][(i + 2) % 4];
        result[3][i] = pt[3][(i + 3) % 4];
    }
    return result;
}
// Mix Columns Transformation
void mix_col(int *col)
{
    int temp[4];
    for (int i = 0; i < 4; i++)
    {
        temp[i] = 0;
        for (int j = 0; j < 4; j++)
        {
            temp[i] ^= multiplication(mix_column[i][j], col[j]);
        }
        temp[i] = mod(temp[i]);
    }
    for (int i = 0; i < 4; i++)
        col[i] = temp[i];
}

// Rotate and Substitute Word for Key Scheduling
int rot_word(int word)
{
    return (word << 8) | ((word >> 24) & 0xFF);
}

int sub_word(int word)
{
    return (cal_sub((word >> 24) & 0xFF) << 24) | (cal_sub((word >> 16) & 0xFF) << 16) |
           (cal_sub((word >> 8) & 0xFF) << 8) | cal_sub(word & 0xFF);
}

// Key Scheduling
int ***key_scheduling(int *key)
{
    int *all_keys = (int *)malloc(60 * sizeof(int));
    int ***round_keys = (int ***)malloc(15 * sizeof(int **));

    for (int i = 0; i < 15; i++)
    {
        round_keys[i] = (int **)malloc(4 * sizeof(int *));
        for (int j = 0; j < 4; j++)
            round_keys[i][j] = (int *)malloc(4 * sizeof(int));
    }

    for (int i = 0; i < 8; i++)
    {
        all_keys[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) |
                      (key[i * 4 + 2] << 8) | key[i * 4 + 3];
    }

    for (int i = 8; i < 60; i++)
    {
        int temp = all_keys[i - 1];
        if (i % 8 == 0)
            temp = sub_word(rot_word(temp)) ^ RCON[i / 8];
        else if (i % 8 == 4)
            temp = sub_word(temp);
        all_keys[i] = all_keys[i - 8] ^ temp;
    }

    int k = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int l = 0; l < 4; l++)
            {
                round_keys[i][j][l] = (all_keys[k] >> (8 * (3 - l))) & 0xFF;
            }
            k++;
        }
    }
    free(all_keys);
    return round_keys;
}

// AES Encryption
int **aes_encryption(int pt[4][4], int key[32])
{
    int ***round_keys = key_scheduling(key);

    // Allocate memory for the resulting matrix
    int **result = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        result[i] = (int *)malloc(4 * sizeof(int));
    }
    int **shifted = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        shifted[i] = (int *)malloc(4 * sizeof(int));
    }
    // Initial Round
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            pt[i][j] ^= round_keys[0][i][j];
    printf("---------------------------After XOR: ---------------------------\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%x ", pt[i][j]);
        }
        printf("\n");
    }
    // Main Rounds
    for (int r = 1; r < 14; r++)
    {
        printf("---------------------------Round%d: ---------------------------\n", r);
        // SubBytes and ShiftRows
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                pt[i][j] = cal_sub(pt[i][j]);
        printf("---------------------------After subbytes: ---------------------------\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%x ", pt[i][j]);
            }
            printf("\n");
        }
        shifted = shift_rows(pt);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                pt[i][j] = shifted[i][j];
            }
        }
        printf("---------------------------After shitf rows: ---------------------------\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%x ", pt[i][j]);
            }
            printf("\n");
        }
        int temp[4];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                temp[j] = pt[j][i];
            mix_col(temp);
            for (int j = 0; j < 4; j++)
                pt[j][i] = temp[j];
        }
        printf("---------------------------After mixcol: ---------------------------\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%x ", pt[i][j]);
            }
            printf("\n");
        }
        // Add Round Key
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                pt[i][j] ^= round_keys[r][i][j];
    }

    // Final Round
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            pt[i][j] = cal_sub(pt[i][j]);
    printf("---------------------------After subbyte: ---------------------------\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%x ", pt[i][j]);
        }
        printf("\n");
    }
    shifted = shift_rows(pt);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pt[i][j] = shifted[i][j];
        }
    }
    printf("---------------------------After shitf rows: ---------------------------\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%x ", pt[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            pt[i][j] ^= round_keys[14][i][j];

    // Copy the result into dynamically allocated memory
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = pt[i][j];

    // Free Round Keys
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 4; j++)
            free(round_keys[i][j]);
        free(round_keys[i]);
    }
    free(round_keys);

    return result; // Return the dynamically allocated result
}

// int main()
// {
//     // int x = 0x53;
//     // printf("x = 0x%02x\n", x);
//     // int sub = cal_sub(x);
//     // printf("Subbytes(0x%x) = 0x%x\n",x,sub);
//     // int col[] = {0xd9,0xd9,0xd9,0xd9};
//     // int *mcol = mix_col(col);
//     // printf("Printing mix col: ");
//     // for(int i = 0;i<4;i++){
//     //   printf("0x%x ",mcol[i]);
//     // }
//     // printf("\n");
//     // int *inv_mcol = mix_col_inv(mcol);
//     // printf("Printing inverse mix col:");
//     // for(int i = 0;i<4;i++){
//     //   printf("0x%x ",inv_mcol[i]);
//     // }
//     // printf("\n");
//     // int **keys = (int **)malloc(15 * sizeof(int *));
//     // for (int i = 0; i < 15; i++)
//     // {
//     //   keys[i] = (int *)malloc(32 * sizeof(int));
//     // }
//     // keys = key_scheduling(key);
//     // for (int i = 0; i < 15; i++)
//     // {
//     //   printf("Key %d: ", i + 1);
//     //   for (int j = 0; j < 32; j++)
//     //   {
//     //     printf("0x%0x ", keys[i][j]);
//     //   }
//     //   printf("\n");
//     // }
//     int pt[4][4] = {
//         {0xd4, 0xe0, 0xb8, 0x1e},
//         {0x27, 0xbf, 0xb4, 0x41},
//         {0x11, 0x98, 0x5d, 0x52},
//         {0xae, 0xf1, 0xe5, 0x30}};
//     // int **sr = (int **)malloc(15 * sizeof(int *));
//     // for (int i = 0; i < 15; i++)
//     // {
//     //   sr[i] = (int *)malloc(32 * sizeof(int));
//     // }
//     // sr = shift_rows(pt);
//     // for (int i = 0; i < 4; i++)
//     // {
//     //   // printf("Key %d: ", i + 1);
//     //   for (int j = 0; j < 4; j++)
//     //   {
//     //     printf("0x%0x ", sr[i][j]);
//     //   }
//     //   printf("\n");
//     // }
//     int key[32] = {
//         0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
//         0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
//         0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
//         0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
//     // int ***round_keys = (int ***)malloc(15 * sizeof(int **));
//     // for (int i = 0; i < 15; i++)
//     // {
//     //   round_keys[i] = (int **)malloc(4 * sizeof(int *));
//     //   for (int j = 0; j < 4; j++)
//     //   {
//     //     round_keys[i][j] = (int *)malloc(4 * sizeof(int));
//     //   }
//     // }
//     // round_keys = key_scheduling(key);
//     // for(int i = 0;i<15;i++){
//     //   printf("\n--------\n");
//     //   for(int j = 0;j<4;j++){
//     //     for(int k = 0;k<4;k++){
//     //       printf("%x ",round_keys[i][j][k]);
//     //     }
//     //     printf("\n");
//     //   }
//     // }
//     aes_encryption(pt, key);
//     return 0;
// Function to input and parse Alice's 128-bit message
void input_message(unsigned char *message)
{
    printf("Enter Alice's 128-bit message (16 space-separated hex bytes):\n");
    for (int i = 0; i < 16; i++)
    {
        scanf("%hhx", &message[i]); // Read hexadecimal byte
    }
}

// Function to print the message
void print_message(unsigned char *message)
{
    printf("Message MA: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", message[i]);
    }
    printf("\n");
}

// int main()
// {
//     // Step III: Generate a random point α
//     int alpha_x, alpha_y;
//     generate_random_alpha(&alpha_x, &alpha_y);
//     printf("Random point α on the curve: (%d, %d)\n", alpha_x, alpha_y);

//     // Step IV: Input private keys
//     int nA = input_private_key("Alice");
//     int nB = input_private_key("Bob");

//     // Step V: Diffie-Hellman Key Exchange
//     int alice_pub_x, alice_pub_y, bob_pub_x, bob_pub_y, shared_x, shared_y;
//     scalar_mult(nA, alpha_x, alpha_y, &alice_pub_x, &alice_pub_y);
//     scalar_mult(nB, alpha_x, alpha_y, &bob_pub_x, &bob_pub_y);
//     scalar_mult(nA, bob_pub_x, bob_pub_y, &shared_x, &shared_y);

//     printf("Alice's public key: (%d, %d)\n", alice_pub_x, alice_pub_y);
//     printf("Bob's public key: (%d, %d)\n", bob_pub_x, bob_pub_y);
//     printf("Shared secret key: (%d, %d)\n", shared_x, shared_y);

//     // Step VI: Compute keys KA and KB
//     unsigned char KA[SHA512_DIGEST_LENGTH];
//     unsigned char KB[SHA512_DIGEST_LENGTH];

//     // Compute SHA-512 hashes
//     compute_sha512(shared_x, shared_y, KA);
//     compute_sha512(shared_x, shared_y, KB);

//     printf("Key KA: ");
//     print_hash(KA);

//     printf("Key KB: ");
//     print_hash(KB);
//     // Step VII: Input Alice's 128-bit message
//     unsigned char MA[16];
//     input_message(MA);
//     print_message(MA);
//     return 0;
// }
int main(){
     int input = 0x53;                     
    int substituted = cal_sub(input);    
    int inverted = cal_inv_sub(substituted); 

    printf("Original: 0x%02x\n", input);
    printf("After SubBytes: 0x%02x\n", substituted);
    printf("After InvSubBytes: 0x%02x\n", inverted);
    return 0;
}