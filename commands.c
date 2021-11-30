#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "commands.h"

#define buffer_len 10
#define buffer_len_2 64

extern int pos;

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

void redirect(char ** args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            int out = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (out < 0) {
                printf("%s\n", strerror(errno));
            }
            dup2(out, STDOUT_FILENO); 
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            int out = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (out < 0) {
                printf("%s\n", strerror(errno));
            }
            dup2(out, STDOUT_FILENO); 
            args[i] = NULL;
        } else if (strcmp(args[i], "<") == 0) {
            int in = open(args[i+1], O_RDONLY, 0644);
            if (in < 0) {
                printf("%s\n", strerror(errno));
            }
            dup2(in, STDIN_FILENO); 
            args[i] = NULL;
        }
        i++;
    }
}

void piping(char ** args, char * line) {
    char ** args1 = parse_line(line, "|");
    FILE *read = popen(args1[0], "r");
    FILE *write = popen(args1[1], "w");
    //int fd = fileno(read);
    if (read < 0) {
        printf("%s\n", strerror(errno));
    }
    char buffer[1000];
    while (fgets(buffer, 1000, read)) {
        fputs(buffer, write);
    }
    //dup2(fd, STDIN_FILENO);
    pclose(read);
    pclose(write);
}

int start(char ** args, char * line) {
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
    if (pid == 0) { // child
        int backup_sdout = dup(STDOUT_FILENO);
        int backup_sdin = dup(STDIN_FILENO);
        redirect(args);
        int i = 0;
        int bool = 0;
        while (args[i] != NULL) {
            if (strcmp(args[i], "|") == 0) {
                bool = 1;
            }
            i++;
        }
        if (bool == 1) {
            piping(args, line);
        } else {
            if (execvp(args[0], args) == -1) {
                printf("%s\n", strerror(errno));
            }
        }
        dup2(backup_sdout, STDOUT_FILENO);
        dup2(backup_sdin, STDIN_FILENO);
    } else if (pid < 0) { // error forking
        printf("%s\n", strerror(errno));
    } else { // parent
        int status;
        wait(&status);
    }
    return 1;
}