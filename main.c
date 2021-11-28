#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define buffer_len 10
#define buffer_len_2 64

// TODO
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
        c = getchar(); // read a character
        if (c == '\n') { // replace end of line with null
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;
        if (pos >= bl) { // reallocate if buffer is insufficient
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
    token = strtok(line, delim); // breaks the line into a series of tokens using the delim
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        if (pos >= bl) { // reallocate if buffer for pointers is insufficient
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
        if (execvp(args[0], args) == -1) { // child
            printf("%s\n", strerror(errno));
        }
        exit(0);
    } else if (pid < 0) {
        printf("%s\n", strerror(errno));
    } else { // parent
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
            pid2 = waitpid(pid, &status, WUNTRACED);
        }
    }
    return WEXITSTATUS(status);
}

int is_command(char **args) {
    char *cmd = args[0];

    if (strcmp(cmd, "exit") == 0) {
        return 1;
    }
    else if (strcmp(cmd, "cd") == 0) {
        return 1;
    }
    else return 0;
}

int exec_command(char **args) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        chdir(args[1]);
        }
    return 1;
}

int main(int argc, char **argv) {   
    while(1) {
        printf("MASH ~$ ");

        // read in line
        char *line = read_line();

        // parse commands from input
        char ** args = parse_line(line, " ");

        // execute commands
        if (is_command(args)) {
            exec_command(args);
        } else {
            start(args);
        }

        
        free(line);
        free(args);
    }
    return 0;
}