#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>

#define SIZE 1024
#define NUM_PARSE 36
#define SEP " \t\r\n\a"

#define die(e) do { fprintf(stderr, "%s(): failed\n", e); return -1; } while (0);

typedef struct _pixel
{
     int x;
     int y;
} pixel;

typedef struct
{
     int r;
     int g;
     int b;
} color;

typedef struct _command
{
     int argc;
     int index;
     int value;
     int option;
     char** files;
     pixel** pixels;
     color**  colors;

} command;

int BATCH_MODE;
extern char *cmd_name[];
extern int (*parse_func[]) (char **, command*);

extern int check_extension(char* );
extern int check_pixel(char* arg, pixel* p);
extern char* name_to_rgb(char* arg);
extern int check_color(char* arg, color* c);
extern int batch_files(int argc, char** args, char** list);
extern int batch_regex(char* reg_file, char** list);
extern int tokenize(char*, char** );
extern char** make_args(int , int , char* , char* , char **);
extern void add_cmd_to_history(char *,char *);
extern void init_graphics();


extern int parse_by_mode(char* , char** , command* );
extern int get_index(char*);
extern int parse_open(char** args, command* cmd);
extern int parse_save(char** args, command* cmd);
extern int parse_set_bg(char** args, command* cmd);
extern int parse_select_rect(char** args, command* cmd);
extern int parse_select_free(char** args, command* cmd);
extern int parse_select_color(char** args, command* cmd);
extern int parse_unselect(char** args, command* cmd);
extern int parse_copy(char** args, command* cmd);
extern int parse_cut(char** args, command* cmd);
extern int parse_paste(char** args, command* cmd);
extern int parse_symetric(char** args, command* cmd);
extern int parse_rotate(char** args, command* cmd);
extern int parse_scale(char** args, command* cmd);
extern int parse_resize(char** args, command* cmd);
extern int parse_fill(char** args, command* cmd);
extern int parse_replace(char** args, command* cmd);
extern int parse_negative(char** args, command* cmd);
extern int parse_gray(char** args, command* cmd);
extern int parse_black_white(char** args, command* cmd);
extern int parse_brightness(char** args, command* cmd);
extern int parse_contrast(char** args, command* cmd);
extern int parse_blur(char** args, command* cmd);
extern int parse_pixel(char** args, command* cmd);
extern int parse_undo(char** args, command* cmd);
extern int parse_redo(char** args, command* cmd);
extern int parse_close(char** args, command* cmd);
extern int parse_write_script(char** args, command* cmd);
extern int parse_load_script(char** args, command* cmd);
extern int parse_edit_script(char** args, command* cmd);
extern int parse_save_script(char** args, command* cmd);
extern int parse_rename_script(char** args, command* cmd);
extern int parse_execute_script(char** args, command* cmd);
extern int parse_cd(char** args, command* cmd);
extern int parse_ls(char** args, command* cmd);
extern int parse_batch_mode(char** args, command* cmd);
extern int parse_help(char** args, command* cmd);
extern int parse_exit(char** args, command* cmd);

#endif
