#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL.h"
#include <stdlib.h>
#include "win_img.h"
#include "selection.h"

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ABS(x) ((x)<0?-(x):(x))
#define SIGN(x) ((x)<0?-1:((x)>0?1:0))
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
//OVERWRITE: remplacer la selection
//enum mode {ADD, SUB, OVERWRITE};

//fonctions publiques
void new_selection_node(SDL_Window* w);
void refresh_selection_list();
void select_all(int wid);
void deselect_all(int wid);
void select_rect_coord(int wid, enum mode mode, int startX, int startY, int endX, int endY);
void select_rect(int wid, enum mode mode);
void select_free(int wid, enum mode mode);
void select_color(int wid, int r, int g, int b, int maxdiff, enum mode mode);
void draw_selected_pixels(int wid, short render);
short is_selected(int pixel_x, int pixel_y, SDL_Window *window);

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

//algorithme de traçage d'une ligne pixel par pixel
//source://
//modifiée pour permettre de tracer toutes les lignes
void draw_line(int x1, int y1, int x2, int y2, SDL_Surface* surface){
  short invX = 0;
  if(x1 > x2){
    if(y1 > y2){
      draw_line(x2,y2,x1,y1,surface);
      return;
    }
    else{
      invX = 1;
      int x = x1;x1 = x2;x2 = x;
    }
  }
  else if(y1 > y2){
    draw_line(x2,y2,x1,y1,surface);
    return;
  }
  SDL_LockSurface(surface);
  int dx=x2-x1;
  int dy=y2-y1;
  int StepVal=0;
  int x,y;
  if(dx>dy){
    y=y1;
    for(x=x1; x<=x2; x++){
      StepVal+=dy;
      if(StepVal>=dx){
	y++;
	StepVal-=dx;
      }
      int xx = invX?(x1+x2-x):x;
      ((Uint32*)(surface->pixels))[PIXEL_COORD(xx,y,surface->w)] = SDL_MapRGBA(surface->format,255,255,255,255);
    }
  }
  else{
    x=x1;
    for(y=y1; y<=y2; y++){
      StepVal+=dx;
      if(StepVal>=dy){
	x++;
	StepVal-=dy;
      }
      int xx = invX?(x1+x2-x):x;
      ((Uint32*)(surface->pixels))[PIXEL_COORD(xx,y,surface->w)] = SDL_MapRGBA(surface->format,255,255,255,255);
    }
  }
  SDL_UnlockSurface(surface);
}

//trace le chemin reliant les points d'une liste de points
void draw_path(point_node* l, SDL_Surface* surface){
  if(l == NULL){
    return;
  }
  while(l->next != NULL){
    draw_line(l->p.x, l->p.y, l->next->p.x, l->next->p.y, surface);
    l = l->next;
  }
}

//ajouter un noeud de selection lié a une fenetre
void new_selection_node(SDL_Window* w){
  selection_node* s = malloc(sizeof(selection_node));
  if(s == NULL){
    perror("malloc ");
  }
  s->window = w;
  SDL_GetWindowSize(w,&(s->w_width),&(s->w_height));
  //printf("new window %d %d\n",s->w_width,s->w_height);
  s->pixels = malloc(sizeof(short)*(s->w_width)*(s->w_height));
  if(s == NULL){
    perror("malloc ");
  }
  s->next = NULL;

  //au debut, considerer que tout les pixels sont selectionnés
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

//supprime un selection_node de la liste de selections
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
void refresh_selection_list(){
  SDL_Window* wl;
  int wl_size;
  //recuperer le tableau des fenetres existantes ici
  get_all_windows(&wl, &wl_size);

  //création des selection_node pour les nouvelle fenetres
  for(int i = 0;i < wl_size;i++){
    if(get_sel_node((&wl)[i]) == NULL){
      new_selection_node((&wl)[i]);
    }
  }

  //suppression et mise à jour des nouvelles fenetres
  //ou des fenetres modifiées
  selection_node* s = selection_list;
  while(s != NULL){
    selection_node* next = s->next;
    SDL_Window* win = s->window;
    for(int i = 0;i < wl_size;i++){
      if(win == ((&wl)[i])){
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	//taille de fenetre changée, recréer selection node
	if((w != s->w_width) || (h != s->w_height)){
	  new_selection_node(win);
	  goto delete;
	}
	else{
	  goto tonext;
	}
      }
    }
    //fenetre n'existe plus ou taille changée
  delete:
    delete_sel_node(s, &selection_list);
  tonext:
    s = next;
    continue;
  }
}

//raffraichir le contenu du renderer d'une fenetre sans dessiner la selection
//render = 1: actualiser l'affichage
//fonction a remplacer par une fonction dans le module win_img
/*void refresh_window(SDL_Window* window, short render){
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  SDL_Surface* image = SDL_LoadBMP("../img/test1.bmp");
  SDL_BlitSurface(image, NULL, surface, NULL);
  //SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 50, 150, 150, 255));
  if(render)
    SDL_UpdateWindowSurface(window);
}*/

//selectionner tout les pixels d'une fenetre
void select_all(int wid){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;
  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      s->pixels[PIXEL_COORD(i,j,s->w_width)] = 1;
    }
  }
}

//deselectionner tout les pixels d'une fenetre
void deselect_all(int wid){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      s->pixels[PIXEL_COORD(i,j,s->w_width)] = 0;
    }
  }
}

void select_rect_coord(int wid, enum mode mode, int startX, int startY, int endX, int endY){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  //printf("selection de tout les pixels entre (%d, %d) et (%d, %d)\n", startX, startY, endX, endY);
  if(mode == OVERWRITE)
    deselect_all(wid);
  for(int i=MIN(startX,endX);i<MAX(startX,endX);i++){
    for(int j=MIN(startY,endY);j<MAX(startY,endY);j++){
      (s->pixels)[PIXEL_COORD(i,j,s->w_width)] = mode==SUB?0:1;
    }
  }
}

//selectionner un rectangle
void select_rect(int wid, enum mode mode){
  //printf("%d %d\n",wid,mode);
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;
  //printf("telgash\n");

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  int startX, startY, endX, endY;
  //SDL_Window* window = s->window;
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  //SDL_Renderer* renderer = SDL_GetRenderer(window);
  //start
  //printf("start\n");
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e) &&
	e.window.windowID == SDL_GetWindowID(window)){
      switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
	startX = e.motion.x;
	startY = e.motion.y;
	//printf("down at %d %d\n", e.motion.x, e.motion.y);
	goto selection;
	break;
      }
    }
  }
  //printf("waiting for up...\n");
 selection:
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type) {
      case SDL_MOUSEMOTION:
	reset_content(window, 0);
	draw_selected_pixels(wid, 0);
	SDL_LockSurface(surface);
	int minX = MIN(startX, e.motion.x);
	int maxX = MAX(startX, e.motion.x);
	int minY = MIN(startY, e.motion.y);
	int maxY = MAX(startY, e.motion.y);
	for(int x = minX;x <= maxX;x++){
	  for(int y = minY;y <= maxY;y++){
	    Uint8 r1,g1,b1;
	    int blend = 100;//[0-255]
	    SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(x,y,surface->w)], surface->format, &r1, &g1, &b1);
	    Uint8 r = r1 + (255 - r1) * blend / 255;
	    Uint8 g = g1 + (255 - g1) * blend / 255;
	    Uint8 b = b1 + (255 - b1) * blend / 255;
	    ((Uint32*)(surface->pixels))[PIXEL_COORD(x,y,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
	  }
	}
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
	SDL_Rect rect = RECT(startX, startY, e.motion.x, e.motion.y);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);*/
	break;
      case SDL_MOUSEBUTTONUP:
	//printf("up at %d %d\n", e.motion.x, e.motion.y);
	endX = e.motion.x;
	endY = e.motion.y;
	goto end;
	break;
      }
    }
  }
 end:
  select_rect_coord(wid, mode, startX, startY, endX, endY);
  //printf("end\n");
}

//selectionner librement avec la souris
void select_free(int wid, enum mode mode){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  //SDL_Window* window = s->window;
  //SDL_Renderer* renderer = SDL_GetRenderer(window);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  point_node* p = NULL;
  //start
  //printf("start\n");
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e) &&
	e.window.windowID == SDL_GetWindowID(window)){
      switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
	add_point_node(&p, e.motion.x, e.motion.y);
	//printf("down at %d %d\n", e.motion.x, e.motion.y);
	goto selection;
	break;
      }
    }
  }
 selection:
  //printf("waiting for up...\n");
  //draw_selected_pixels(s, 0);
  while(1){
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type) {
      case SDL_MOUSEMOTION:
	add_point_node(&p, e.motion.x, e.motion.y);
	draw_path(p, surface);
	//SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
	break;
      case SDL_MOUSEBUTTONUP:
	//printf("up at %d %d\n", e.motion.x, e.motion.y);
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
  if(mode == OVERWRITE)
    deselect_all(wid);
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

int color_diff(Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2){
  int r = r1 - r2;
  int b = b1 - b2;
  int g = g1 - g2;
  //printf("dif %d %d %d\n",r,g,b);
  return ABS(r) + ABS(b) + ABS(g);
}

/*Uint32 getpixel(SDL_Surface *surface, int x, int y){
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
  return *(Uint32 *)p;
}*/

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    return *p;
    break;

  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;

  case 4:
    return *(Uint32 *)p;
    break;

  default:
    return 0;       /* shouldn't happen, but avoids warnings */
  }
}

void select_color(int wid, int red, int green, int blue, int maxdiff, enum mode mode){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;

  SDL_Surface* surface = get_sf_by_id(wid);
  if(surface == NULL) return;

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  //transformation pourcentage en difference de couleurs
  maxdiff = maxdiff * (3 * 255) / 100;
  SDL_PixelFormat *format = surface->format;

  Uint32 _color = SDL_MapRGB(format, (Uint8)red, (Uint8)green, (Uint8)blue);

  Uint8 r,g,b;
  SDL_GetRGB(_color, format, &r, &g, &b);
  //printf("selecting color %d %d %d\n", r, g, b);
  for(int i = 0;i < s->w_width;i++){
    for(int j = 0;j < s->w_height;j++){
      Uint8 r2,g2,b2;
      SDL_GetRGB(getpixel(surface, i, j), format, &r2, &g2, &b2);
      int diff = color_diff(r,g,b,r2,g2,b2);
      s->pixels[PIXEL_COORD(i,j, s->w_width)] = (diff<=maxdiff)?1:0;
    }
  }
}

//visualiser sur une fenetre la région selectionnée
//la partie non selectionnée apparait sombre
void draw_selected_pixels(int wid, short render){
  SDL_Window* window = get_w_by_id(wid);
  if(window == NULL) return;

  selection_node* s = get_sel_node(window);
  if(s == NULL)
    return;
  //SDL_Window* window = s->window;
  //SDL_Renderer* renderer = SDL_GetRenderer(window);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  reset_content(window,0);
  //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
  SDL_LockSurface(surface);
  for(int i = 0;i<s->w_width;i++){
    for(int j = 0;j<s->w_height;j++){
      if(!s->pixels[PIXEL_COORD(i,j,s->w_width)]){
	//SDL_RenderDrawPoint(renderer,i,j);
	//printf("not selected %d %d\n", i, j);
	Uint8 r1,g1,b1;
	SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r1, &g1, &b1);
	Uint8 r = r1*4/10;
	Uint8 g = g1*4/10;
	Uint8 b = b1*4/10;
	((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
      }
      else{
	//printf("selected %d %d\n", i, j);
      }
    }
  }
  SDL_UnlockSurface(surface);
  /*if(render)
    SDL_RenderPresent(renderer);*/
  if(render)
    SDL_UpdateWindowSurface(window);
}

//vérifie si le pixel (x,y) est selectionné dans la fenetre donnée
//si il n'y a pas de selection_node sur cette fenetre, le pixel est
//considéré comme sélectionné aussi
short is_selected(int pixel_x, int pixel_y, SDL_Window *window){
  selection_node* s = get_sel_node(window);
  return (s == NULL) || (s->pixels[PIXEL_COORD(pixel_x,pixel_y,s->w_width)] == 1);
}

/*int main(){
  if(0 != SDL_Init(SDL_INIT_VIDEO))
    return EXIT_FAILURE;

  open_new("../img/test1.bmp");

  SDL_Window *window = get_w_by_id(1);

  new_selection_node(window);
  reset_content(window, 1);
  select_free(selection_list->window, OVERWRITE);
  draw_selected_pixels(selection_list->window, 1);
  select_rect(selection_list->window, ADD);
  draw_selected_pixels(selection_list->window, 1);
  int a;
  scanf("%d",&a);
  SDL_Quit();

  return EXIT_SUCCESS;
}
*/
