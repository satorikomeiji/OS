#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int BUF_SIZE = 4096;
const int TRY_COUNT = 3;

void my_print(char * buf, int size) {
    int ret;
    int remained = size;
    int tries = 0;
    while(remained > 0) {
        ret = write(1, buf + size - remained, remained);
        if(ret < 0) {
            char str[] = "Error write";
            perror(str);
            exit(-1);
        } else if(ret == 0) {
            ++tries;
            if(tries == TRY_COUNT) {
                exit(-1);
            }
        } else {
            tries = 0;
        }
        remained -= ret;
    }
}



int main(int argc, char ** argv) {
    char buf [BUF_SIZE];
    char *delim = argv[1];
    for(int i = 0; i < argc - 2; ++i) {
        int fd;
        sscanf(argv[i + 2], "%d", &fd);
        int ret;
        while((ret = read(fd, buf, BUF_SIZE)) != 0) {
            if (ret == -1) {
                char str[] = "Error read";
                perror(str);
                return EXIT_FAILURE;
            } else {
                my_print(buf, ret);
           }
        }
        if (i < (argc - 3)) {
            my_print(delim, strlen(delim));
        }
    }

    return EXIT_FAILURE;
}
