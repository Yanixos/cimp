#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED
#include <stdio.h>
#include "parser.h"
#define LINE_SIZE 254

extern int write_script(char* filename);
extern int load_script(char* filename);
extern int edit_script(char* filename);
extern int rename_script(char* filename,char* new);
extern int execute_script(char* filename);

#endif
