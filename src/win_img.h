#ifndef WIN_IMG_H
#define WIN_IMG_H

#define POSX 0
#define POSY 0
#define SIZE_X 500
#define SIZE_Y 500

typedef struct node{ // structure d'un noeud de liste de fenetres

  int id;
  SDL_Window *w;
  SDL_Texture *img;
  struct node *next;
}node;

node * list=NULL;

//Interface : Fonctions globale

extern SDL_Texture * get_by_id(int in);
extern void open_new(char *path);


//Fonctions internes
//Fonctions standards sur les listes 

node * allocNode(SDL_Window *w,SDL_Texture *img);
node * next(node * n);
void addNode(node ** list, SDL_Window *w,SDL_Texture *img);
int length(struct node * l);
void freelist(node * l);

// Fonctions SDL
void sdl_init();
SDL_Surface* load_bmp_img(char *filename);
int save_bmp_img(SDL_Surface *img,const char * path);
SDL_Window * creat_window(char *title,int high,int width);
SDL_Renderer * window_renderer(SDL_Window *w);
SDL_Texture* display_img_text(SDL_Renderer *r,SDL_Surface *s);
void close_window(SDL_Window *w,SDL_Renderer *r);


#endif