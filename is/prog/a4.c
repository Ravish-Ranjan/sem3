#include "a4.h"

int extended_euclidean(int a, int b, int* x, int* y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    int x1,y1;

    int gcd = extended_euclidean(b,a%b,&x1,&y1);

    *x = y1;
    *y = x1 - (a/b)*y1;

    return gcd;
}

char* additive_cipher(const char* data, int key) {
    char* output = (char*)calloc(strlen(data)+1,sizeof(char));

    int at = 0;
    while (data[at] != '\0') {
        int c = data[at];

        if (
            ((c >= 'a') && (c <= 'z')) || 
            ((c >= 'A') && (c <= 'Z'))
        ) {
            c += (key%26);
            
            if (
                ((data[at] >= 'a') && (data[at] <= 'z') && (c > 'z')) || 
                ((data[at] >= 'A') && (data[at] <= 'Z') && (c > 'Z'))
            ) {
                c -= 26;
            } else if (
                ((data[at] >= 'a') && (data[at] <= 'z') && (c < 'a')) || 
                ((data[at] >= 'A') && (data[at] <= 'Z') && (c < 'A'))
            ) {
                c += 26;
            }
        } else if ((c >= '0') && (c <= '9')) {
            c += (key%10);
            if (c > '9') c -= 10;
            else if (c < '0') c += 10;
        }
        output[at++] = (char)c;
    }
    output[at] = '\0';

    return output;
}

char* multiplicative_cipher(const char* data, int key) {
    char* output = (char*)calloc(strlen(data)+1,sizeof(char));
    output[strlen(data)] = '\0';

    key %= 26;
    if (key < 0) key += 26;

    int x,y;

    if (extended_euclidean(key,26,&x,&y) != 1) {
        printf("Error : Not a valid key\n");
        return output;
    }

    int at = 0;
    while (data[at] != '\0') {
        int c = data[at];

        if ((c >= 'a') && (c <= 'z')) {
            c = 'a' + ((c - 'a')*key)%26;
        } else if ((c >= 'A') && (c <= 'Z')) {
            c = 'A' + ((c - 'A')*key)%26;
        } 

        output[at++] = (char)c;
    }
    output[at] = '\0';

    return output;
}

char* additive_decipher(const char* data, int key) {
    return additive_cipher(data,-key);
}

char* multiplicative_decipher(const char* data, int key) {
    int x,y;
    key %= 26;
    int gcd = extended_euclidean(key,26,&x,&y);
    
    char* output = (char*)calloc(strlen(data)+1,sizeof(char));
    output[strlen(data)] = '\0';

    if (gcd != 1) {
        printf("Multiplicative cipher Error : Not a valid key\n");
        return output;
    }

    return multiplicative_cipher(data,(x%26 + 26)%26);
}

// int main() {
//     char* data = "Hello, World! This is Ravish Ranjan in 2026: Cipher Test #1. Keep it Safe & Simple!";
//     int key = 75;

//     char* add_cipher = additive_cipher(data,key);
//     char* add_decipher = additive_decipher(add_cipher,key);
//     char* mul_cipher = multiplicative_cipher(data,key);
//     char* mul_decipher = multiplicative_decipher(mul_cipher,key);

//     printf("\nData     : %s\n\n",data);
//     printf("Cipher   : %s\n",add_cipher);
//     printf("Decipher : %s\n",add_decipher);
//     printf("Is Same  : %s\n\n",strcmp(add_decipher,data) == 0? "Yes":"No");

//     printf("Cipher   : %s\n",mul_cipher);
//     printf("Decipher : %s\n",mul_decipher);
//     printf("Is Same  : %s\n\n",strcmp(mul_decipher,data) == 0?"Yes":"No");
//     return 0;
// }