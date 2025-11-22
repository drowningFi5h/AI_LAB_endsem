#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define F 283

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
int mix_column[4][4] = {
    {1, 4, 4, 5},
    {5, 1, 4, 4},
    {4, 5, 1, 4},
    {4, 4, 5, 1}};
int mix_column_inv[4][4] = {
    {165, 7, 26, 115},
    {115, 165, 7, 26},
    {26, 115, 165, 7},
    {7, 26, 115, 165}};

int RCON[] = {0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000};
// helper functions
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

int mod(int dividend)
{
  int divisor = F;
  int reminder = 0;
  int quotient = 1;
  if (bit_len(dividend) < bit_len(divisor))
  {
    return dividend;
  }
  while (bit_len(dividend) >= bit_len(divisor))
  {
    int diff = bit_len(dividend) - bit_len(divisor);
    quotient += (int)pow(2, diff);
    reminder = dividend ^ (divisor << diff);
    dividend = reminder;
  }
  return reminder;
}

int multiplication(int q1, int q2)
{
  int count = 0;
  int ans = 0;
  while (q2)
  {
    int lsb = q2 & 1;
    if (lsb)
    {
      ans ^= q1 << count;
    }
    q2 >>= 1;
    count++;
  }
  // printf("ans : %d\n",ans);
  return ans;
}

// functions for calculation
int cal_sub(int x)
{
  int temp_res = (x * 211);
  // printf("%xx211 without mod = %x\n", x,temp_res);
  temp_res = mod(temp_res);
  // printf("%xx211 with mod = %x\n",x, temp_res);
  temp_res = temp_res ^ 122;
  // printf("%xx211 + 122 without mod = %x\n", x, temp_res);
  temp_res = mod(temp_res);
  // printf("%xx211 + 122 with mod = %x\n", x, temp_res);
  int row = (x >> 4) & 0xF;
  int col = x & 0xF;
  // printf("row = %x, col = %x \n", row, col);
  // printf("Sub(0x%x*211 + 122) = %x", x, subBytes_table[row][col]);
  return subBytes_table[row][col];
}
int *mix_col(int *col)
{
  int *m = (int *)malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; i++)
  {
    int sum = 0;
    for (int j = 0; j < 4; j++)
    {
      sum ^= multiplication(mix_column[i][j], col[i]);
      sum = mod(sum);
    }
    m[i] = sum;
  }
  return m;
}
int *mix_col_inv(int *mcol)
{
  int *c = (int *)malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; i++)
  {
    int sum = 0;
    // printf("Sum variations : \n");
    for (int j = 0; j < 4; j++)
    {
      int mult = multiplication(mix_column_inv[i][j], mcol[i]);
      // printf("mult[%d][%d]: 0x%x before mod\n", i, j , mult);
      mult = mod(mult);
      // printf("mult[%d][%d]: 0x%x after mod\n", i, j , mult);
      sum ^= mult;
      // printf("Sum[%d][%d]: 0x%x \n", i, j , sum);
    }
    // printf("Sum[%d]: 0x%x\n", i, sum);
    c[i] = sum;
  }
  return c;
}
int rot_word(int word)
{
  // int first = 0x00000000;
  unsigned char first = word >> 24;
  // printf("First: 0x%x\n",first);
  int last = word & 0x00FFFFFF;
  // printf("Last: 0x%x\n",last);
  int shifted = last << 8;
  // printf("Shifted: 0x%x\n",shifted);
  int ans = shifted | first;
  // printf("Ans: 0x%x\n",ans);
  // return (word >> 24) | ((word & 0x00FFFFFF) << 8);
  return ans;
}
int sub_word(int word)
{
  // int B1 = (word & 0xFF000000) >> 24;
  // int B2 = (word & 0x00FF0000) >> 16;
  // int B3 = (word & 0x0000FF00) >> 8;
  // int B4 = word & 0x000000FF;
  //   printf("B1: 0x%x, B2: 0x%x, B3: 0x%x, B4: 0x%x\n",B1,B2,B3,B4);
  //   printf("B1: \n");
  // B1 = cal_sub(B1);
  //   printf("B: \n");
  // B2 = cal_sub(B2);
  //   printf("B1: \n");
  // B3 = cal_sub(B3);
  // printf("B1: \n");
  // B4 = cal_sub(B4);
  //   printf("B1: 0x%x, B2: 0x%x, B3: 0x%x, B4: 0x%x\n",B1,B2,B3,B4);
  return (cal_sub((word & 0xFF000000) >> 24) << 24) | (cal_sub((word & 0x00FF0000) >> 16) << 16) | (cal_sub((word & 0x0000FF00) >> 8) << 8) | cal_sub(word & 0x000000FF);
}
int ***key_scheduling(int *key)
{
  int *all_keys = (int *)malloc(60 * sizeof(int *));
  ;
  int *initial_key_words = (int *)malloc(8 * sizeof(int *));
  int j = 0;
  for (int i = 0; i < 32; i += 4)
  {
    // printf("Key[%d]: 0x%x\n",i,key[i]);
    initial_key_words[j] = key[i] << 24 | key[i + 1] << 16 | key[i + 2] << 8 | key[i + 3];
    j++;
  }
  for (int i = 0; i < 8; i++)
  {
    // printf("word[%d]: 0x%x\n", i, initial_key_words[i]);
  }
  int N = 8;
  for (int i = 0; i < 60; i++)
  {
    if (i < N)
    {
      // printf("inside first if \n");
      all_keys[i] = initial_key_words[i];
    }
    else if (i >= N && i % N == 0)
    {
      // printf("inside second if \n");
      // printf("all_keys[%d]: 0x%x\n",i-1,all_keys[i-1]);
      all_keys[i] = all_keys[i - N] ^ sub_word(rot_word(all_keys[i - 1])) ^ RCON[i / N];
      // printf("rotword[%d]: 0x%x\n",i, rot_word(all_keys[i-1]));
      // printf("subword[%d]: 0x%x\n",i, sub_word(rot_word(all_keys[i-1])));
      // printf("RCON[%d]: 0x%x\n",RCON[i/N]);
      // printf("After XOR with RCON[%d]: 0x%x\n",i, sub_word(rot_word(all_keys[i-1])) ^ RCON[i/N]);
      // printf("final[%d] : 0x%x\n",i,  all_keys[i]);
    }
    else if (i >= N && i % N == 4)
    {
      // printf("inside third if \n");
      all_keys[i] = all_keys[i - N] ^ sub_word(all_keys[i - 1]);
    }
    else
    {
      // printf("inside fourth if \n");
      all_keys[i] = all_keys[i - N] ^ all_keys[i - 1];
    }
    // printf("all_keys[%d]: 0x%x\n", i, all_keys[i]);
  }
  int ***round_keys = (int ***)malloc(15 * sizeof(int **));
  for (int i = 0; i < 15; i++)
  {
    round_keys[i] = (int **)malloc(4 * sizeof(int *));
    for (int j = 0; j < 4; j++)
    {
      round_keys[i][j] = (int *)malloc(4 * sizeof(int));
    }
  }
  int k = 0;
  for(int i = 0;i<15;i++){
    for(int j = 0;j<4;j++){
      round_keys[i][j][0] = (all_keys[k] & 0xFF000000) >> 24;
      round_keys[i][j][1] = (all_keys[k] & 0x00FF0000) >> 16;
      round_keys[i][j][2] = (all_keys[k] & 0x0000FF00) >> 8;
      round_keys[i][j][3] = all_keys[k] & 0x000000FF;
      k++;
    }
  }
  return round_keys;
}
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
int **one_round(int pt[4][4], int key[4][4])
{
  int result[4][4] = {0};
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      result[i][j] = pt[i][j] ^ key[i][j];
      result[i][j] = cal_sub(result[i][j]);
    }
  }
  printf("After xor with key\n");
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      printf("%x ",result[i][j]);
    }
    printf("\n");
  }
  int **shifted_result = shift_rows(result);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result[i][j] = shifted_result[i][j];
    }
  }
  free(shifted_result);
  printf("After shift rows\n");
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      printf("%x ",result[i][j]);
    }
    printf("\n");
  }
  // col-1
  int *col = (int*)malloc(4*sizeof(int));
  for(int i = 0;i<4;i++){
    col[i] = result[i][0];
  }
  col = mix_col(col);
  for(int i =0;i<4;i++){
    result[i][0] = col[i];
  }
  // free(col);
  // col-2
  // int *col = (int*)malloc(4*sizeof(int));
  for(int i = 0;i<4;i++){
    col[i] = result[i][1];
  }
  col = mix_col(col);
  for(int i =0;i<4;i++){
    result[1][0] = col[i];
  }
  // free(col);
  // col-3
  // int *col = (int*)malloc(4*sizeof(int));
  for(int i = 0;i<4;i++){
    col[i] = result[i][2];
  }
  col = mix_col(col);
  for(int i =0;i<4;i++){
    result[i][2] = col[i];
  }
  // free(col);
  // col-4
  // int *col = (int*)malloc(4*sizeof(int));
  for(int i = 0;i<4;i++){
    col[i] = result[i][3];
  }
  col = mix_col(col);
  for(int i =0;i<4;i++){
    result[i][3] = col[i];
  }
  free(col);
  printf("After mix col\n");
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      printf("%x ",result[i][j]);
    }
    printf("\n");
  }
  return result;
}
int **aes_encryption(int pt[4][4], int key[32]){
  int ***round_keys = (int ***)malloc(15 * sizeof(int **));
  for (int i = 0; i < 15; i++)
  {
    round_keys[i] = (int **)malloc(4 * sizeof(int *));
    for (int j = 0; j < 4; j++)
    {
      round_keys[i][j] = (int *)malloc(4 * sizeof(int));
    }
  }
  int **copy_plaintext = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++) {
        copy_plaintext[i] = (int *)malloc(4 * sizeof(int));
        for (int j = 0; j < 4; j++) {
            copy_plaintext[i][j] = pt[i][j];
        }
    }

    for (int i = 0; i < 15; i++) {
        int **temp = one_round(copy_plaintext, (int (*)[4])round_keys[i]);
        for (int j = 0; j < 4; j++) {
            free(copy_plaintext[j]);
        }
        free(copy_plaintext);
        copy_plaintext = temp;
    }

    // Free memory allocated for copy_plaintext
    for (int i = 0; i < 4; i++) {
        free(copy_plaintext[i]);
    }
    free(copy_plaintext);
  for(int i = 0;i<14;i++){
    printf("---------------rounnd[%d]---------------",i+1);
    copy_plaintext = one_round(copy_plaintext,round_keys[i]);
  }
}
int main()
{
  // int x = 0x53;
  // printf("x = 0x%02x\n", x);
  // int sub = cal_sub(x);
  // printf("Subbytes(0x%x) = 0x%x\n",x,sub);
  // int col[] = {0xd9,0xd9,0xd9,0xd9};
  // int *mcol = mix_col(col);
  // printf("Printing mix col: ");
  // for(int i = 0;i<4;i++){
  //   printf("0x%x ",mcol[i]);
  // }
  // printf("\n");
  // int *inv_mcol = mix_col_inv(mcol);
  // printf("Printing inverse mix col:");
  // for(int i = 0;i<4;i++){
  //   printf("0x%x ",inv_mcol[i]);
  // }
  // printf("\n");
  // int **keys = (int **)malloc(15 * sizeof(int *));
  // for (int i = 0; i < 15; i++)
  // {
  //   keys[i] = (int *)malloc(32 * sizeof(int));
  // }
  // keys = key_scheduling(key);
  // for (int i = 0; i < 15; i++)
  // {
  //   printf("Key %d: ", i + 1);
  //   for (int j = 0; j < 32; j++)
  //   {
  //     printf("0x%0x ", keys[i][j]);
  //   }
  //   printf("\n");
  // }
  int pt[4][4] = {
      {0xd4, 0xe0, 0xb8, 0x1e},
      {0x27, 0xbf, 0xb4, 0x41},
      {0x11, 0x98, 0x5d, 0x52},
      {0xae, 0xf1, 0xe5, 0x30}};
  // int **sr = (int **)malloc(15 * sizeof(int *));
  // for (int i = 0; i < 15; i++)
  // {
  //   sr[i] = (int *)malloc(32 * sizeof(int));
  // }
  // sr = shift_rows(pt);
  // for (int i = 0; i < 4; i++)
  // {
  //   // printf("Key %d: ", i + 1);
  //   for (int j = 0; j < 4; j++)
  //   {
  //     printf("0x%0x ", sr[i][j]);
  //   }
  //   printf("\n");
  // }
  int key[32] = {
      0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
  // int ***round_keys = (int ***)malloc(15 * sizeof(int **));
  // for (int i = 0; i < 15; i++)
  // {
  //   round_keys[i] = (int **)malloc(4 * sizeof(int *));
  //   for (int j = 0; j < 4; j++)
  //   {
  //     round_keys[i][j] = (int *)malloc(4 * sizeof(int));
  //   }
  // }
  // round_keys = key_scheduling(key);
  // for(int i = 0;i<15;i++){
  //   printf("\n--------\n");
  //   for(int j = 0;j<4;j++){
  //     for(int k = 0;k<4;k++){
  //       printf("%x ",round_keys[i][j][k]);
  //     }
  //     printf("\n");
  //   }
  // }
  aes_encryption(pt, key);
  return 0;
}
