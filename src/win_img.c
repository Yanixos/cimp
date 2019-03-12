#include "SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>

#define POSX 0
#define POSY 0
#define SIZE_X 500
#define SIZE_Y 500

typedef struct node{ // structure d'un noeud de liste de fenetres

  SDL_Window *w;
  SDL_Texture *img;
  struct node *next;
}node ;

node * list=NULL;

//Fonctions standards sur les listes 

node * allocNode(SDL_Window *w,SDL_Texture *img);
node * next(node * n);
void addNode(node * list, SDL_Window *w,SDL_Texture *img);
int length(struct node * l);
node * indexx(node *l,int index);
void freelist(node * l);

// Fonctions SDL
void sdl_init();
SDL_Surface* load_bmp_img(char *filename);
SDL_Window * creat_window(char *title,int high,int width);
SDL_Renderer * window_renderer(SDL_Window *w);
SDL_Texture* display_img_text(SDL_Renderer *r,SDL_Surface *s);
void close_window(SDL_Window *w,SDL_Renderer *r);




int main(int argc,char *argv[]){

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture=NULL;
    SDL_Window *window1 = NULL;
    SDL_Renderer *renderer1 = NULL;
    SDL_Surface *surface1 = NULL;
    SDL_Texture *texture1=NULL;

    int width =SIZE_X, height =SIZE_Y;
    
    sdl_init();
    
    window=creat_window("new window",height,width);
    renderer=window_renderer(window);
    surface=load_bmp_img("../img/test.bmp");
    texture=display_img_text(renderer,surface);
    SDL_FreeSurface(surface);// n'est pas utile car changement possible
    addNode(list,window,texture);
    
    window1=creat_window("new window",1000,1000);
    renderer1=window_renderer(window1);
    surface1=load_bmp_img("../img/test1.bmp");
    texture1=display_img_text(renderer1,surface1);
    SDL_FreeSurface(surface1);// n'est pas utile car changement possible
    addNode(list,window1,texture1);
    
    int rep=1;
    //close_window(window,renderer);
    //scanf("%d",&rep);
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
    
    close_window(window,renderer);
    close_window(window1,renderer1);


    return 0;
}

/*--------------------------Modele de liste de fenetres---------------------------------------*/

/*Allocation avec initialisation d'un noeud */

node * allocNode(SDL_Window *w,SDL_Texture *img){

  node * n;
  n=malloc(sizeof(node));
  n->w=w;
  n->img=img;
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

void addNode(node * list, SDL_Window *w,SDL_Texture *img){ //ajoute un noeud d'une fenetre
  node *n=allocNode(w,img);
  if(list !=NULL) affectNext(list,n);
  list=n;
}

int length(struct node * l){ //Retourne la taille de la liste des fenetres

  if(l==NULL)return 0;
  else return (1+length(next(l)));
}

node * indexx(node *l,int index){ // Retourne l'@ du indice eme noeud

  node * p=l;
  int i=0;
  if((index<0 || index>=length(l))|| l==NULL)return NULL;
  else {

    while(i<index){p=next(p);i++;}
    return p;
  }
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

SDL_Renderer * window_renderer(SDL_Window *w){
    SDL_Renderer * renderer = SDL_CreateRenderer(w, -1, 0);
    /* gestion de l'echec de la creation du renderer */
    if (renderer == NULL) {
	fprintf(stderr,
		"\nCreation du renderer impossible : %s\n", 
		SDL_GetError()
		);
        exit(1);
    }

    return renderer;
}

SDL_Texture* display_img_text(SDL_Renderer *renderer,SDL_Surface *surface){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w,h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dest_rect = (SDL_Rect) {POSX,POSY,2*w,2*h};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
    return texture;
}

void close_window(SDL_Window *w,SDL_Renderer *r){
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}



