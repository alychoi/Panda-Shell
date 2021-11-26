#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define buffer_len 10
#define buffer_len_2 64

// Read a line at a time, parse the line to separate the command from its arguments. 
// It should then fork and exec the command. 
// The parent process should wait until the exec’d program exits and then it should read the next command.

// implement exit and cd on our own (use chdir())

// Read and separate multiple commands on one line with ;
// ex: ls -l ; echo hello should first run the command ls -l and then echo hello

// Implement simple redirection using > (redirecting stdout) and < (redirecting stdin)

// Implement simple pipes, that is: ls | wc would run ls and use the output from ls as the input for wc
// use popen()

char * read_line() {
    char *buffer = malloc(sizeof(char) * buffer_len);
    int bl = buffer_len;
    int c;
    int pos = 0;
    while (1) {
        c = getchar();
        if (c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;
        if (pos >= bl) {
            bl += buffer_len;
            buffer = realloc(buffer, bl);
            if (!buffer) {
                printf("%s\n", strerror(errno));
            }
        }
    }
    return buffer;
}

char ** parse_line(char *line, char *delim) {
    int bl = buffer_len_2;
    int pos = 0;
    char ** tokens = malloc(bl * sizeof(char *));
    char *token;
    token = strtok(line, delim);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        if (pos >= bl) {
            bl += buffer_len_2;
            tokens = realloc(tokens, bl * sizeof(char *));
            if (!tokens) {
                printf("%s\n", strerror(errno));
            }
        }
        token = strtok(NULL, delim);
    }
    tokens[pos] = NULL;
    return tokens;
}

int start(char ** args) {
    int pid = fork();
    int pid2;
    int status;
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            printf("%s\n", strerror(errno));
        }
        exit(0);
    } else if (pid < 0) {
        printf("%s\n", strerror(errno));
    } else {
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
            pid2 = waitpid(pid, &status, WUNTRACED);
        }
    }
    return WEXITSTATUS(status);
}

int main () {   
    while (1) {
        printf("Start shell\n");
        char *line = read_line();
        char ** args = parse_line(line, " ");
        start(args);
        free(line);
        free(args);
    }
    return 0;
}