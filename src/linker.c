#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
#include "script.h"
#include "win_img.h"
#include "image_effect.h"
#include "selection.h"
#include "selection_effects.h"

int (*call_func[]) (command*) =
{
    &call_open,
    &call_save,
    &call_set_bg,
    &call_select_rect,
    &call_select_free,
    &call_select_color,
    &call_unselect,
    &call_copy,
    &call_cut,
    &call_paste,
    &call_symetric,
    &call_rotate,
    &call_scale,
    &call_resize,
    &call_fill,
    &call_replace,
    &call_negative,
    &call_gray,
    &call_black_white,
    &call_brightness,
    &call_contrast,
    &call_blur,
    &call_pixel,
    &call_undo,
    &call_redo,
    &call_close,
    &call_write_script,
    &call_load_script,
    &call_edit_script,
    &call_rename_script,
    &call_execute_script
};

int call_command(command* cmd)
{
     return call_func[cmd->index](cmd);
}

int call_open(command* cmd)
{
     if ( cmd->argc == 2 )
     {
          open_new(cmd->files[0]);
          refresh_selection_list();
          return 0;
     }
     else
     {
          int id = get_id_name(cmd->files[1]);
          open_old(cmd->files[0],id);
          refresh_selection_list();
          return 0;
     }
}

int call_save(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     char *ext = strrchr(cmd->files[0], '.');
     if (strcasecmp(ext,".bmp"))
          return save_bmp(id,cmd->files[1]);
     else if (strcasecmp(ext,".png"))
          return save_png(id,cmd->files[1]);
     /*else if (strcasecmp(ext,".jpg"))
          return save_jpg_img(id,cmd->files[1]);
     else if (strcasecmp(ext,".jpeg"))
               return save_bmp_img(id,cmd->files[1]);*/
     else
          return -1;
}

int call_set_bg(command* cmd)
{
     fprintf(stdout,"TODO\n");
     return 0; //******************************** TO IMPLEMENT HERE
     //return set_bg(cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);
}

int call_select_rect(command* cmd)
{
     int id = get_id_name(cmd->files[0]);

     if (cmd->option < 10)
     {
          printf("option : %d\n",cmd->option);
          select_rect(id,cmd->option);
     }
     else
          select_rect_coord(id,cmd->option-10,cmd->pixels[0]->x,cmd->pixels[0]->y,cmd->pixels[1]->x,cmd->pixels[1]->y);

     return 0;
}

int call_select_free(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     printf("%d %d",id,cmd->option);
     select_free(id,cmd->option);
     printf("BACK\n");
     return 0;
}

int call_select_color(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     select_color(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b,cmd->value,cmd->option);
     return 0;
}

int call_unselect(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     deselect_all(id);
     return 0;
}

int call_copy(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     copy(id);
     return 0;
}

int call_cut(command* cmd)
{
     int r=255,g=255,b=255;
     int id = get_id_name(cmd->files[0]);
     if ( cmd->argc == 2 )
          cut(id,r,g,b);
     else
          cut(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);

     return 0;
}

int call_paste(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     paste(id,cmd->pixels[0]->x,cmd->pixels[0]->y);
     return 0;
}

int call_symetric(command* cmd)
{
     int id = get_id_name(cmd->files[0]);

     if ( cmd->value == 1)
          apply_vertical(id);
     else
          apply_horizontal(id);

     return 0;
}

int call_rotate(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_rotate(id,cmd->value);
     refresh_selection_list();
     return 0;
}

int call_scale(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     if ( cmd->option == 1 )
          apply_cut(id,cmd->pixels[0]->x,cmd->pixels[0]->y,cmd->pixels[1]->x,cmd->pixels[1]->y);
     else
          apply_enlarge(id,cmd->value,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);

     refresh_selection_list();
     return 0;
}

int call_resize(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_resize(id,cmd->pixels[0]->x,cmd->pixels[0]->y); // ce qui equivalent à resize(window_id , largeur , hauteur)
     refresh_selection_list();
     return 0;
}

int call_fill(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_fill(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);
     return 0;
}

int call_replace(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_replace_color(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b,cmd->colors[1]->r,cmd->colors[1]->g,cmd->colors[1]->b,cmd->value);
     return 0;
}

int call_negative(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_negative(id);
     return 0;
}

int call_gray(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_blur(id,cmd->value);
     return 0;
}

int call_black_white(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_blackwhite(id,cmd->value);
     return 0;
}

int call_brightness(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_brightness(id,cmd->value);
     return 0;
}

int call_contrast(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_contrast(id,cmd->value);
     return 0;
}

int call_blur(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_blur(id,cmd->value);
     return 0;
}

int call_pixel(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_pixel(id,cmd->value);
     return 0;
}

int call_undo(command* cmd)
{
     fprintf(stdout,"TODO\n");//***********************************
     //return undo(cmd->files[0]);
     return 0;
}

int call_redo(command* cmd)
{
     fprintf(stdout, "TODO\n");//****************************************
     //return redo(cmd->files[0]);
     return 0;
}

int call_close(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     close_window(id);
     refresh_selection_list();
     return 0;
}

int call_write_script(command* cmd)
{
     return write_script(cmd->files[0]);
}

int call_load_script(command* cmd)
{
     return load_script(cmd->files[0]);
}

int call_edit_script(command* cmd)
{
     return edit_script(cmd->files[0]);
}

int call_rename_script(command* cmd)
{
     return rename_script(cmd->files[0],cmd->files[1]);
}
int call_execute_script(command* cmd)
{
     return execute_script(cmd->files[0]);
}
