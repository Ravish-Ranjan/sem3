#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int bitsCount(long data) {
    int bits = 0;

    while (data > 0) {
        bits++;
        data>>=1;
    }

    return bits;
}

bool crc_check(char* codeword, char* generator) {
    long cdw = strtol(codeword,NULL,2);
    long gen = strtol(generator,NULL,2);

    int cdw_len = bitsCount(cdw), gen_len = bitsCount(gen);

    if (cdw_len < (2*gen_len)) {
        printf("Error : codeword has to be twice as long as generator\n");
        return "";
    }

    long rem = cdw;
    int rem_len = cdw_len;

    do {
        rem = rem ^ (gen << (rem_len - gen_len));
        rem_len = bitsCount(rem);
    } while(rem_len >= gen_len);

    return (rem == 0);
}

int main() {
    char* codeword = (char*)calloc(65,sizeof(char));
    char* generator = (char*)calloc(65,sizeof(char));

    printf("Enter codeword : ");
    scanf("%s",&codeword[0]);
    
    printf("Enter generator : ");
    scanf("%s",&generator[0]);
    
    bool correct = crc_check(codeword,generator);

    printf("Recieved message has %serror\n",correct?"no ":"");

    free(codeword);
    free(generator);

    return 0;
}