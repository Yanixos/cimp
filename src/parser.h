#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <stdio.h>

#define SIZE 1024                                                               // nombre maximal de fichier a traité en mode batch
#define NUM_PARSE 36                                                            // nombre de commande
#define SEP " \t\r\n\a"                                                         // le separateur de commande

typedef struct _pixel                                                           // structure qui définit une position dun pixel : (x,y)
{
     int x;
     int y;
} pixel;

typedef struct                                                                  // structure qui définit une color d'un pixel : (r,g,b)
{
     int r;
     int g;
     int b;
} color;

typedef struct _command                                                         // structure générique d'une commande
{
     int argc;                                                                  // nombre d'argument
     int index;                                                                 // index de la commande dans le tableau des commandes
     int value;                                                                 // valeur de radius ou d'un mode
     int option;                                                                // valeur d'option
     char** files;                                                              // les fichiers auquels appliqué la commande
     pixel** pixels;                                                            // les pixels de l'image auquels appliqué la commande
     color**  colors;                                                           // les couleur qui identifient les pixels

} command;

int BATCH_MODE;

extern char *cmd_name[];                                                        // la liste des noms des commandes
extern int (*parse_func[]) (char **, command*);                                 // la liste des fonctions des commandes

//void init_graphics();

extern int parse_by_mode(char* , char** , command* ,int );                      // le parser generique selon le batch mode
int get_index(char*);                                                           // retourne l'index d'une commande dans la table
int parse_open(char** args, command* cmd);                                      // parse_X ou X est le nom d'une commande : sert à parser une commande d'une façon unique
int parse_save(char** args, command* cmd);
int parse_set_bg(char** args, command* cmd);
int parse_select_rect(char** args, command* cmd);
int parse_select_free(char** args, command* cmd);
int parse_select_color(char** args, command* cmd);
int parse_unselect(char** args, command* cmd);
int parse_copy(char** args, command* cmd);
int parse_cut(char** args, command* cmd);
int parse_paste(char** args, command* cmd);
int parse_symetric(char** args, command* cmd);
int parse_rotate(char** args, command* cmd);
int parse_scale(char** args, command* cmd);
int parse_resize(char** args, command* cmd);
int parse_fill(char** args, command* cmd);
int parse_replace(char** args, command* cmd);
int parse_negative(char** args, command* cmd);
int parse_gray(char** args, command* cmd);
int parse_black_white(char** args, command* cmd);
int parse_brightness(char** args, command* cmd);
int parse_contrast(char** args, command* cmd);
int parse_blur(char** args, command* cmd);
int parse_pixel(char** args, command* cmd);
int parse_undo(char** args, command* cmd);
int parse_redo(char** args, command* cmd);
int parse_close(char** args, command* cmd);
int parse_write_script(char** args, command* cmd);
int parse_load_script(char** args, command* cmd);
int parse_edit_script(char** args, command* cmd);
int parse_save_script(char** args, command* cmd);
int parse_rename_script(char** args, command* cmd);
int parse_execute_script(char** args, command* cmd);
int parse_cd(char** args, command* cmd);
int parse_ls(char** args, command* cmd);
int parse_batch_mode(char** args, command* cmd);
int parse_help(char** args, command* cmd);
int parse_exit(char** args, command* cmd);

#endif
