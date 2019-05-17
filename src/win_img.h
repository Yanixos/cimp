#ifndef WIN_IMG_H
#define WIN_IMG_H
#include "SDL.h"

typedef struct node{ // structure d'un noeud de liste de fenetres

  int id;
  SDL_Window *w;
  SDL_Surface *img;
  char *name;
  // undo redo index	
  int index; 
  struct node *next;
}node;

node * list;
Uint32 background;


//Interface : Fonctions globale
extern void sdl_init();
extern SDL_Surface * get_sf_by_id(int in);
extern void set_sf_by_id(int in,SDL_Surface *s);
extern SDL_Window * get_w_by_id(int in);
extern void set_w_by_id(int in,SDL_Window *w);
extern char * get_name_by_id(int id);
extern void set_name_by_id(int id,char *name);
extern int get_id_name(char *name); //Retourne -1 si inexistant
extern void set_index_by_id(int id,int ind);
extern int get_index_by_id(int id); //Retourne -1 si inéxistant
extern SDL_Window * creat_window(char *title,int high,int width);
extern SDL_Surface *open_new(char *path);
extern SDL_Surface * open_old(char * path,int id_win);
extern int save_bmp(int id,const char * path);
extern int save_png(int id,const char * path);
//extern int save_jpg(int id,const char * path);
//extern int save_jpeg(int id,const char * path);
extern void reset_content(SDL_Window* window, short render);
extern void get_all_windows(SDL_Window** w, int* size);
extern void close_window(int id_win);
extern void get_bg_color(int *r,int *g,int *b);
extern void set_bg_color(int r,int g, int b);
extern void freelist();
extern SDL_Window * test_window();

#endif
