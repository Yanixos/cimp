#ifndef WIN_IMG_H
#define WIN_IMG_H


typedef struct node{ // structure d'un noeud de liste de fenetres

  int id;
  SDL_Window *w;
  SDL_Surface *img;
  char *name;

  struct node *next;
}node;

node * list;


//Interface : Fonctions globale

extern SDL_Surface * get_sf_by_id(int in);
extern SDL_Window * get_w_by_id(int in);
extern int get_id_name(char *name);
extern SDL_Surface *open_new(char *path);
extern SDL_Surface * open_old(char * path,int id_win);
extern int save_bmp_img(SDL_Surface *img,const char * path);
extern int save_png_img(SDL_Surface *img, const char *path);
//extern int save_jpg_img(SDL_Surface *img, const char *path);
//extern int save_jpeg_img(SDL_Surface *img, const char *path);
extern void reset_content(SDL_Window* window, short render);
extern void get_all_windows(SDL_Window** w, int* size);

//provisoire pour avancer puis one en discutera
extern node * find_node(int in);
extern void update_node(node* n,SDL_Window *w,SDL_Surface *t,char *name);
void close_window(int id_win);
extern SDL_Window * test_window();

#endif
