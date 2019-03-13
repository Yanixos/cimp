#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL.h"
#include <stdlib.h>

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ABS(x) ((x)<0?-(x):(x))
//retourne un objet de rectangle formé par les 4 points
#define RECT(x1,y1,x2,y2) {MIN(x1,x2),MIN(y1,y2),ABS(x1-x2),ABS(y1-y2)}
//transforme les coordonnées d'un pixel en indice d'un tableau 1D
#define PIXEL_COORD(x,y,w) ((x)+(y)*(w))

//noeud de la liste de selection/fenetre
typedef struct selection_node{
  //sur quelle fenetre la selection est faite
  SDL_Window* window;
  //pixel selectionné = 1
  //pixel non selectionné = 0
  short* pixels;
  int w_width,w_height;
  //noeud prochain
  struct selection_node* next;
} selection_node;

//point 2D sur l'ecran
typedef struct point{
  int x, y;
} point;

//noeud pour la liste des points
typedef struct point_node{
  point p;
  struct point_node* next;
} point_node;

//noeud de triangle
typedef struct triangle{
  point p0, p1, p2;
} triangle;

//mode de selection
//ADD: ajout à la selection
//SUB: supprimer de la selection
//OVERRIDE: remplacer la selection
enum mode {ADD, SUB, OVERRIDE};

void draw_selected_pixels(selection_node* s, short render);

//SDL_Renderer* renderer;

//liste des selection/fenetre
selection_node* selection_list;
int selection_list_count;

//vérifie si 'p' est à l'intérieur du triangle 'tr'
//idée inspirée de:
//stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
short point_in_tri(point p, triangle tr) {
  //vérifier d'abord que la surface du triangle n'est pas nulle
  int crossproduct = (tr.p0.y - tr.p1.y) * (tr.p2.x - tr.p1.x) -
    (tr.p0.x - tr.p1.x) * (tr.p2.y - tr.p1.y);
  if(crossproduct == 0)return 0;
  
  int dX = p.x-tr.p2.x;
  int dY = p.y-tr.p2.y;
  int dX21 = tr.p2.x-tr.p1.x;
  int dY12 = tr.p1.y-tr.p2.y;
  int D = dY12*(tr.p0.x-tr.p2.x) + dX21*(tr.p0.y-tr.p2.y);
  int s = dY12*dX + dX21*dY;
  int t = (tr.p2.y-tr.p0.y)*dX + (tr.p0.x-tr.p2.x)*dY;
  if(D<0)
    return (s<=0) && (t<=0) && ((s+t)>=D);
  return (s>=0) && (t>=0) && ((s+t)<=D);
}

//vérifie si le segment [s1,s2] passe par le point p
//idée inspirée de:
//stackoverflow.com/questions/328107/how-can-you-determine-a-point-is-between-two-other-points-on-a-line-segment
short point_in_seg(point p, point s1, point s2){
  int crossproduct = (p.y - s1.y) * (s2.x - s1.x) - (p.x - s1.x) * (s2.y - s1.y);

  if(crossproduct!=0)
    return 0;

  int dotproduct = (p.x - s1.x) * (s2.x - s1.x) + (p.y - s1.y)*(s2.y - s1.y);
  if(dotproduct < 0)
    return 0;

  int sql = (s2.x - s1.x)*(s2.x - s1.x) + (s2.y - s1.y)*(s2.y - s1.y);
  if(dotproduct > sql)
    return 0;

  return 1;
}

//insere un noeud d'un point 2D dans une liste
void add_point_node(point_node** l, int x, int y){
  point_node* p = malloc(sizeof(point_node));
  p->p.x = x;
  p->p.y = y;
  p->next = NULL;
  if(*l == NULL){
    *l = p;
    return;
  }
  
  point_node* q = *l;
  while(q->next != NULL)
    q = q->next;
  q->next = p;
}

//retourne la taille d'une liste de points 2D
int point_list_size(point_node* l){
  int r = 0;
  while(l != NULL){
    r++;
    l = l->next;
  }
  return r;
}

//libère une liste de points
void free_point_list(point_node* l){
  point_node* p = l, * q;
  while(p != NULL){
    q = p->next;
    free(p);
    p = q;
  }
}

//dessine le chemin reliant les points d'une liste de points
void draw_path(point_node* l, SDL_Renderer *renderer){
  if(l == NULL){
    return;
  }
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  while(l->next != NULL){
    SDL_RenderDrawLine(renderer, l->p.x, l->p.y, l->next->p.x, l->next->p.y);
    l = l->next;
  }
}

//ajouter un noeud de selection lié a une fenetre
void new_window(SDL_Window* w){
  selection_node* s = malloc(sizeof(selection_node));
  if(s == NULL){
    perror("malloc ");
  }
  s->window = w;
  SDL_GetWindowSize(w,&(s->w_width),&(s->w_height));
  printf("new window %d %d\n",s->w_width,s->w_height);
  s->pixels = malloc(sizeof(short)*(s->w_width)*(s->w_height));
  if(s == NULL){
    perror("malloc ");
  }
  s->next = NULL;

  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      s->pixels[PIXEL_COORD(i,j,s->w_width)] = 1;
    }
  }

  if(selection_list == NULL){
    selection_list = s;
    return;
  }
  
  selection_node* p = selection_list;
  while(p->next != NULL){
    p = p->next;
  }
  p->next = s;
}

//retourne un selection_node de la fenetre donnée
selection_node* get_sel_node(SDL_Window *window){
  selection_node* p = selection_list;
  while(p != NULL){
    if(p->window == window){
      return p;
    }
    p = p->next;
  }
  return NULL;
}

void delete_sel_node(selection_node* s, selection_node** l){
  if(*l == NULL) return;
  if(*l == s){
    free(s->pixels);
    *l = s->next;
    free(s);
    return;
  }

  selection_node* p = *l;
  while(p != NULL){
    if(p->next == s){
      p->next = p->next->next;
      free(s->pixels);
      free(s);
    }
    p = p->next;
  }
}

//actualise la liste des selection/fenetre
//si une fenetre n'a pas de selection_node, il sera créé
//si la fenetre d'un selection_node n'existe plus, il sera libéré
void refresh_selection_list(SDL_Window* wl, int wl_size){
  //création des selection_node pour les nouvelle fenetres
  /*for(int i = 0;i < wl_size;i++){
    if(get_sel_node(&(wl[i])) == NULL){
      new_window(&(wl[i]));
    }
  }

  //suppression et mise à jour des nouvelles fenetres
  //ou des fenetres modifiées
  selection_node* s = selection_list;
  while(s != NULL){
    SDL_Window* win = s->window;
    for(int i = 0;i < wl_size;i++){
      if(win == &(wl[i])){
	int w, h;
	
      }
    }
  }*/
}

//raffraichir le contenu du renderer d'une fenetre sans dessiner la selection
//render = 1: actualiser l'affichage
void refresh_window(SDL_Window* window, short render){
  //recharger l'image de fond ici
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  SDL_SetRenderDrawColor(renderer, 50, 150, 150, 255);
  SDL_RenderClear(renderer);
  if(render)
    SDL_RenderPresent(renderer);
}

//deselectionner tout les pixels d'une selection
void deselect_all(selection_node* s){
  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      s->pixels[PIXEL_COORD(i,j,s->w_width)] = 0;
    }
  }
}

//selectionner un rectangle
void select_rect(selection_node* s, enum mode mode){
  int startX, startY, endX, endY;
  SDL_Window* window = s->window;
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  //start
  printf("start\n");
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e) &&
	e.window.windowID == SDL_GetWindowID(window)){
      switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
	startX = e.motion.x;
	startY = e.motion.y;
	printf("down at %d %d\n", e.motion.x, e.motion.y);
	goto selection;
	break;
      }
    }
  }
  printf("waiting for up...\n");
 selection:
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type) {
      case SDL_MOUSEMOTION:
	//refresh_window(window, 0);
	draw_selected_pixels(s, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
	SDL_Rect rect = RECT(startX, startY, e.motion.x, e.motion.y);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
	break;
      case SDL_MOUSEBUTTONUP:
	printf("up at %d %d\n", e.motion.x, e.motion.y);
	endX = e.motion.x;
	endY = e.motion.y;
	goto end;
	break;
      }
    }
  }
 end:
  if(mode == OVERRIDE)
    deselect_all(s);
  for(int i=MIN(startX,endX);i<MAX(startX,endX);i++){
    for(int j=MIN(startY,endY);j<MAX(startY,endY);j++){
      (s->pixels)[PIXEL_COORD(i,j,s->w_width)] = mode==SUB?0:1;
    }
  }
  printf("end\n");
}

//selectionner librement avec la souris
void select_free(selection_node* s, enum mode mode){
  SDL_Window* window = s->window;
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  point_node* p = NULL;
  //start
  printf("start\n");
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e) &&
	e.window.windowID == SDL_GetWindowID(window)){
      switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
	add_point_node(&p, e.motion.x, e.motion.y);
	printf("down at %d %d\n", e.motion.x, e.motion.y);
	goto selection;
	break;
      }
    }
  }
 selection:
  printf("waiting for up...\n");
  //draw_selected_pixels(s, 0);
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type) {
      case SDL_MOUSEMOTION:
	add_point_node(&p, e.motion.x, e.motion.y);
	draw_path(p, renderer);
	SDL_RenderPresent(renderer);
	break;
      case SDL_MOUSEBUTTONUP:
	printf("up at %d %d\n", e.motion.x, e.motion.y);
	goto end;
	break;
      }
    }
  }
  int n;
  end:
  //return;
  n = point_list_size(p) - 2;
  if(n <= 0)
    return;
  triangle tri[n];
  point_node* q = p;
  for(int i = 0;i < n;i++){
    tri[i].p0 = p->p;
    tri[i].p1 = q->next->p;
    tri[i].p2 = q->next->next->p;
    q = q->next;
  }
  if(mode == OVERRIDE)
    deselect_all(s);
  int minX=10000,maxX=0,minY=10000,maxY=0;
  q = p;
  while(q != NULL){
    minX = MIN(minX, (q->p.x));
    maxX = MAX(maxX, (q->p.x));
    minY = MIN(minY, (q->p.y));
    maxY = MAX(maxY, (q->p.y));
    q=q->next;
  }
  
  for(int i = minX;i < maxX;i++){
    for(int j = minY;j < maxY;j++){
      if((mode == ADD && (s->pixels)[PIXEL_COORD(i,j,s->w_width)])
	 || (mode == SUB && !(s->pixels)[PIXEL_COORD(i,j,s->w_width)]))
	continue;
      for(int k = 0;k < n;k++){
	point pp = {i, j};
	if(point_in_tri(pp, tri[k]) &&
	   !point_in_seg(pp, tri[k].p0, tri[k].p2)){
	  (s->pixels)[PIXEL_COORD(i,j,s->w_width)] = 
	    ((s->pixels)[PIXEL_COORD(i,j,s->w_width)]?0:1);
	}
      }
    }
  }
}

//visualiser sur une fenetre la région selectionnée
//la partie non selectionnée apparait sombre
void draw_selected_pixels(selection_node* s, short render){
  SDL_Window* window = s->window;
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  refresh_window(window,0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      if(!s->pixels[PIXEL_COORD(i,j,s->w_width)]){
	SDL_RenderDrawPoint(renderer,i,j);
	//printf("not selected %d %d\n", i, j);
      }
      else{
	//printf("selected %d %d\n", i, j);
      }
    }
  }
  if(render)
    SDL_RenderPresent(renderer);
}

//vérifie si le pixel (x,y) est selectionné dans la fenetre donnée
//si il n'y a pas de selection_node sur cette fenetre, le pixel est
//considéré comme sélectionné aussi
short is_selected(int pixel_x, int pixel_y, SDL_Window *window){
  selection_node* s = get_sel_node(window);
  return (s == NULL) || (s->pixels[PIXEL_COORD(pixel_x,pixel_y,s->w_width)] == 1);
}

int main(){
  if(0 != SDL_Init(SDL_INIT_VIDEO))
    return EXIT_FAILURE;
  printf("success\n");

  SDL_Window *window = NULL;
  window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    640, 480, SDL_WINDOW_SHOWN);
  if(NULL == window){
    fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
					      SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  new_window(window);
  refresh_window(window, 1);
  //select_rect(selection_list, OVERRIDE);
  //draw_selected_pixels(selection_list, 1);
  select_free(selection_list, OVERRIDE);
  draw_selected_pixels(selection_list, 1);
  select_free(selection_list, ADD);
  draw_selected_pixels(selection_list, 1);
  /*select_free(selection_list, SUB);
  draw_selected_pixels(selection_list, 1);
  select_rect(selection_list, ADD);
  draw_selected_pixels(selection_list, 1);
  select_rect(selection_list, SUB);
  draw_selected_pixels(selection_list, 1);*/
  //select_rect(renderer);
  printf("end\n");
  //SDL_Delay(3000);
  int a;
  scanf("%d",&a);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
