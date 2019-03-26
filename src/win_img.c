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

node * allocNode(SDL_Window *w,SDL_Surface *img);
node * next(node * n);
void addNode(node ** list, SDL_Window *w,SDL_Surface *img);
void deleteNode(node **list,int id);
int length(struct node * l);
void freelist(node * l);

// Fonctions SDL
void sdl_init();
SDL_Surface* load_bmp_img(char *filename);
SDL_Surface* load_img(char *filename);
SDL_Window * creat_window(char *title,int high,int width);
void display_img(SDL_Window *window,SDL_Surface *surface);

int main(int argc,char *argv[]){

    
    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;
    
    sdl_init();
    open_new("../img/test1.bmp");
    open_new("../img/test.bmp");
    
    printf("\nTaille de liste %d",length(list));
    open_old("../img/test.bmp",2);

    save_png_img(get_sf_by_id(1),"../img/out.png");
    
    int rep=1;
    do {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
	    
	        switch (e.type) {
		
	          case SDL_QUIT:
		          printf("fermeture de la fenetre.\n"); 
		          rep=0;
		          break;
		
	          case SDL_MOUSEMOTION :
		          printf("deplacement : %d %d\n", 
		           e.motion.x, 
		           e.motion.y);
		          break;
          }
        }
    } while (rep != 0);
    
    close_window(1);
    close_window(2);

    return 0;
}


/*--------------------------Modele de liste de fenetres---------------------------------------*/

/*Allocation avec initialisation d'un noeud */

node * allocNode(SDL_Window *w,SDL_Surface *img){

  node * n;
  n=malloc(sizeof(node));
  n->w=w;
  n->img=img;
  n->next=NULL;
  printf("%p\n",n );
  return n;
}

/* Modèle de listes */

node * next(node * n){ // Retourne l'@ du noeud suivant

  return (n->next);
}

int affectNext(node * n,node * s){//Retourne 1 si mise à jour de next avec s réussie

  return ((n->next=s)==s);
}

void addNode(node ** list, SDL_Window *w,SDL_Surface * img){ //ajoute un noeud d'une fenetre
  node *n=allocNode(w,img);
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
    exit(1);
  }
  node *p=*list;
  node *last=*list;
  
  while(p!=NULL){
    if(p->id==id){
      affectNext(last,next(p));
      if(p==*list)
        *list=next(p);
      free(p);
   }
   else
    last=p;
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
void update_node(node* n,SDL_Window *w,SDL_Surface *t){
  if (n==NULL) {
        fprintf(stderr,"\nEchec de Mise à jour du noeud \n");
        exit(1);
  }
  n->w=w;
  n->img=t;
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

/* Lébiration d'espace occupé par une lise */
void freelist(node * l){

  while(l!=NULL){

    free(l);
    l=next(l);
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
        exit(1);
    }
    //atexit(SDL_Quit);
}

SDL_Surface* load_bmp_img(char *filename){
   
     SDL_Surface *surface = SDL_LoadBMP(filename);
    if (surface == NULL) {
	fprintf(stderr,"/nImpossible de charger l'image : %s\n", 
		SDL_GetError()
		);
	exit(1);
    }
    return surface;

}

SDL_Surface* load_img(char *filename){
   
     SDL_Surface *surface =IMG_Load(filename);
    if (surface == NULL) {
  fprintf(stderr,"/nImpossible de charger l'image : %s\n", 
    SDL_GetError()
    );
  exit(1);
    }
    return surface;

}

int save_bmp_img(SDL_Surface *img, const char *path){

  return SDL_SaveBMP(img,path);
}

int save_png_img(SDL_Surface *img, const char *path){

  return IMG_SavePNG(img,path);
}
/*
int save_jpg_img(SDL_Surface *img, const char *path){

  return IMG_SaveJPG(img,path);
}


int save_jpeg_img(SDL_Surface *img, const char *path){

  return IMG_SaveJPEG(img,path);
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
        exit(1);
    }

    return window;
}

void display_img(SDL_Window *window,SDL_Surface *surface){
    
     SDL_Surface *winSurface = SDL_GetWindowSurface(window);
     SDL_Rect dest_rect = (SDL_Rect) {POSX,POSY,surface->w,surface->h};
     SDL_BlitSurface(surface, NULL, winSurface, NULL);
     SDL_UpdateWindowSurface(window);
}

void close_window(int id_win){
    node *n=find_node(id_win);
    SDL_FreeSurface(n->img);
    SDL_DestroyWindow(n->w);
    deleteNode(&list,id_win);
}

SDL_Surface *open_new(char *path){ // que des bmp

  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  //surface=load_bmp_img(path);
  surface=load_img(path);
  char *nameWin=malloc(256);
  
  if(list != NULL)
    sprintf(nameWin,"Window_ID: %d file: %s",(list->id)+1,path);
  else
    sprintf(nameWin,"Window_ID:1 file: %s",path);

  window=creat_window(nameWin,surface->h,surface->w);
  display_img(window,surface);
  addNode(&list,window,surface);

  return surface;

}

SDL_Surface * open_old(char * path,int id_win){

  SDL_Window *window = NULL;
  SDL_Surface *surface = NULL;
  node *n=find_node(id_win);

  if (n==NULL) {
  /* noeud introuvable*/
        fprintf(stderr,"\nFentre avec Id:%d introuvable\n",id_win);
        exit(1);
  }
  window=n->w;
  //surface=load_bmp_img(path);
  surface=load_img(path);
  
  //Editer la taille de la fentre selon l'image
  SDL_SetWindowSize(window,surface->w,surface->h);
  display_img(window,surface);
  //Maj de la fentre
  update_node(n,window,surface);
  return surface;

}

SDL_Window * test_window(){
  return list->w;
}

