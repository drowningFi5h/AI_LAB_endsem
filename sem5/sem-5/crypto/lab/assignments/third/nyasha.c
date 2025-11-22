#include <stdio.h>
#include <stdint.h>


uint16_t binary_to_field(uint8_t binary) {
    printf("\n");
    printf("binary to field conversion: \n");
    printf("binary: %d\n", binary); 
    uint16_t result = binary;
    printf("field representation: ");
    int j = 1;
    for(int i = 7; i >= 0; i--) {
        if((binary >> i) & 1) {
            if(!j){
                printf("+ ");
            } 
            if(i == 0){
                printf("1 ");
            } 
            else{
                printf("x^%d ", i);
            } 
            j = 0;
        }
    }
    printf("\n");
    return result;
}


uint8_t field_to_binary(uint16_t field) {
    printf("\n");
    printf("field to binary conversion: \n");
    printf("field: %d\n", field);

    int j = 1;
    for(int i = 15; i >= 0; i--) {
        if((field >> i) & 1) {
            if(!j){
                printf(" + ");
            } 
            if(i == 0){
                printf(" 1 ");
            } 
            else{
                printf("x^%d ", i);
            } 
            j = 0;
        }
    }
    printf("\n binary form is: 0x%02X\n\n", (uint8_t)field);
    return (uint8_t)field;
}


uint8_t field_inv(uint8_t input) {
    printf("\n");
    printf("multiplicative inverse of 0x%02X\n", input);

    if(input == 0){
        printf("0x00 does not have a multiplicative inverse\n");
        return 0;
    }
     
    uint16_t divident = 0x11B;  // x^8 + x^4 + x^3 + x + 1
    uint16_t divisor = input;
    uint16_t t0 = 0;
    uint16_t t1 = 1;
    
    while(divisor != 0) {
        
        int degree_divident = 15;
        int degree_divisor = 15;
        while(!(divident & (1 << degree_divident)) && degree_divident > 0){
            degree_divident--;
        } 
        while(!(divisor & (1 << degree_divisor)) && degree_divisor > 0){
            degree_divisor--;
        } 
        
        if(divisor == 1) { 
            printf("multiplicative inverse of 0x%02X is 0x%02X\n", input, (uint8_t)t1);
            return (uint8_t)t1;
        }
        
        if(degree_divident < degree_divisor) {
            
            uint16_t temp = divident;
            divident = divisor;
            divisor = temp;
            temp = t0;
            t0 = t1;
            t1 = temp;
            continue;
        }
        
        
        int shift = degree_divident - degree_divisor;
        divident ^= divisor << shift;
        t0 ^= t1 << shift;
    }
    
    printf("mutlipicative inverse doesnt exist\n\n");
    return 0;  
}

uint8_t subbytes(uint8_t input) {
    uint16_t z = binary_to_field(input);
    
    if(z != 0) {
        z = field_inv((uint8_t)z);
    }
    
    uint8_t a = field_to_binary(z);
    uint8_t c = 0x63;  
    uint8_t b = 0;
    
    
    for(int i = 0; i < 8; i++) {
        uint8_t sum = 0;
        // ai + ai+4 + ai+5 + ai+6 + ai+7
        for(int j = 0; j < 5; j++) {
            int idx = (i + j + 4) % 8;
            sum ^= ((a >> idx) & 1);
        }
        // ci
        sum ^= ((c >> i) & 1);
        
        b |= (sum << i);
    }
    
    return b;
}

int main() {
    
    uint8_t test_value;
    printf("Enter the value in hexadecimal: 0x");
    scanf("%hhx", &test_value);
    printf("Input: 0x%02X\n\n", test_value);
    
    uint8_t result = subbytes(test_value);
    printf("Final Result: 0x%02X\n", result);
    
    return 0;
}