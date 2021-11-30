// reads line from stdin 
char * read_line();

// parses line into list of strings with a delimeter of the user's choosing
char ** parse_line(char *line, char *delim);

// redirects stdout to a file, stdout to a file by appending, and stdin from a file
void redirect(char ** args);

// parses line by "|" and redirects stdout from one program to stdin of the next
void piping(char ** args, char * line);

// starts program; includes cd, exit, fork & exec commands
int start(char ** args, char * line);