#include "SDL.h"
#include "win_img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_image.h>

#define POSX 0
#define POSY 0
#define SIZE_X 500
#define SIZE_Y 500

//Fonctions internes
//Fonctions standards sur les listes

node * allocNode(SDL_Window *w,SDL_Surface *img, char *name);
node * next(node * n);
void addNode(node ** list, SDL_Window *w,SDL_Surface *img,char *name);
void deleteNode(node **list,int id);
int length(struct node * l);
node * find_node(int in);
void update_node(node* n,SDL_Window *w,SDL_Surface *t,char *name);


// Fonctions SDL
void sdl_init();
SDL_Surface* load_img(char *filename);
SDL_Window * creat_window(char *title,int high,int width);
void display_img(SDL_Window *window,SDL_Surface *surface);
int save_bmp_img(SDL_Surface *img,const char * path);
int save_png_img(SDL_Surface *img, const char *path);
//int save_jpg_img(SDL_Surface *img, const char *path);
//int save_jpeg_img(SDL_Surface *img, const char *path);
void set_index_by_id(int id,int ind);
int get_index_by_id(int id);

/*--------------------------Modele de liste de fenetres---------------------------------------*/

/*Allocation avec initialisation d'un noeud */

node * allocNode(SDL_Window *w,SDL_Surface *img, char *name){

  node * n;
  n=malloc(sizeof(node));
  n->w=w;
  n->img=img;
  n->name=malloc(sizeof(name));
  strcpy(n->name,name);
  n->next=NULL;

  return n;
}

/* Modèle de listes */

node * next(node * n){ // Retourne l'@ du noeud suivant

  return (n->next);
}

int affectNext(node * n,node * s){//Retourne 1 si mise à jour de next avec s réussie

  return ((n->next=s)==s);
}

void addNode(node ** list, SDL_Window *w,SDL_Surface * img,char *name){ //ajoute un noeud d'une fenetre
  node *n=allocNode(w,img,name);
  affectNext(n,*list);
  if(*list !=NULL){
    n->id=(*list)->id +1;
  }
  else{
    n->id=1;
  }

  *list=n;
}

void deleteNode(node **list,int id){//supprime un noeud selon son ID

  if(list == NULL){
    fprintf(stderr, "Echec de Supression du noeud car liste vide\n");
    return;
  }
  node *p=*list;
  node *last=*list;

  while(p!=NULL){
    if(p->id==id){
      affectNext(last,next(p));
      if(p==*list)
        *list=next(p);
      SDL_FreeSurface(p->img);
      SDL_DestroyWindow(p->w);
      free(p->name);
      free(p);
   }
   else
   {
        last=p;
   }
    p=next(p);
  }
}
int length(struct node * l){ //Retourne la taille de la liste des fenetres

  if(l==NULL)return 0;
  else return (1+length(next(l)));
}

node * find_node(int in){ // Retourne l'@ noeud ayant id=in et NULL si introuvable
  if(list==NULL)
    return NULL;
  node * p=list;
  while(p!=NULL){
    if(p->id==in)
      return p;
    else
      p=next(p);
  }

  return NULL;
}

void update_node(node* n,SDL_Window *w,SDL_Surface *t,char *name){
  if (n==NULL) {
        fprintf(stderr,"\nEchec de Mise à jour du noeud \n");
        return;
  }
  n->w=w;
  n->img=t;
  n->name=realloc(n->name,sizeof(name));
  if(n->name == NULL){
    fprintf(stderr, "\nRealloc erreur\n");
    return;
  }
  strcpy(n->name,name);
}
SDL_Surface * get_sf_by_id(int in){ // Retourne img noeud ayant id=in NULL si introuvable

  node *n=find_node(in);
  if(n==NULL)
    return NULL;
  return n->img;
}

SDL_Window * get_w_by_id(int in){ // Retourne window noeud ayant id=in NULL si introuvable

  node *n=find_node(in);
  if(n==NULL)
    return NULL;
  return n->w;
}

int get_id_name(char *name){ //Retourne -1 si inexistant

  node *n=list;
  if(n==NULL)
    return -1;
  while(n!=NULL){
    if(!strcmp(n->name,name))
      return n->id;
    else
      n=next(n);
  }
  return -1;
}

void set_index_by_id(int id,int ind){
  node *n=find_node(id);
  if(n==NULL){
    fprintf(stderr, "Introuvable index undo redo\n");
    return;
  }
  n->index=ind;
}
int get_index_by_id(int id){
  node *n=find_node(id);
  if(n==NULL)
    return -1;
  return n->index;

}

/* Lébiration d'espace occupé par une lise */
void freelist(){
  int id=length(list);
  while(length(list)!=0){
    deleteNode(&list,id);
    id--;
  }
}
/*--------------------------------------------------------------------------------------------------*/

void sdl_init(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	/* gestion de l'echec de l'initialisation */
        fprintf(stderr,
                "\nImpossible d'initialiser SDL :  %s\n",
                SDL_GetError()
		);
        return;
    }
    //atexit(SDL_Quit);
}

SDL_Surface* load_img(char *filename){

     SDL_Surface *surface =IMG_Load(filename);
    if (surface == NULL) {
  fprintf(stderr,"/nImpossible de charger l'image : %s\n",
    SDL_GetError()
    );
  return NULL;
    }
    return surface;

}

int save_bmp_img(SDL_Surface *img, const char *path){

  return SDL_SaveBMP(img,path);
}

int save_bmp(int id,const char *path){

  SDL_Surface *s=get_sf_by_id(id);
  printf("%d  %s\n",id,path);
  if(s!=NULL)
    return save_bmp_img(s,path);
  else
    return -1;
}

int save_png_img(SDL_Surface *img, const char *path){

  return IMG_SavePNG(img,path);
}

int save_png(int id,const char *path){

  SDL_Surface *s=get_sf_by_id(id);
  if(s!=NULL)
    return save_png_img(s,path);
  else
    return -1;
}

/*
int save_jpg_img(SDL_Surface *img, const char *path){

  return IMG_SaveJPG(img,path);
}

int save_jpg(int id,const char *path){

  SDL_Surface *s=get_sf_by_id(id);
  if(s!=NULL)
    return save_jpg_img(s,path);
  else
    return -1;
}


int save_jpeg_img(SDL_Surface *img, const char *path){

  return IMG_SaveJPEG(img,path);
}

int save_jpeg(int id,const char *path){

  SDL_Surface *s=get_sf_by_id(id);
  if(s!=NULL)
    return save_jpeg_img(s,path);
  else
    return -1;
}

*/
SDL_Window * creat_window(char *title,int high,int width){

    SDL_Window *window=SDL_CreateWindow
	(title,                 /* titre */
	 SDL_WINDOWPOS_CENTERED,     /* placement en x */
	 SDL_WINDOWPOS_CENTERED,     /* placement en y */
	 width,                         /* largeur */
	 high,                         /* hauteur */
	 0                            /* drapeaux, cf, la doc.  */
	 );

    if (window == NULL) {
	fprintf(stderr,
		"\nCreation de la fenetre impossible : %s\n",
		SDL_GetError()
		);
        return NULL;
    }

    return window;
}

void display_img(SDL_Window *window,SDL_Surface *surface){

     SDL_Surface *winSurface = SDL_GetWindowSurface(window);
     //SDL_Rect dest_rect = (SDL_Rect) {POSX,POSY,surface->w,surface->h};
     SDL_BlitSurface(surface, NULL, winSurface, NULL);
     SDL_UpdateWindowSurface(window);
}

//affiche sur window le contenu de l'image qui lui correspond
void reset_content(SDL_Window* window, short render){
  node * p=list;
  while(p!=NULL && p->w != window){
      p=next(p);
  }
  if(p == NULL) return;
  SDL_Surface *winSurface = SDL_GetWindowSurface(window);
  SDL_BlitSurface(p->img, NULL, winSurface, NULL);
  if(render)
    SDL_UpdateWindowSurface(window);
}

void close_window(int id_win){
    deleteNode(&list,id_win);
}

SDL_Surface *open_new(char *path){

  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  //surface=load_bmp_img(path);
  surface=load_img(path);
  if (!surface) return NULL;
  char *nameWin=malloc(256);

  if(list != NULL)
    sprintf(nameWin,"Window_ID: %d file: %s",(list->id)+1,path);
  else
    sprintf(nameWin,"Window_ID:1 file: %s",path);

  window=creat_window(nameWin,surface->h,surface->w);
  display_img(window,surface);
  addNode(&list,window,surface,path);

  return surface;

}

SDL_Surface * open_old(char * path,int id_win){

  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  node *n=find_node(id_win);

  if (n==NULL) {
  /* noeud introuvable*/
        fprintf(stderr,"\nFentre avec Id:%d introuvable\n",id_win);
        return NULL;
  }
  window=n->w;
  //surface=load_bmp_img(path);
  surface=load_img(path);

  //Editer la taille de la fentre selon l'image
  SDL_SetWindowSize(window,surface->w,surface->h);
  display_img(window,surface);
  //Maj de la fentre
  update_node(n,window,surface,path);
  return surface;

}

void get_all_windows(SDL_Window** w, int* size){
  *size = length(list);
  if(*size == 0){
    *w = NULL;
    return;
  }
  *w = malloc(*size * sizeof(node));
  node* n = list;
  for(int i = 0;i < *size;i++){
    w[i] = n->w;
    n = n->next;
  }
}

void set_bg_color(int r,int g,int b){

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  background=SDL_MapRGB(format,(Uint8)r,(Uint8)g,(Uint8)b);
  SDL_FreeFormat(format);
}

void get_bg_color(int *r,int *g,int *b){

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
  SDL_GetRGB(background,format,(Uint8 *)r,(Uint8 *)g,(Uint8 *)b);
  SDL_FreeFormat(format);
}

void set_sf_by_id(int id,SDL_Surface *s){

  node *n=find_node(id);
  if(n==NULL){
    printf("\nID not found (set_sf_by_id)");
    return;
  }
  SDL_FreeSurface(n->img);
  n->img=s;
}


void set_w_by_id(int id,SDL_Window *w){

  node *n=find_node(id);
  if(n==NULL){
    printf("\nID not found (set_sf_by_id)");
    return;
  }

  n->w=w;
}

void set_name_by_id(int id,char *name){

  node *n=find_node(id);
  if(n==NULL){
    printf("\nID not found (set_sf_by_id)");
    return;
  }

  n->name=name;
}

SDL_Window * test_window(){
  return list->w;
}
