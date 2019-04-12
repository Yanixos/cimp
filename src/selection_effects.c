#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL.h"
#include <stdlib.h>
#include "win_img.h"
#include "selection.h"
#include <string.h>

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
//#define PIXEL_COORD(x,y,w) ((x)+(y)*(w))

//presse-papier pour: copy/cut/paste
typedef struct cb_pixel{
  Uint32 color;
  short included;
} cb_pixel;
//pixels du presse-papier
cb_pixel* clipboard;
//largeur/hauter de la zone en presse-papier
int cb_width;
int cb_height;

//getteur/setteur de pixels des surface
//source: http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
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

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = pixel;
    break;

  case 2:
    *(Uint16 *)p = pixel;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;

  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

void apply_fill(int wid, int r, int g, int b){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;

      set_pixel(surface, i, j, SDL_MapRGB(surface->format, (Uint8)r, (Uint8)g, (Uint8)b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void apply_replace_color(int wid, int sr, int sg, int sb, int dr, int dg, int db, int maxdiff){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;

  maxdiff = maxdiff * (3 * 255) / 100;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i, j), surface->format, &r, &g, &b);
      if(color_diff((Uint8)sr, (Uint8)sg, (Uint8)sb, r, g, b) <= maxdiff)
	set_pixel(surface, i, j, SDL_MapRGB(surface->format, (Uint8)dr, (Uint8)dg, (Uint8)db));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//transforme une couleur en sa couleur négative
void to_negative(Uint8* r, Uint8* g, Uint8* b){
  *r = 255 - (*r);
  *g = 255 - (*g);
  *b = 255 - (*b);
}

//appliquer l'effet négative sur une image
void apply_negative(int wid){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i, j), surface->format, &r, &g, &b);

      to_negative(&r, &g, &b);

      set_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//transforme une couleur en de niveaux de gris
void to_grayscale(Uint8* r, Uint8* g, Uint8* b){
  *r = ((*r) * 21 + (*g) * 72 + (*b) * 7) / 100;
  *g = *r;
  *b = *r;
}

//appliquer l'effet niveaux de gris sur une image
void apply_grayscale(int wid){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i, j), surface->format, &r, &g, &b);

      to_grayscale(&r, &g, &b);

      set_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//transforme une couleur en noir et blanc
//sep étant le seuil d'intensité pour séparer le blanc du noir
//sep entre 0 et 100, par défaut sep=50
void to_bw(Uint8* r, Uint8* g, Uint8* b, int sep){
  //calcul de l'intensité du couleur
  Uint8 bright = ((*r) * 21 + (*g) * 72 + (*b) * 7) / 100;
  //intensité dépasse le seuil
  //transformer la couleur en blanc
  if(bright > (sep * 255 / 100)){
    *r = 255;
  }
  //intensité ne dépasse pas le seuil
  //transformer la couleur en noir
  else{
    *r = 0;
  }
  //les champs G et B auront la meme couleur que R
  //soit 0 soit 255
  *g = *r;
  *b = *r;
}

//appliquer l'effet noir et blanc sur une image
void apply_blackwhite(int wid, int sep){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i, j), surface->format, &r, &g, &b);

      to_bw(&r, &g, &b, sep);

      set_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//changer la luminosité d'une couleur
//brightness entre 0 et 200, par défaut égale à 100
void change_brightness(Uint8* r, Uint8* g, Uint8* b, int brightness){
  //multiplication des champs R G B par brightness
  Uint32 red = (*r) * brightness / 100;
  Uint32 green = (*g) * brightness / 100;
  Uint32 blue = (*b) * brightness / 100;
  //s'assurer que R G B sont toujours entre 0 et 255
  *r = MIN(red, 255);
  *g = MIN(green, 255);
  *b = MIN(blue, 255);
}

//changer la luminosité d'une image
void apply_brightness(int wid, int brightness){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i, j), surface->format, &r, &g, &b);

      change_brightness(&r, &g, &b, brightness);
      
      set_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//changer le contraste d'une image
//contrast entre 0 et 200, par défault égale à 100
void change_contrast(Uint8* r, Uint8* g, Uint8* b, int contrast){
  //dispersion des champs R G B de leurs centres 128
  //par le facteur "contrast"
  int red = 128 + ((*r) - 128) * contrast / 100;
  int green = 128 + ((*g) - 128) * contrast / 100;
  int blue = 128 + ((*b) - 128) * contrast / 100;
  //s'assurer que les champs R G B sont entre 0 et 255
  red = MIN(red, 255);
  green = MIN(green, 255);
  blue = MIN(blue, 255);
  red = MAX(red, 0);
  green = MAX(green, 0);
  blue = MAX(blue, 0);
  *r = (Uint8)red;
  *g = (Uint8)green;
  *b = (Uint8)blue;
}

//changer le contrast d'une image
void apply_contrast(int wid, int contrast){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;

  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(get_pixel(surface, i ,j), surface->format, &r, &g, &b);

      change_contrast(&r, &g, &b, contrast);

      set_pixel(surface, i, j, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//appliquer l'effet de flou sur une image
//radius étant le rayon de l'effet
void apply_blur(int wid, int radius){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;

  //création d'une copy temporaire de surface
  SDL_Surface *tmp = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);
  SDL_BlitSurface(surface, NULL, tmp, NULL);

  SDL_LockSurface(tmp);
  SDL_LockSurface(surface);
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x,y,window))continue;
      //calculer la somme des pixels selectionnés se trouvant entre
      //([x - radius, x + radius], [y - radius], [y + radius])
      //et les stocker dans ces variables
      Uint32 r = 0,g = 0,b = 0;
      //garder le nombre de pixels selectionnés dans l'intervalle
      //précédent
      int count = 0;

      for(int xx = x - radius;xx <= x + radius;xx++){
	if((xx < 0) || (xx >= surface->w))
	  continue;
	
	for(int yy = y - radius;yy <= y + radius;yy++){
	  if((yy < 0) || (yy >= surface->h))
	    continue;
	  
	  count++;
	  Uint8 r1,g1,b1;
	  SDL_GetRGB(get_pixel(tmp, xx, yy), tmp->format, &r1, &g1, &b1);
	  r += r1;
	  g += g1;
	  b += b1;
	}
      }

      //calculer la couleur moyenne en divisant la somme sur
      //le nombre total des pixels sommés
      r/=count;
      g/=count;
      b/=count;
      //affecter le résultat sur l'image source
      set_pixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  SDL_UnlockSurface(tmp);
  SDL_FreeSurface(tmp);
  reset_content(window, 1);
}

//appliquer l'effet de pixelisation sur une image
//size étant la taille du grand pixel de l'effet
void apply_pixel(int wid, int size){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  //création d'une copie temporaire de surface
  SDL_Surface *tmp = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);
  SDL_BlitSurface(surface, NULL, tmp, NULL);

  SDL_LockSurface(tmp);
  SDL_LockSurface(surface);
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x, y, window)) continue;
      //diviser la surface en régions de taille size*size
      //calculer la couleur moyenne de chaque région
      //affecter le résultat à chaque pixels de cette région

      //variables pour stocker la somme des couleurs
      Uint32 r = 0,g = 0,b = 0;
      //nombre de pixels selectionnés sommés
      int count = 0;
      //calcul des éxtremités de la région auquelle
      //appartient le pixel (x, y)
      int startX = x - (x % size);
      int startY = y - (y % size);
      //sommer les couleurs de la région
      for(int xx = startX;xx <= startX + size;xx++){
	if((xx < 0) || (xx >= surface->w))
	  continue;
	
	for(int yy = startY;yy <= startY + size;yy++){
	  if((yy < 0) || (yy >= surface->h))
	    continue;
	  
	  count++;
	  Uint8 r1,g1,b1;
	  SDL_GetRGB(get_pixel(tmp, xx, yy), tmp->format, &r1, &g1, &b1);
	  r += r1;
	  g += g1;
	  b += b1;
	}
      }

      //calcul de la couleur moyenne
      r/=count;
      g/=count;
      b/=count;

      //affectation du résultat
      set_pixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));
    }
  }
  SDL_UnlockSurface(surface);
  SDL_UnlockSurface(tmp);
  SDL_FreeSurface(tmp);
  reset_content(window, 1);
}

//copier la zone selectionnée dans une fenetre
void copy(int wid){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  //position de presse-papier
  int cb_x = surface->w;
  int cb_y = surface->h;
  cb_width = 0;
  cb_height = 0;
  //indicateur pour savoir s'il n'y a aucun pixel selectionné
  short empty = 1;
  //calcul des extrémités du réctangle qui entoure
  //la zone sélectionnée
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x, y, window)) continue;
      empty = 0;
      if(x < cb_x){
	cb_x = x;
      }
      else if(x > cb_width){
	cb_width = x;
      }

      if(y < cb_y){
	cb_y = y;
      }
      else if(y > cb_height){
	cb_height = y;
      }
    }
  }

  //suppression du contenu de presse-papier précédent
  if(clipboard != NULL){
    free(clipboard);
  }
  if(empty == 1) return;
  cb_width -= cb_x;
  cb_height -= cb_y;
  if(cb_width == 0 || cb_height == 0) return;
  clipboard = malloc(sizeof(cb_pixel) * cb_width * cb_height);
  //copier le contenu des pixels selectionnés dans le presse-papier
  for(int i = 0;i < cb_width;i++){
    for(int j = 0;j < cb_height;j++){
      if(is_selected(cb_x + i, cb_y + j, window)){
	(clipboard[i + j * cb_width]).included = 1;
	(clipboard[i + j * cb_width]).color = get_pixel(surface, cb_x + i, cb_y + j);
      }
      else{
	(clipboard[i + j * cb_width]).included = 0;
      }
    }
  }
}

//couper la zone selectionnée d'une image
//background: la couleur qui remplacera les pixels coupés
void cut(int wid, int r, int g, int b){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;

  Uint32 background = SDL_MapRGB(surface->format, (Uint32)r, (Uint32)g, (Uint32)b);
  
  //meme principe que celui de la fonction de copiage
  int cb_x = surface->w;
  int cb_y = surface->h;
  cb_width = 0;
  cb_height = 0;
  short empty = 1;
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x, y, window)) continue;
      empty = 0;
      if(x < cb_x){
	cb_x = x;
      }
      else if(x > cb_width){
	cb_width = x;
      }

      if(y < cb_y){
	cb_y = y;
      }
      else if(y > cb_height){
	cb_height = y;
      }
    }
  }

  if(clipboard != NULL){
    free(clipboard);
  }
  if(empty == 1) return;
  cb_width -= cb_x;
  cb_height -= cb_y;
  if(cb_width == 0 || cb_height == 0) return;
  clipboard = malloc(sizeof(cb_pixel) * cb_width * cb_height);
  SDL_LockSurface(surface);
  for(int i = 0;i < cb_width;i++){
    for(int j = 0;j < cb_height;j++){
      if(is_selected(cb_x + i, cb_y + j, window)){
	(clipboard[i + j * cb_width]).included = 1;
	(clipboard[i + j * cb_width]).color = get_pixel(surface, cb_x + i, cb_y + j);
	//apres avoir copié le contenu du pixel
	//le remplir avec la couleur de fond donnée
	set_pixel(surface, cb_x + i, cb_y + j, background);
      }
      else{
	(clipboard[i + j * cb_width]).included = 0;
      }
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

//coller le presse-papier dans une image
//à partir des coordonnées (x, y)
void paste(int wid, int x, int y){
  SDL_Window* window = get_w_by_id(wid);
  SDL_Surface* surface = get_sf_by_id(wid);
  if(window == NULL || surface == NULL) return;
  
  if(clipboard == NULL || x > surface->w || y > surface->h) return;
  //extremités de la zone qui sera remplie par le presse-papier
  int startX = MAX(0, x);
  int startY = MAX(0, y);
  int endX = MIN(x + cb_width, surface->w);
  int endY = MIN(y + cb_height, surface->h);

  SDL_LockSurface(surface);
  for(int xx = startX;xx < endX;xx++){
    for(int yy = startY;yy < endY;yy++){
      if((clipboard[xx - x + (yy - y) * cb_width]).included)
	set_pixel(surface, xx, yy, (clipboard[xx - x + (yy - y) * cb_width]).color);
    }
  }
  SDL_UnlockSurface(surface);

  reset_content(window, 1);
}

/*int main(){
  if(0 != SDL_Init(SDL_INIT_VIDEO))
    return EXIT_FAILURE;
  
  open_new("../img/test1.bmp");

  SDL_Window *window = get_w_by_id(1);
  
  new_selection_node(window);
  select_free(1, OVERWRITE);
  apply_replace_color(1, 255, 255, 255, 0, 255, 255, 30);
  draw_selected_pixels(1, 1);
  select_free(1, OVERWRITE);
  copy(1);
  paste(1, 200, 200);
  draw_selected_pixels(1, 1);
  printf("done..\n");
  int a;
  scanf("%d",&a);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
*/
