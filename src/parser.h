#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>

#define SIZE 1024
#define NUM_CMD 29
#define SEP " \t\r\n\a"

typedef struct _pixel
{
     int x;
     int y;
} pixel;

typedef struct _command
{
    char** files;
    char* color;
    int argc;
    int option;
    pixel* p1;
    pixel* p2;
} command;

int check_extension(char* );
//int batch_files(command*, char**, int );
int tokenize(char*, char** );
int parse(char*, command* );

int parse_open(char** args, command* cmd, int index);
int parse_save(char** args, command* cmd, int index);
int parse_export(char** args, command* cmd, int index);
int parse_set_bg(char** args, command* cmd, int index);
int parse_select(char** args, command* cmd, int index);
int parse_unselect(char** args, command* cmd, int index);
int parse_copy(char** args, command* cmd, int index);
int parse_cut(char** args, command* cmd, int index);
int parse_paste(char** args, command* cmd, int index);
int parse_symetric(char** args, command* cmd, int index);
int parse_rotate(char** args, command* cmd, int index);
int parse_scale(char** args, command* cmd, int index);
int parse_resize(char** args, command* cmd, int index);
int parse_fill(char** args, command* cmd, int index);
int parse_replace(char** args, command* cmd, int index);
int parse_negative(char** args, command* cmd, int index);
int parse_gray(char** args, command* cmd, int index);
int parse_black_white(char** args, command* cmd, int index);
int parse_brightness(char** args, command* cmd, int index);
int parse_contrast(char** args, command* cmd, int index);
int parse_write_script(char** args, command* cmd, int index);
int parse_load_script(char** args, command* cmd, int index);
int parse_edit_script(char** args, command* cmd, int index);
int parse_save_script(char** args, command* cmd, int index);
int parse_rename_script(char** args, command* cmd, int index);
int parse_execute_script(char** args, command* cmd, int index);
int parse_undo(char** args, command* cmd, int index);
int parse_redo(char** args, command* cmd, int index);
int parse_modify_bpc(char** args, command* cmd, int index);
int parse_art_effect(char** args, command* cmd, int index);

char *cmd_name[] =
{
    "open",
    "save",
    "export",
    "set_bg",
    "select",
    "unselect",
    "copy",
    "cut",
    "paste",
    "symetric",
    "rotate",
    "scale",
    "resize",
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
    "modify_bpc",
    "art_effect"
};

int (*parse_func[]) (char **, command*, int) =
{
    &parse_open,
    &parse_save,
    &parse_export,
    &parse_set_bg,
    &parse_select,
    &parse_unselect,
    &parse_copy,
    &parse_cut,
    &parse_paste,
    &parse_symetric,
    &parse_rotate,
    &parse_scale,
    &parse_resize,
    &parse_fill,
    &parse_replace,
    &parse_negative,
    &parse_gray,
    &parse_black_white,
    &parse_brightness,
    &parse_contrast,
    &parse_write_script,
    &parse_load_script,
    &parse_edit_script,
    &parse_save_script,
    &parse_rename_script,
    &parse_execute_script,
    &parse_undo,
    &parse_redo,
    &parse_modify_pbc,
    &parse_art_effect,
};

#endif
