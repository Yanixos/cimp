#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void tokenize(char* ,char **)
int parse_command(char*, char** );

int main(int argc, char **argv)
{

	return 0;
}

void tokenize(char* line,char **args)
{
    int i = 0;
    for (char *token = strtok(line,SEP); token != NULL; token = strtok(NULL, SEP))
            args[i++] = strdup(token);
    args[i] = NULL;
}

int parse_command(char* line, command cmd)
{
    tokinize(line,args);
    for ( int i=0; i < CMD_NUM; i++ ) 
}
