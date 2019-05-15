#ifndef LINKER_H_INCLUDED
#define LINKER_H_INCLUDED
#include <stdio.h>
#include "parser.h"


extern int (*call_func[]) (command*);

extern int call_command(command* cmd);
extern int call_open(command* cmd);
extern int call_save(command* cmd);
extern int call_export(command* cmd);
extern int call_set_bg(command* cmd);
extern int call_select_rect(command* cmd);
extern int call_select_free(command* cmd);
extern int call_select_color(command* cmd);
extern int call_unselect(command* cmd);
extern int call_copy(command* cmd);
extern int call_cut(command* cmd);
extern int call_paste(command* cmd);
extern int call_symetric(command* cmd);
extern int call_rotate(command* cmd);
extern int call_scale(command* cmd);
extern int call_resize(command* cmd);
extern int call_fill(command* cmd);
extern int call_replace(command* cmd);
extern int call_negative(command* cmd);
extern int call_gray(command* cmd);
extern int call_black_white(command* cmd);
extern int call_brightness(command* cmd);
extern int call_contrast(command* cmd);
extern int call_blur(command* cmd);
extern int call_pixel(command* cmd);
extern int call_undo(command* cmd);
extern int call_redo(command* cmd);
extern int call_close(command* cmd);
extern int call_write_script(command* cmd);
extern int call_load_script(command* cmd);
extern int call_edit_script(command* cmd);
extern int call_rename_script(command* cmd);
extern int call_execute_script(command* cmd);

#endif
