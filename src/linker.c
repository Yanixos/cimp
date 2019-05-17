#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
#include "script.h"
#include "win_img.h"
#include "image_effect.h"
#include "selection.h"
#include "selection_effects.h"

int init = 0;

int (*call_func[]) (command*) =                                                 // la liste des fonctions qui appellent les autres fonctions pour executer le commande
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

void delete_cimpfiles(char* file)                                               // sert à supprimer l'historique d'une image
{
     char dest[254];
     sprintf(dest,"%s/.%s_cimphistory",getenv("HOME"),file);                    // les fichiers historique sont au repertoire HOME
     remove(dest);

     char tmp[254];
     sprintf(tmp,"%s_cimpbackup",file);
     remove(tmp);                                                               // supprimer le fichier de backup
}

int call_command(command* cmd)
{
     if ( ! init )
     {
          init = 1;
          sdl_init();
     }
     return call_func[cmd->index](cmd);                                         // appeler la fonction selon son index
}

int call_open(command* cmd)
{

     char temp[254];
     sprintf(temp,"cp %s %s_cimpbackup",cmd->files[0],cmd->files[0]);
     system(temp);

     //if ( ! exist_file(cmd->files[0]) )                                         // si le fichier n'est pas déja chargé
     //{
          if ( cmd->argc == 2 )                                                 // ouverture dans une nouvelle fenetre
          {

               open_new(cmd->files[0]);
               int id = get_id_name(cmd->files[0]);
               set_index_by_id(id,0);
               refresh_selection_list();
               return 0;
          }
          else                                                                  // ouverture dans une fenetre existante
          {
               int id = get_id_name(cmd->files[1]);                             // recuperer l'id de l'ancienne fenetre
               open_old(cmd->files[0],id);                                      // ouvrir l'image dans cette derniere
               set_index_by_id(id,0);
               refresh_selection_list();                                        // rafraishir la liste des fenetre
               return 0;
          }
     //}
     fprintf(stderr, "%s: already loaded\n",cmd->files[0] );
     return -1;
}

int call_save(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     char *ext = strrchr(cmd->files[1], '.');                                   // recuperer l'extension du fichier a sauvegarder
     if (!strcasecmp(ext,".bmp"))                                               // cas fichier bmp
          return save_bmp(id,cmd->files[1]);
     else if (!strcasecmp(ext,".png"))                                          // cas fichier jpg
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
     set_bg_color(cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);
     return 0;
}

int call_select_rect(command* cmd)
{
     int id = get_id_name(cmd->files[0]);

     if (cmd->option < 10)                                                      // cas selection avec main
          select_rect(id,cmd->option);
     else                                                                       // cas selection avec pixels
          select_rect_coord(id,cmd->option-10,cmd->pixels[0]->x,cmd->pixels[0]->y,cmd->pixels[1]->x,cmd->pixels[1]->y);

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_select_free(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     select_free(id,cmd->option);                                               // selection libre

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_select_color(command* cmd)
{
     int id = get_id_name(cmd->files[0]);                                       // selection par couleur
     select_color(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b,cmd->value,cmd->option);

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_unselect(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     deselect_all(id);                                                          // deselection

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_copy(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     copy(id);                                                                  // copier
     return 0;
}

int call_cut(command* cmd)
{
     int r, g, b;
     get_bg_color(&r,&g,&b);
     int id = get_id_name(cmd->files[0]);
     if ( cmd->argc == 2 )
          cut(id,r,g,b);                                                        // cas couper sans couleur d'arriere plan
     else
          cut(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);        // cas couper avec couleur d'arriere plan

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_paste(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     paste(id,cmd->pixels[0]->x,cmd->pixels[0]->y);                             // coller

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_symetric(command* cmd)
{
     int id = get_id_name(cmd->files[0]);

     if ( cmd->value == 1)
          apply_vertical(id);                                                   // symetrie vertical
     else
          apply_horizontal(id);                                                 // symetrie horizenal

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_rotate(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_rotate(id,cmd->value);                                               // rotation

     refresh_selection_list();
     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_scale(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     if ( cmd->option == 1 )
          apply_cut(id,cmd->pixels[0]->x,cmd->pixels[0]->y,cmd->pixels[1]->x,cmd->pixels[1]->y);    // scale avec coupage des bordure
     else
          apply_enlarge(id,cmd->value,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);       // scale avec zoom

     refresh_selection_list();
     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_resize(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_resize(id,cmd->pixels[0]->x,cmd->pixels[0]->y);                      // redimensionner

     refresh_selection_list();
     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_fill(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_fill(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b);      // remplissage avec une couleur donnée

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_replace(command* cmd)
{
     int id = get_id_name(cmd->files[0]);                                       // remplacer une couleur par une autre
     apply_replace_color(id,cmd->colors[0]->r,cmd->colors[0]->g,cmd->colors[0]->b,cmd->colors[1]->r,cmd->colors[1]->g,cmd->colors[1]->b,cmd->value);

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_negative(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_negative(id);                                                        // appliquer le mode negative

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_gray(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_grayscale(id);                                                       // appliquer le mode graysale

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_black_white(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_blackwhite(id,cmd->value);                                           // appliquer le mode noir et blanc

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_brightness(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_brightness(id,cmd->value);                                           // appliquer le mode luminosité

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_contrast(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_contrast(id,cmd->value);                                             // appliquer le mode de ontrast

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_blur(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_blur(id,cmd->value);                                                 // appliquer le mode blur

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_pixel(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     apply_pixel(id,cmd->value);                                                // appliquer le mode pixelerization

     reset_content(get_w_by_id(id), 0);
     draw_selected_pixels(id, 1);

     return 0;
}

int call_undo(command* cmd)
{
     return undo_redo(cmd->files[0],0);
}

int call_redo(command* cmd)
{
     return undo_redo(cmd->files[0],1);
}

int call_close(command* cmd)
{
     int id = get_id_name(cmd->files[0]);
     close_window(id);                                                          // fermer une fenetre
     refresh_selection_list();
     delete_cimpfiles(cmd->files[0]);
     return 0;
}

int call_write_script(command* cmd)
{
     return write_script(cmd->files[0]);                                        // ecriture d'une script cimp
}

int call_load_script(command* cmd)
{
     return load_script(cmd->files[0]);                                         // chargement d'une script cimp
}

int call_edit_script(command* cmd)
{
     return edit_script(cmd->files[0]);                                         // modification d'une script cimp
}

int call_rename_script(command* cmd)
{
     return rename_script(cmd->files[0],cmd->files[1]);                         // renommage d'une script cimp
}
int call_execute_script(command* cmd)
{
     return execute_script(cmd->files[0]);                                      // execution d'une script cimp
}
