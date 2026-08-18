#include <string.h>
#include <stdio.h>

void testing_function(const char* input) {
    char buffer[24];
    strcpy(buffer,input);

    printf("Buffered content : %s\n",buffer);
}

int main(int argc,char *args[]) {
    if (argc != 2) {
        printf("Wrong format\n\tUsage : %s <input>\n",args[0]);
    }

    testing_function(args[1]);

    return 0;
}