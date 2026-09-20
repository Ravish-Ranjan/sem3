#include "a4.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cryptanalysis_additive(const char* ciphertext) {
    printf("1. Cryptanalysis of Additive Cipher\n");
    printf("Ciphertext: %s\n\n", ciphertext);
    printf("Trying all possible keys (0 to 25):\n\n");

    char* plaintext = (char*)calloc(500,sizeof(char));
    for (int k = 0; k < 26; k++) {
        plaintext = additive_decipher(ciphertext, k);
        printf("Key %2d : %s\n", k, plaintext);
    }
    printf("\n");
}

void cryptanalysis_multiplicative(const char* ciphertext) {
    printf("2. Cryptanalysis of Multiplicative Cipher\n");
    printf("Ciphertext: %s\n\n", ciphertext);
    printf("Trying all valid multiplicative keys (coprime to 26):\n");

    char* plaintext = (char*)calloc(500,sizeof(char));
    int valid_keys[] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    int num_keys = sizeof(valid_keys) / sizeof(valid_keys[0]);

    for (int i = 0; i < num_keys; i++) {
        int k = valid_keys[i];
        plaintext = multiplicative_decipher(ciphertext, k);
        printf("Key %2d : %s\n", k, plaintext);
    }
    printf("\n");
}

int main() {
    const char* add_ciphertext = "Ebiil, Tloia! Qefp fp Oxsfpe Oxkgxk fk 7571: Zfmebo Qbpq #6. Hbbm fq Pxcb & Pfjmib!";
    const char* mul_ciphertext = "Fottk, Mkbtr! Vfcy cy Bapcyf Banzan cn 2026: Uchfob Voyv #1. Wooh cv Yalo & Ycqhto!";

    cryptanalysis_additive(add_ciphertext);
    cryptanalysis_multiplicative(mul_ciphertext);

    return 0;
}
