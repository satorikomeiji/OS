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


void reverse_char_array(char * array, int len){
    int i;
    char c;
    for(i = 0; (i << 1) < len; ++i){
        c = array[i];
        array[i] = array[len - i - 1];
        array[len - i - 1] = c;
    }
}



int main(int argc, char ** argv) {
    char buf [BUF_SIZE];
    char string_buf [BUF_SIZE * 100];
    char *delim = argv[1];
    int string_size = 0;
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
                int newline_pos = 0;
                int offset1 = 0;//, offset2 = 0;
                while (newline_pos < ret) {
                    offset1 = newline_pos;
                    while (newline_pos < ret && buf[newline_pos] != '\n') {
                        newline_pos++;
                    }
                    //strcpy(buf + offset1, newline_pos - offset1);
                    if (newline_pos != ret) {
                        //buf += newline_pos;
                        if (offset1 == 0) {
                            reverse_char_array(string_buf, string_size); 
                            my_print(string_buf, string_size);
                            string_size = 0;

                        }
                        reverse_char_array(buf + offset1, newline_pos - offset1);
                        my_print(buf + offset1, newline_pos - offset1);
                        offset1 += newline_pos - offset1; 
                        my_print(buf + offset1, 1);
                        newline_pos++;
                    } 
                    else {
                        memcpy(string_buf + string_size, buf + offset1, ret - offset1);
                        string_size += ret - offset1;
                    }
                }
               
           }
        }
        if (i < (argc - 3)) {
            my_print(delim, strlen(delim));
        }
    }

    return EXIT_FAILURE;
}
