#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL.h"
#include <stdlib.h>
#include "win_img.h"
#include "selection.h"
#include <string.h>

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define PIXEL_COORD(x,y,w) ((x)+(y)*(w))

void to_negative(Uint8* r, Uint8* g, Uint8* b){
  *r = 255 - (*r);
  *g = 255 - (*g);
  *b = 255 - (*b);
}

void apply_negative(SDL_Window* window, SDL_Surface* surface){
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r, &g, &b);

      to_negative(&r, &g, &b);
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void to_grayscale(Uint8* r, Uint8* g, Uint8* b){
  *r = ((*r) * 21 + (*g) * 72 + (*b) * 7) / 100;
  *g = *r;
  *b = *r;
}

void apply_grayscale(SDL_Window* window, SDL_Surface* surface){
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r, &g, &b);

      to_grayscale(&r, &g, &b);
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void to_bw(Uint8* r, Uint8* g, Uint8* b, int sep){
  Uint8 bright = ((*r) * 21 + (*g) * 72 + (*b) * 7) / 100;
  if(bright > (sep * 255 / 100)){
    *r = 255;
  }
  else{
    *r = 0;
  }
  *g = *r;
  *b = *r;
}

void apply_blackwhite(SDL_Window* window, SDL_Surface* surface, int sep){
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r, &g, &b);

      to_bw(&r, &g, &b, sep);
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void change_brightness(Uint8* r, Uint8* g, Uint8* b, int brightness){
  Uint32 red = (*r) * brightness / 100;
  Uint32 green = (*g) * brightness / 100;
  Uint32 blue = (*b) * brightness / 100;
  *r = MIN(red, 255);
  *g = MIN(green, 255);
  *b = MIN(blue, 255);
}

void apply_brightness(SDL_Window* window, SDL_Surface* surface, int brightness){
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r, &g, &b);

      change_brightness(&r, &g, &b, brightness);
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void change_contrast(Uint8* r, Uint8* g, Uint8* b, int contrast){
  int red = 128 + ((*r) - 128) * contrast / 100;
  int green = 128 + ((*g) - 128) * contrast / 100;
  int blue = 128 + ((*b) - 128) * contrast / 100;
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

void apply_contrast(SDL_Window* window, SDL_Surface* surface, int contrast){
  SDL_LockSurface(surface);
  for(int i = 0;i < surface->w;i++){
    for(int j = 0;j < surface->h;j++){
      if(!is_selected(i, j, window)) continue;
      Uint8 r,g,b;
      SDL_GetRGB(((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)], surface->format, &r, &g, &b);

      change_contrast(&r, &g, &b, contrast);
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(i,j,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  reset_content(window, 1);
}

void apply_blur(SDL_Window* window, SDL_Surface* surface, int radius){
  size_t pixc = sizeof(Uint32) * surface->w * surface->h;
  Uint32* src = malloc(pixc);
  memcpy(src, surface->pixels, pixc);
  
  SDL_LockSurface(surface);
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x,y,window))continue;
      Uint32 r = 0,g = 0,b = 0;
      int count = 0;

      for(int xx = x - radius;xx <= x + radius;xx++){
	if((xx < 0) || (xx >= surface->w))
	  continue;
	
	for(int yy = y - radius;yy <= y + radius;yy++){
	  if((yy < 0) || (yy >= surface->h))
	    continue;
	  
	  count++;
	  Uint8 r1,g1,b1;
	  SDL_GetRGB(src[PIXEL_COORD(xx,yy,surface->w)], surface->format, &r1, &g1, &b1);
	  r += r1;
	  g += g1;
	  b += b1;
	}
      }

      r/=count;
      g/=count;
      b/=count;
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(x,y,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
}

void apply_pixel(SDL_Window* window, SDL_Surface* surface, int size){
  printf("bytes %d\n", surface->format->BytesPerPixel);
  size_t pixc = sizeof(Uint32) * (surface->w) * (surface->h);
  Uint32* src = malloc(pixc);
  memmove(src, surface->pixels, pixc);
  printf("pass\n");
  
  SDL_LockSurface(surface);
  for(int x = 0;x < surface->w;x++){
    for(int y = 0;y < surface->h;y++){
      if(!is_selected(x, y, window)) continue;
      Uint32 r = 0,g = 0,b = 0;
      int count = 0;
      int startX = x - (x % size);
      int startY = y - (y % size);
      for(int xx = startX;xx <= startX + size;xx++){
	if((xx < 0) || (xx >= surface->w))
	  continue;
	
	for(int yy = startY;yy <= startY + size;yy++){
	  if((yy < 0) || (yy >= surface->h))
	    continue;
	  
	  count++;
	  Uint8 r1,g1,b1;
	  SDL_GetRGB(src[PIXEL_COORD(xx,yy,surface->w)], surface->format, &r1, &g1, &b1);
	  r += r1;
	  g += g1;
	  b += b1;
	}
      }

      r/=count;
      g/=count;
      b/=count;
      
      ((Uint32*)(surface->pixels))[PIXEL_COORD(x,y,surface->w)] = SDL_MapRGB(surface->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface);
  free(src);
}

int main(){
  if(0 != SDL_Init(SDL_INIT_VIDEO))
    return EXIT_FAILURE;
  
  //open_new("../img/test1.bmp");
  /*SDL_Surface* image = SDL_LoadBMP("../img/test1.bmp");
  
  SDL_Window *window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 750, 475, SDL_WINDOW_SHOWN );
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  SDL_BlitSurface(image, NULL, surface, NULL);*/
  
  //SDL_UpdateWindowSurface(window);
  //new_selection_node(window);
  //select_free(window, OVERWRITE);
  //is_selected(0,0,window);
  //blur_filter(window, 15);
  //SDL_UpdateWindowSurface(window);

  int a;
  
  
  SDL_Surface* surface = open_new("../img/test1.bmp");
  SDL_Window *window = get_w_by_id(1);
  /*new_selection_node(window);

  select_free(window, OVERWRITE);
  draw_selected_pixels(window, 1);*/
  
  //scanf("%d",&a);
  apply_pixel(window, surface, 4);
  SDL_UpdateWindowSurface(window);
  draw_selected_pixels(window, 1);
  printf("done..\n");
  scanf("%d",&a);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
