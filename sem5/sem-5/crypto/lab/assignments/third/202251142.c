#include<stdio.h>
#include<math.h>

int divisor_arr[100],quotient_arr[100];
int init_dividend = 283, quotient=0, reminder=0,iterator;

int bit_len(int num){
    int bit = 0;
    while(num>0){
        bit++;
        num>>=1;
    }
    return bit;
}
void div(int dividend, int divisor){
    while(bit_len(dividend) >= bit_len(divisor)){
        int diff = bit_len(dividend) - bit_len(divisor);
        quotient += (int)pow(2,diff);
        reminder = dividend ^ (divisor << diff);
        // printf("q: %d, r: %d", quotient, reminder);     
        dividend = reminder;
    }
}
void long_div(int divisor){
    for(int i = 0;i<100;i++){
        quotient_arr[i] = divisor_arr[i] = 0;
    }
    if(divisor == 0)return;
    if(divisor == 1)return;
    int dividend = init_dividend;
    // printf("%d\n",dividend);
    while(1){
        div(dividend,divisor);
        quotient_arr[iterator] = quotient;
        divisor_arr[iterator] = divisor;
        // printf("q: %d, r: %d\n", quotient, reminder);
        dividend = divisor;
        // printf("%d\n",dividend);
        divisor = reminder;
        iterator++;
        if(reminder == 1)break;
        quotient = 0;
        reminder = 0;
    }
}
int multiplication(int q1, int q2){
    int count = 0;
    int ans = 0;
    while(q2){
        int lsb = q2&1;
        if(lsb){
            ans ^= q1<<count;
        }
        q2>>=1;
        count++;
    }
    // printf("ans : %d\n",ans);
    return ans;
}
int mult_inv(){
    quotient_arr[iterator-2] = multiplication(quotient_arr[iterator-1],quotient_arr[iterator-2])  ^ 1;
    for(int i = iterator - 3; i>=0;i--){
        quotient_arr[i] = multiplication(quotient_arr[i],quotient_arr[i+1]) ^ quotient_arr[i+2];
    }
    return quotient_arr[0];
}
int subByte(int ans){
    if(ans == 0) return 99;
    if(ans == 1) return 124;
    // long_div(a);
    // int ans = mult_inv();
    // printf("Ans in subbyte: %d\n",ans);
    int c = 99;
    int result = 0;
    for(int i = 7;i>=0;i--){
        int sum = 0;
        int x = ((ans >> i)&1);
        // printf("bit: %d, x : %d\n",i,x);
        int y = ((ans >> (i+4)%8)&1);
        // printf("bit: %d, y : %d\n",(i+4)%8,y);
        int z = ((ans >> (i+5)%8)&1);
        // printf("bit: %d, z : %d\n",(i+5)%8,z);
        int w = ((ans >> ((i+6)%8))&1);
        // printf("bit: %d, w : %d\n",(i+6)%8,w);
        int p = ((ans >> ((i+7)%8))&1);
        // printf("bit: %d, p : %d\n",(i+7)%8,p);
        int q = ((c >> i)&1);
        // printf("q : %d\n",q);
        sum =  x + y + z + w + p + q;
        sum %= 2;
        // printf("sum[%d] = %d\n",i,sum);
        result <<= 1;
        result |= sum;
        // printf("result = %d\n",result);
    }
    return result;
}
int handleSingleinput(unsigned int num){
    int a = (int)num;
    long_div(a);
    printf("---------Printing quotients and divisors : ---------\n");
    for(int j = 0;j<iterator;j++){
        printf("quotient(%d): 0x%x(%d)\tdivisor(%d): 0x%x(%d) \n",j,quotient_arr[j],quotient_arr[j],j,divisor_arr[j],divisor_arr[j]);
    }
    printf("it : %d\n",iterator);
    int inv = mult_inv();
    // int ans = multiplication(5,4);
    // printf("--------- Printing multiplicative inverse: ---------\n");
    // printf("Multiplicative inverse : 0x%x(%d)\n",quotient_arr[0],quotient_arr[0]);
    // printf("ans : %d\n",ans);
    printf("multiplicative inverse of %d in given field is %d\n",a,inv);
    int result = subByte(inv);
    printf("--------- Printing final Subbyte: ---------\n");
    printf("Subbyte for 0x%x(%d) is 0x%x(%d)\n", a, a , result , result);
    return result;
}
// void handle_table(){
//     int table[17][17];
//     unsigned int count = 0;
//     for(int i = 1;i<17;i++){
//         for(int j = 1;j<17;j++){
//             int temp = handleSingleinput(count);
//             // printf("subbyte of %d : 0x%x\n",count, temp);
//             table[i][j] = temp;
//             count++;
//         }
//     }
//     for(int i = 0;i<17;i++){
//         for(int j = 0;j<17;j++){
//             printf("%x\t",table[i][j]);
//         }
//         printf("\n");
//     }
// }
int main(){
    unsigned int num;
    printf("Enter the number: ");
    scanf("%x",&num);
    int ans = handleSingleinput(num);
    printf("subbyte : 0x%x", ans);
    // handle_table();
    return 0;
}