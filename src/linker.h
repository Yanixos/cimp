#ifndef LINKER_H_INCLUDED
#define LINKER_H_INCLUDED
#include <stdio.h>
#include "parser.h"


extern int (*call_func[]) (command*);                                           // la liste des commandes appelantes

extern int call_command(command* cmd, char* line, int flag);                    // la fonction generique
int call_open(command* cmd);                                                    // call_X : ou X le nom d'une commande, fait appel à la fonction d'une autre module pour etre executé
int call_save(command* cmd);
int call_export(command* cmd);
int call_set_bg(command* cmd);
int call_select_rect(command* cmd);
int call_select_free(command* cmd);
int call_select_color(command* cmd);
int call_unselect(command* cmd);
int call_copy(command* cmd);
int call_cut(command* cmd);
int call_paste(command* cmd);
int call_symetric(command* cmd);
int call_rotate(command* cmd);
int call_scale(command* cmd);
int call_resize(command* cmd);
int call_fill(command* cmd);
int call_replace(command* cmd);
int call_negative(command* cmd);
int call_gray(command* cmd);
int call_black_white(command* cmd);
int call_brightness(command* cmd);
int call_contrast(command* cmd);
int call_blur(command* cmd);
int call_pixel(command* cmd);
int call_undo(command* cmd);
int call_redo(command* cmd);
int call_close(command* cmd);
int call_write_script(command* cmd);
int call_load_script(command* cmd);
int call_edit_script(command* cmd);
int call_rename_script(command* cmd);
int call_execute_script(command* cmd);

#endif
