#include "SDL2/SDL.h"
#include "image_effect.h"
#include "win_img.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NAME_LEN 500


SDL_Surface * vertical_sym(SDL_Surface* src);
SDL_Surface * horizontal_sym(SDL_Surface* src);
SDL_Surface * color_effect(SDL_Surface *src, short effect);
SDL_Surface * rotate_img(SDL_Surface *src,int angle);// angle multiple de 90
SDL_Surface* rotate_img_a(SDL_Surface* src, float angle);// angle quelquonque
SDL_Surface * new_dim_img(SDL_Surface* src,int w,int h);//redimensioner avc w et h
SDL_Surface * cut_img(SDL_Surface *src,int pos_x,int by,int w,int h);// recadrage sur un sous rectangle
SDL_Surface * enlarge_img(SDL_Surface *src,int gate,int r,int g,int b);// agrandir avec une marge de pixel de taille gate




SDL_Surface* vertical_sym(SDL_Surface* src){
    
  SDL_Surface *dst=NULL;
  SDL_Rect l1,l2;
  int i;
 
  l1.y=0;
  l1.w=1;
 
  l2.y=0;
  l2.w=1;
 
  dst = SDL_CreateRGBSurface(0, src->w,src->h, 32,  0, 0, 0, 0);
 
  for (i=0;i<dst->w;i++){

      l1.x=i;
      l1.h=dst->h;
 
      l2.x=dst->w-i-1;
      l2.h=dst->h;
 
      SDL_BlitSurface(src,&l1,dst,&l2);
  }
 
  return dst;
}

void apply_vertical(int id){

  SDL_Surface *src,*dest;

  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour symertie v inexistante");
    return;
  }

  dest=vertical_sym(src);
  set_sf_by_id(id,dest);
  SDL_Window *wd=get_w_by_id(id);
  SDL_SetWindowSize(wd,dest->w,dest->h);
  reset_content(get_w_by_id(id),1);
}


SDL_Surface* horizontal_sym(SDL_Surface* src){
    
  SDL_Surface *dst=NULL;
  SDL_Rect l1,l2;
  int i;
 
  l1.x=0;
  l1.h=1;
 
  l2.x=0;
  l2.h=1;
 
  dst = SDL_CreateRGBSurface(0, src->w,src->h, 32,  0, 0, 0, 0);
 
  for (i=0;i<dst->h;i++){

      l1.y=i;
      l1.w=dst->w;
 
      l2.y=dst->h-i-1;
      l2.w=dst->w;
 
      SDL_BlitSurface(src,&l1,dst,&l2);
  }
 
  return dst;
}

void apply_horizontal(int id){

  SDL_Surface *src,*dest;

  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour symertie v inexistante");
    return;
  }

  dest=horizontal_sym(src);
  set_sf_by_id(id,dest);
  SDL_Window *wd=get_w_by_id(id);
  SDL_SetWindowSize(wd,dest->w,dest->h);
  reset_content(get_w_by_id(id),1);
}

SDL_Surface *color_effect(SDL_Surface *src, short effect){// effect 0:negatif 1: gris

  SDL_Surface * dst=NULL;
  int w,h;
  w=src->w;
  h=src->h;

  Uint32 *psrc,*pdst;
  psrc=NULL;
  pdst=NULL;

  if(SDL_LockSurface(src) < 0){
        fprintf(stderr, "Erreur SDL_LockSurface : %s", SDL_GetError());
        return dst;
    }

    pdst = malloc(src->pitch * h);
    psrc = (Uint32 *)src->pixels;

    Uint8 r,g,b,grey;

    for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w; j++)
      {
        SDL_GetRGB(psrc[i * w + j], src->format, &r, &g, &b);

        switch (effect){
          case 0:
            pdst[i * w + j] = SDL_MapRGB(src->format, 255 - r, 255 - g, 255 -b);
            break;
          case 1:
            grey=(r+g+b)/3;
            pdst[i * w + j] = SDL_MapRGB(src->format,grey,grey,grey);
            break;
          default:
            break;
        }
      }
    }

    dst=SDL_CreateRGBSurfaceWithFormatFrom(pdst, src->w, src->h, 32, src->pitch,src->format->format);
    
    if(dst==NULL){
        fprintf(stderr, "Erreur SDL_CreateRGBSurface : %s", SDL_GetError());
        free(pdst);
        SDL_UnlockSurface(src);
        return dst;
    }
    
    SDL_UnlockSurface(src);
    return dst;
}

/*Lire la valeur d'un pixel au position x,y*/
Uint32 SDL_GetPixel(SDL_Surface* surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
 
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
 
  switch(bpp)
  {
             case 1:
                  return *p;
             case 2:
                  return *(Uint16 *)p;
             case 3:
                 if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                     return p[0] << 16 | p[1] << 8 | p[2];
                 else
                     return p[0] | p[1] << 8 | p[2] << 16;
             case 4:
                  return *(Uint32 *)p;
             default:
                  return 0;
  } 
 
}
 
 
/*Ecrire un pixel au position x,y*/
void SDL_SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel; 
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
SDL_Surface * rotate_img(SDL_Surface *src,int angle){// angle multiple de 90
  
  if(angle%90 != 0){

    fprintf(stderr, "\nRotation impossible: angle n'est pas un multiple de 90 ");
    return NULL;
  }
  SDL_Surface * dst=NULL;
  int k=((int)angle/90)%4;

  if(k==2 || k==0)
    dst=SDL_CreateRGBSurface(SDL_SWSURFACE,src->w,src->h,32,0,0,0,0);
  else
    dst=SDL_CreateRGBSurface(SDL_SWSURFACE,src->h,src->w,32,0,0,0,0);

  if(SDL_LockSurface(src) < 0){
        fprintf(stderr, "Erreur SDL_LockSurface : %s", SDL_GetError());
        return dst;
    }
  for (int i = 0; i < src->w ; i++)
    {
      for (int j = 0; j < src->h ; j++)
      {
        Uint32 pix=SDL_GetPixel(src,i,j);
        switch(k){
          case 0:
            SDL_SetPixel(dst,i,j,pix);
            break;
          case 1:
            SDL_SetPixel(dst,j,src->w-1-i,pix);
            break;
          case 2:
            SDL_SetPixel(dst,src->w-1-i,src->h-1-j,pix);
            break;
          case 3:
            SDL_SetPixel(dst,src->h - 1-j,i,pix);
            break;
          default:
            break;

        }
      }
    }

  SDL_UnlockSurface(src);
  return dst;
}


SDL_Surface* rotate_img_a(SDL_Surface* src, float angle)
{
 SDL_Surface* dest;
 int i,j;
 float r,r_cos,r_sin;
 double dest_w,dest_h;
 int x_src,y_src,x_dest,y_dest;
 int pos_x,pos_y;
 Uint32 pixel;
 

 r = angle*M_PI / 180.0;
 r_cos= cos(r);
 r_sin= sin(r);

 dest_w= ceil(src->w * fabs(r_cos) + src->h * fabs(r_sin));
 dest_h= ceil(src->w * fabs(r_sin) + src->h * fabs(r_cos));

 dest= SDL_CreateRGBSurface(SDL_SWSURFACE,dest_w,dest_h,src->format->BitsPerPixel,
      src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask);

 x_dest = dest->w/2.;
 y_dest = dest->h/2.;
 x_src = src->w/2.;
 y_src = src->h/2.;
 
 for(j=0;j<dest->h;j++)
  for(i=0;i<dest->w;i++)
  {

   pos_x = (ceil (r_cos * (i-x_dest) + r_sin * (j-y_dest) + x_src));
   pos_y = (ceil (-r_sin * (i-x_dest) + r_cos * (j-y_dest) + y_src));
   
   if (pos_x>=0 && pos_x< src->w && pos_y>=0 && pos_y< src->h)
   {
     pixel = SDL_GetPixel(src,pos_x,pos_y);
     SDL_SetPixel(dest,i,j,pixel);
   }
 }

return dest;
}


void apply_rotate(int id,int angle){

  SDL_Surface *src,*dest;

  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour rotation v inexistante");
    return;
  }

  dest=rotate_img_a(src,angle);
  set_sf_by_id(id,dest);
  
  SDL_Window *wd=get_w_by_id(id);
  char *title=malloc(NAME_LEN);
  strcpy(title,SDL_GetWindowTitle(wd));
  printf("%s\n",title);
  SDL_DestroyWindow(wd);
  SDL_Window *wd2=creat_window(title,dest->h,dest->w);
  set_w_by_id(id,wd2);
  reset_content(wd2,1);

}
void edit_dimension(SDL_Surface* src,SDL_Surface* dst)
{
    double rx,ry;
    rx = dst->w*1.0/src->w;
    ry = dst->h*1.0/src->h;
    for(int i=0;i<dst->w;i++)
        for(int j=0;j<dst->h;j++)
            {
              Uint32 pix=SDL_GetPixel(src,(int)(i/rx),(int)(j/ry));
              SDL_SetPixel(dst,i,j,pix);
            }

}

SDL_Surface * new_dim_img(SDL_Surface* src,int w,int h){

    SDL_Surface* img = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0,0,0,0);
    edit_dimension(src,img);

    return img;
}

void apply_resize(int id,int w,int h){

  SDL_Surface *src=NULL,*dest=NULL;

  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour rotation v inexistante");
    return;
  }

  SDL_Window *wd=get_w_by_id(id);
  char *title=malloc(NAME_LEN);
  strcpy(title,SDL_GetWindowTitle(wd));
  printf("%s\n",title);
  SDL_DestroyWindow(wd);
  //free(wd);
  printf("%s\n",title);
  dest=new_dim_img(src,w,h);
  set_sf_by_id(id,dest);
  SDL_Window *wd2=NULL;
  wd2=creat_window(title,dest->h,dest->w);
  set_w_by_id(id,wd2);
  reset_content(wd2,1);

}


SDL_Surface *cut_img(SDL_Surface *src,int pos_x,int by,int w,int h){
   
  if(pos_x+w > src->w || by+h > src->h){
    
    fprintf(stderr, "\nRecadrage impossible: debordement sur l'image");
    return NULL;
  }
  
  SDL_Surface *dst=SDL_CreateRGBSurface(0,w,h, 32, 0,0,0,0);
  SDL_Rect r1,r2;
  r1= (SDL_Rect) {pos_x,by,w,h};
  r2= (SDL_Rect) {0,0,w,h};
  SDL_BlitSurface(src,&r1,dst,&r2);

  return dst;

}

void apply_cut(int id,int i1,int j1,int i2,int j2){

  SDL_Surface *src,*dest;

  int w=i2-i1;
  int h=j2-j1;

  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour rotation v inexistante");
    return;
  }

  dest=cut_img(src,i1,j1,w,h);
  set_sf_by_id(id,dest);
  SDL_Window *wd=get_w_by_id(id);
  SDL_SetWindowSize(wd,dest->w,dest->h);
  reset_content(wd,1);
}


SDL_Surface *enlarge_img(SDL_Surface *src,int gate,int r,int g,int b){

  if(gate <0){
    
    fprintf(stderr, "\nAgrandissement impossible");
    return NULL;
  }

  SDL_Surface *dst=SDL_CreateRGBSurface(0,src->w+gate,src->h+gate, 32, (Uint8)r, (Uint8)g,(Uint8)b,0);
  SDL_Rect r1,r2;
  r1= (SDL_Rect) {0,0,src->w,src->h};
  r2= (SDL_Rect) {(int)gate/2,(int)gate/2,src->w,src->h};
  SDL_BlitSurface(src,&r1,dst,&r2);

  return dst;

}

void apply_enlarge(int id,int gate,int r,int g,int b){

  SDL_Surface *src,*dest;
  src=get_sf_by_id(id);
  if(src==NULL){
    fprintf(stderr, "\nImage demandée pour rotation v inexistante");
    return;
  }

  dest=enlarge_img(src,gate,r,g,b);
  set_sf_by_id(id,dest);
  
  SDL_Window *wd=get_w_by_id(id);
  char *title=malloc(NAME_LEN);
  strcpy(title,SDL_GetWindowTitle(wd));
  printf("%s\n",title);
  SDL_DestroyWindow(wd);
  
  SDL_Window *wd2=creat_window(title,dest->h,dest->w);
  set_w_by_id(id,wd2);
  reset_content(wd2,1);
}

/*
//test
int main(int argc, char const *argv[])
{
  int d;
  open_new("../img/test1.bmp");
  scanf("%d",&d);
  apply_resize(1,800,800);
  scanf("%d",&d);
  apply_rotate(1,50);
  scanf("%d",&d);
  printf("finishing...\n");

  return 0;
}
*/