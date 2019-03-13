#include "SDL.h"
#include "win_img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>






/*

int main(int argc,char *argv[]){

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture=NULL;
    
    sdl_init();
    open_new("../img/test1.bmp");
    //open_new("../img/test.bmp");

    node * p=list;
    while(p!=NULL){
      printf("\n%d",p->id);
      p=next(p);
    }
    
    printf("\nTaille de liste %d",length(list));
    if(get_by_id(2)==NULL)
      printf("No");
    else
      printf("Ys");
  
    
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
    
    close_window(window,renderer);


    return 0;
}
*/

/*--------------------------Modele de liste de fenetres---------------------------------------*/

/*Allocation avec initialisation d'un noeud */

node * allocNode(SDL_Window *w,SDL_Texture *img){

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

void addNode(node ** list, SDL_Window *w,SDL_Texture *img){ //ajoute un noeud d'une fenetre
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

/*
int deleteNode(node *list,SDL_Window *w){//supprime un noeud

  if(list == NULL)
    return -1;
  node *p=list;
  node *last=list;
  
  while(p!=NULL){
    if(p->w==w){
      affectNext(last,next(p));
      if(p==list)
        list
      free(p);

    }

  }


}
*/
int length(struct node * l){ //Retourne la taille de la liste des fenetres

  if(l==NULL)return 0;
  else return (1+length(next(l)));
}

SDL_Texture * get_by_id(int in){ // Retourne l'@ du noeud ayant id=in NULL si introuvable

  if(list==NULL)
    return NULL;
  node * p=list;
  while(p!=NULL){
    if(p->id==in)
      return p->img;
    else
      p=next(p);
  }

  return NULL;
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

int save_bmp_img(SDL_Surface *img, const char *path){

  return SDL_SaveBMP(img,path);
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
    SDL_Rect dest_rect = (SDL_Rect) {POSX,POSY,w,h};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
    return texture;
}

void close_window(SDL_Window *w,SDL_Renderer *r){
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}

void open_new(char *path){ // que des bmp

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Surface *surface = NULL;
  SDL_Texture *texture=NULL;
  surface=load_bmp_img(path);
  char *nameWin=malloc(100);
  if(list != NULL)
    sprintf(nameWin,"Window_ID: %d ",(list->id)+1);
  else
    sprintf(nameWin,"Window_ID:1");

  window=creat_window(nameWin,surface->h,surface->w);
  renderer=window_renderer(window);
  texture=display_img_text(renderer,surface);
  SDL_FreeSurface(surface);// n'est pas utile car changement possible
  addNode(&list,window,texture);

}


