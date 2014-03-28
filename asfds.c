#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    char *cat = argv[1];
    int fds[argc - 2];
    for (int i = 0; i < argc - 2; ++i) {
        char *filepath = argv[i + 2];
        fd[i] = open(filepath, O_RDONLY);
        if (fd[i] < 0) {
            char str[] = "Error open";
            perror(str);
            return EXIT_FAILURE;
        }
    }
    char *new_argv[argc];
    new_argv[0] = cat;
    char buf[argc - 2][40];

    for (int i = 0; i < argc - 2; ++i) {
        new_argv[i + 1] = buf[i];
        int ret = sprintf(new_argv[i + 1], "%d", fd[i]);
        if (ret < 0) {
            char str[] = "Error sprintf";
            perror str;
            return EXIT_FAILURE;
        }   
    }
    new_argv[argc - 1] = 0;

    pid_t pid = fork();
    if (pid == 0) {
        int ret = execvp(cat, new_argv);
        if (ret == -1) {
            char str[] = "Error exec";
            perror(str);
            return EXIT_FAILURE;
        }
    } else if (pid == -1) {
        char str[] = "Error fork";
        perror(str);
        return EXIT_FAILURE;
    } else {
        int status = 0;
        pid_t ret = waitpid(pid, &status, 0);
        if (ret == -1) {
            char str[] = "Error waitpid";
            perror(str);
            return EXIT_FAILURE;
        }
        for (int i = 0; i < argc - 2; ++i) {
            int ret2 = close(fd[i]);
            if (ret2 == -1) {
                char str[];
                perror(str);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}




    
    

