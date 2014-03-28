#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int BUFFER_SIZE = 4096;
const int TRY_COUNT = 10;

int main(int argc, char ** argv){
    char buf [BUFFER_SIZE];
    for(int i = 0; i < argc - 1; ++i) {
        int fd;
        sscanf(argv[i + 1], "%d", &fd);
        int ret;
        while((ret = read(fd, buf, BUFFER_SIZE)) != 0){
            if (ret == -1) {
                char str[] = "Error read";
                perror(str);
                return EXIT_FAILURE;
            } else {
                int ret2;
                int remained = ret;
                int tries = 0;
                while(remained > 0){
                    ret2 = write(STDOUT_FILENO, buf + ret - remained, remained);
                    if(ret2 < 0) {
                        char str[] = "Error write";
                        perror(str);
                        return EXIT_FAILURE;
                    } else if(ret2 == 0) {
                        ++tries;
                        if(tries == TRY_COUNT){
                            return EXIT_FAILURE;
                        }
                    }else{
                        tries = 0;
                    }
                    remained -= ret2;
                }
            }
        }
    }
    return -1;
}
