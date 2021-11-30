#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "commands.h"

int pos;

int main () {   
    char panda[] = "\U0001F43C";
    char bamboo[] = "\U0001F38B";
    char intro[] = ",------.   ,---.  ,--.  ,--.,------.    ,---.  ,--.     ,---.  ,--.  ,--.,------.\n|  .--. ' /  O  \\ |  ,'.|  ||  .-.  \\  /  O  \\ |  |    /  O  \\ |  ,'.|  ||  .-.  \\\n|  '--' ||  .-.  ||  |' '  ||  |  \\  :|  .-.  ||  |   |  .-.  ||  |' '  ||  |  \\  :\n|  | --' |  | |  ||  | `   ||  '--'  /|  | |  ||  '--.|  | |  ||  | `   ||  '--'  /\n`--'     `--' `--'`--'  `--'`-------' `--' `--'`-----'`--' `--'`--'  `--'`-------'\n";
    printf("%s %s Entering Shell %s %s \n", bamboo, panda, panda, bamboo);
    printf("%s\n", intro);
    while (1) {
        char path[1000];
		printf("%s%s$: ", panda, getcwd(path, 1000));
        char *line = read_line();
        char ** args = parse_line(line, ";");
        int i;
        char ** args1;
        char ** args2;
        //printf("%d\n",pos);
        int p = pos;
        int p2;
        for (i = 0; i < p; i++) {
            char *cpy = malloc(strlen(args[i]) + 1);
            strcpy(cpy, args[i]);
            args1 = parse_line(cpy, " ");
            start(args1, args[i]);
            free(args1);
        }
        free(line);
        free(args);
    }
    return 0;
}