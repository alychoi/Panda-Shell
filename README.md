## PANDALAND
Alyssa Choi (Period 4)<br>
This project is an implementation of a shell in C.

## Shell Features
* read line from stdin
* parse line into list of strings with a delimeter of the user's choosing
* redirection (>, >>, <)
* piping
* start processes 
* read and separate multiple commands with ;

## Restrictions
* multiple commands should be separated with ";" (no adjacent spaces)
* redirection should be used with one space on adjacent sides of the symbol (i.e. " < ")
* piping should be used with one space on adjacent sides of the symbol (i.e. " | ")

## Function Headers
// reads line from stdin<br>
char * read_line();

// parses line into list of strings with a delimeter of the user's choosing<br>
char ** parse_line(char *line, char *delim);

// redirects stdout to a file, stdout to a file by appending, and stdin from a file<br>
void redirect(char ** args);

// parses line by "|" and redirects stdout from one program to stdin of the next<br>
void piping(char ** args, char * line);

// starts program; includes cd, exit, fork & exec commands<br>
int start(char ** args, char * line);
