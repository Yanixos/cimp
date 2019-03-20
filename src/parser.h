#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>

#define SIZE 1024
#define NUM_CMD 35
#define SEP " "

typedef struct _command
{
    char* name;
    int batch;
    char** files;
} command;

char *cmd_name[] =
{
    "load",  // load file.png // load -l file* // load -l file1.png file2.png
    "transfer",
    "read",
    "save",
    "export",
    "select",
    "show_selected",
    "delete_selected",
    "add_selected",
    "unselect",
    "copy",
    "cut",
    "paste",
    "symetric",
    "rotate",
    "zoom",
    "size",
    "fill",
    "replace",
    "negative",
    "gray",
    "black_white",
    "brightness",
    "contrast",
    "write_script",
    "load_script",
    "edit_script",
    "save_script",
    "rename_script",
    "execute_script",
    "undo",
    "redo",
    "transparency",
    "modify_pbc",
    "angle_rotate",
    "art_effect"
};

int check_extension(char* );
void batch_files(command*, char**, int );
int tokenize(char*, char** );
int parse_command(char*, command* );

#endif
