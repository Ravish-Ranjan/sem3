#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bitsCount(long data) {
    int bits = 0;

    while (data > 0) {
        bits++;
        data>>=1;
    }

    return bits;
}

char* crc(char* message, char* generator) {
    long msg = strtol(message,NULL,2);
    long gen = strtol(generator,NULL,2);

    int msg_len = bitsCount(msg), gen_len = bitsCount(gen);

    if (msg_len < gen_len) {
        printf("Error : generator has to be less long then message\n");
        return "";
    }

    long rem = msg << (gen_len-1);
    int rem_len = msg_len + gen_len - 1;

    do {
        rem = rem ^ (gen << (rem_len - gen_len));
        rem_len = bitsCount(rem);
    } while(rem_len >= gen_len);

    printf("\nRemainder : %lb\n\n",rem);

    msg = (msg << (gen_len-1)) | rem;
    msg_len += (gen_len - 1);

    char* codeword = (char*)calloc(msg_len+1,sizeof(char));

    for(int i=msg_len-1; i>=0; i--) {
        codeword[i] = (msg&1)?'1':'0';
        msg >>= 1;
    }

    codeword[msg_len] = '\0';

    return codeword;
}

int main() {
    char* message = (char*)calloc(65,sizeof(char));
    char* generator = (char*)calloc(65,sizeof(char));

    printf("Enter message : ");
    scanf("%s",&message[0]);
    
    printf("Enter generator : ");
    scanf("%s",&generator[0]);
    
    char* codeword = crc(message,generator);

    printf("Sender codeword : %s\n",codeword);

    free(message);
    free(generator);

    return 0;
}