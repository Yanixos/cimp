#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>

#define BUFFSIZE 1024
#define NUM_CMD 39
#define SEP " \t\r\n\a"

typedef struct _command
{
    char* name;
    int batch;
    char** filename; 
} command;

void tokenize(char* ,char **)
int parse_command(char*, char** );

#endif 
