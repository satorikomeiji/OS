#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <fcntl.h>

#define DIGITS_NUMBER 40
int main(int argc, char **argv) {
    char *delim = argv[1];
    char *cat = argv[2];
    int fds[argc - 3];
    for (int i = 0; i < argc - 3; ++i) {
        char *filepath = argv[i + 3];
        fds[i] = open(filepath, O_RDONLY);
        if (fds[i] < 0) {
            char str[] = "Error open";
            perror(str);
            return EXIT_FAILURE;
        }
    }
    char *n_argv[argc];
    n_argv[0] = cat;
    n_argv[1] = delim;
    
    char buf[argc - 3][DIGITS_NUMBER];

    for (int i = 0; i < argc - 3; ++i) {
        n_argv[i + 2] = buf[i];
        int ret = sprintf(n_argv[i + 2], "%d", fds[i]);
        if (ret < 0) {
            char str[] = "Error sprintf";
            perror (str);
            return EXIT_FAILURE;
        }   
    }
    n_argv[argc - 1] = 0;

    pid_t pid = fork();
    if (pid == 0) {
        int ret = execvp(cat, n_argv);
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
        for (int i = 0; i < argc - 3; ++i) {
            int ret2 = close(fds[i]);
            if (ret2 == -1) {
                char str[] = "Error close";
                perror(str);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}
