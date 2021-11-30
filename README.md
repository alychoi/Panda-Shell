## MASH (or Pandaland)
(**M**acy and **A**lyssa's **SH**ell)<br>
This project is an implementation of a shell in C.

Features:
- Multiple Pipes
- Changing Directories
- Exiting
- 

Bugs
* Redirection does not work properly (creates file but nothing in file)
* In multiple commands (“;”), order is switched

Function Headers
## Function List

```c
/* in main.h: */
int main();

/* in commands.h: */
char * read_line();
char ** parse_line(char *line, char *delim);
int redirection(char **args);
int pipe(char **args);
int start(char ** args);
```
