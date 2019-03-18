#ifndef WIN_IMG_H
#define WIN_IMG_H


typedef struct node{ // structure d'un noeud de liste de fenetres

  int id;
  SDL_Window *w;
  SDL_Texture *img;
  struct node *next;
} node;

node * list;


//Interface : Fonctions globale

extern SDL_Texture * get_txt_by_id(int in);
extern SDL_Window * get_w_by_id(int in);
extern SDL_Surface *open_new(char *path);
extern SDL_Surface * open_old(char * path,int id_win);

//provisoire pour avancer puis one en discutera
extern node * find_node(int in);
extern void update_node(node* n,SDL_Window *w,SDL_Texture *t);
extern SDL_Window * test_window();

#endif
