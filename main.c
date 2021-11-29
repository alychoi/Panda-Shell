#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define buffer_len 10
#define buffer_len_2 64

int pos;

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
    pos = 0;
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

void redirection(char ** args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            int in = open(args[i+1], O_RDONLY, 0644);
            if (in < 0) {
                printf("%s\n", strerror(errno));
            }
            dup2(in, STDIN_FILENO); 
            //strcpy(args[i-1], args[i+1]);
            args[i] = NULL;
            //printf("char: %s\t%s\t%s\n", args[0], args[1], args[2]);
            //strcpy(args[i-1], args[i+1]);
            printf("char: %s\t%s\t%s\n", args[0], args[1], args[2]);
        }
        if (strcmp(args[i], ">") == 0) {
            int out = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (out < 0) {
                printf("%s\n", strerror(errno));
            }
            dup2(out, STDOUT_FILENO); 
            args[i] = NULL;
        }
        i++;
    }
}

// void piping(char ** args) { 
//     int i = 0;
//     while (args[i] != NULL) {
//         if 
//         FILE *f = popen(args[i], "c");
//         int duplicate = dup(STDIN_FILENO); // redirect stdout to stdin of second
//         dup2(fileno(f), STDOUT_FILENO);
//         //char *args[i+1] = args[i+1];
//         // execvp(args[i+1], args);
//         // dup2(duplicate, STDIN_FILENO);
//         // }
//         i++;
//     }
// }

int start(char ** args) {
    // exit
    if (strcmp(args[0], "exit") == 0) {
        printf("exiting...\n");
        exit(0);
    }
    // cd
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            printf("Please use the correct format\n");
        }
        chdir(args[1]);
        return 0;
    }
    int pid = fork();
    int pid2;
    int status;
    if (pid == 0) { // child
        int backup_sdout = dup(STDOUT_FILENO);
        int backup_sdin = dup(STDIN_FILENO);
        redirection(args);
        //piping(args);
        //printf("char: %s\t%s\t%s\n", args[0], args[1], args[2]);
        if (execvp(args[0], args) == -1) {
            printf("%s\n", strerror(errno));
        }
        dup2(backup_sdout, STDOUT_FILENO);
        dup2(backup_sdin, STDIN_FILENO);
        exit(0);
    } else if (pid < 0) { // error forking
        printf("%s\n", strerror(errno));
    } else { // parent
        wait(&status);
    }
    return WEXITSTATUS(status);
}

int main () {   
    printf("Start shell\n");
    while (1) {
        char *line = read_line();
        char ** args = parse_line(line, ";");
        int i;
        char ** args1;
        char ** args2;
        //printf("%d\n",pos);
        int p = pos;
        int p2;
        for (i = 0; i < p; i++) {
            //args2 = parse_line(args[i], "|");
            args1 = parse_line(args[i], " ");
            start(args1);
            //start(args2);
            free(args1);
            //free(args2);
        }
        // ls -l;echo hello;echo bye;echo hi
        // start(args);
        free(line);
        free(args);
    }
    return 0;
}