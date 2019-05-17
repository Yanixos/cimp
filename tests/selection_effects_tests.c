#include <stdio.h>
#include <check.h>
#include <SDL2/SDL.h>
#include "../src/selection_effects.h"
#include "../src/selection.h"
#include "../src/win_img.h"
#include "selection_effects_tests.h"

//getteur/setteur de pixels des surface
//source: http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 _get_pixel(SDL_Surface *surface, int x, int y)
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

START_TEST(fill_color_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  Uint8 r = 255, g = 155, b = 55;
  apply_fill(1, r, g, b);
  
  for(int x = minX; x<maxX;x++){
    for(int y = minY; y<maxY;y++){
      Uint8 r2, g2, b2;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r2, &g2, &b2);
      ck_assert_msg((r == r2)&&(g == g2)&&(b == b2), "pixel %d %d should be (%d, %d, %d)", x, y, r, g, b);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(replace_color_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int pixelX = 15, pixelY = 15;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  Uint8 dr = 255, dg = 155, db = 55;
  //lire la couleur d'un pixel quelconque
  Uint8 r, g ,b;
  SDL_GetRGB(_get_pixel(surf, pixelX, pixelY), surf->format, &r, &g, &b);
  //remplacer la couleur lue par une couleur destination (dr, dg, db)
  apply_replace_color(1, r, g, b, dr, dg, db, 50);
  //vérifier que le pixel lu en premier a change de couleur
  Uint8 r2, g2, b2;
  SDL_GetRGB(_get_pixel(surf, pixelX, pixelY), surf->format, &r2, &g2, &b2);
  ck_assert_msg((dr == r2)&&(dg == g2)&&(db == b2), "pixel %d %d should be (%d, %d, %d)", pixelX, pixelY, dr, dg, db);

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(greyscale_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  //remplir une partie de l'image en rouge, pour s'assurer
  //qu'il y a assez de pixels non gris
  Uint8 r = 255, g = 0, b = 0;
  apply_fill(1, r, g, b);

  //appliquer l'effet de niveaux de gris sur l'image entiere
  select_all(1);
  apply_grayscale(1);
  
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      Uint8 r2, g2, b2;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r2, &g2, &b2);
      //la couleur est en gris si toutes ses composantes sont egales
      ck_assert_msg((r2 == g2)&&(g2 == b2), "pixel %d %d should be gray", x, y);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(black_white_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  //remplir une partie de l'image en rouge, pour s'assurer
  //qu'il y a assez de pixels colories
  Uint8 r = 255, g = 0, b = 0;
  apply_fill(1, r, g, b);

  //appliquer l'effet de noir et blanc sur l'image entiere
  select_all(1);
  apply_blackwhite(1, 50);
  
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      Uint8 r2, g2, b2;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r2, &g2, &b2);
      short is_black = (r2 == 0) && (g2 == 0) && (b2 == 0);
      short is_white = (r2 == 255) && (g2 == 255) && (b2 == 255);
      //la couleur est en gris si toutes ses composantes sont egales
      ck_assert_msg(is_black || is_white, "pixel %d %d should be either black or white", x, y);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(negative_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();

  //remplir toute l'image en rouge
  select_all(1);
  Uint8 r = 255, g = 0, b = 0;
  apply_fill(1, r, g, b);
  
  int minX = 10;
  int maxX = 50;
  int minY = 10;
  int maxY = 50;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  //remplir une partie de l'image en vert
  r = 0; g = 255; b = 0;
  apply_fill(1, r, g, b);

  //appliquer l'effet de negative sur l'image entiere
  select_all(1);
  apply_negative(1);
  
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      Uint8 r2, g2, b2;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r2, &g2, &b2);
      //les pixels du rectangle vert doivent etre colorie
      //en couleur inverse (255,0,255)
      if((x>=minX)&&(x<maxX)&&(y>=minY)&&(y<maxY)){
	ck_assert_msg((r2 == 255)&&(g2 == 0)&&(b2 == 255), "pixel %d %d should be (0,255,255)", x, y);
      }
      //les pixels hors le rectangle, qui sont en rouge,
      //doivent etre colorie en couleur inverse (0,255,255)
      else{
	ck_assert_msg((r2 == 0)&&(g2 == 255)&&(b2 == 255), "pixel %d %d should be (255,0,255)", x, y);
      }
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(change_brightness_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int pixelX = 15, pixelY = 15;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  Uint8 dr = 255, dg = 155, db = 55;
  //remplir le rectangle avec une couleur quelconque
  apply_fill(1, dr, dg, db);
  
  //doubler la luminosité d'un pixel
  apply_brightness(1, 200);
  
  //vérifier que le pixel lu en premier a moin de luminosite
  Uint8 r2, g2, b2;
  SDL_GetRGB(_get_pixel(surf, pixelX, pixelY), surf->format, &r2, &g2, &b2);
  int bright1 = dr + dg + db;
  int bright2 = r2 + g2 + b2;

  ck_assert_msg(bright1 < bright2, "pixel %d %d should be brighter", pixelX, pixelY);

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(copy_paste_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int targetX = 100, targetY = 100;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  //copier le rectangle selectionne
  copy(1);
  //coller dans la position (100,100)
  paste(1, targetX, targetY);

  //verifier que le meme contenu est collee
  for(int x = minX;x < maxX-1;x++){
    for(int y = minY;y < maxY-1;y++){
      Uint8 r, g, b, r2, g2, b2;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r, &g, &b);
      SDL_GetRGB(_get_pixel(surf, targetX + x - minX, targetY + y - minY),
		 surf->format, &r2, &g2, &b2);
      ck_assert_msg((r == r2) && (g == g2) && (b == b2), "pixel %d %d should be copied from %d %d", x + targetX - minX, y + targetY - minY, x, y);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(cut_paste_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;
  int targetX = 100, targetY = 100;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);

  //couper le rectangle selectionne
  //la zone coupee est remplacee en noir
  cut(1, 0, 0, 0);
  //coller dans la position (100,100)
  paste(1, targetX, targetY);

  //verifier que la zone coupee est supprimee
  for(int x = minX;x < maxX-1;x++){
    for(int y = minY;y < maxY-1;y++){
      Uint8 r, g, b;
      SDL_GetRGB(_get_pixel(surf, x, y), surf->format, &r, &g, &b);
      ck_assert_msg((r == 0) && (g == 0) && (b == 0), "pixel %d %d should be filled with (0,0,0)", x + targetX - minX, y + targetY - minY);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

Suite* selection_effects_suite(){
  Suite* s = suite_create("selection effects test suite");
  
  TCase* fill = tcase_create("fill area with color");
  tcase_add_test(fill, fill_color_test);

  TCase* replace = tcase_create("replace color in area");
  tcase_add_test(replace, replace_color_test);

  TCase* rgb_eff = tcase_create("RGB effects");
  tcase_add_test(rgb_eff, greyscale_test);
  tcase_add_test(rgb_eff, black_white_test);
  tcase_add_test(rgb_eff, negative_test);
  tcase_add_test(rgb_eff, change_brightness_test);

  TCase* clipbrd = tcase_create("clipboard operations");
  tcase_add_test(clipbrd, copy_paste_test);
  tcase_add_test(clipbrd, cut_paste_test);
  
  suite_add_tcase(s, fill);
  suite_add_tcase(s, replace);
  suite_add_tcase(s, rgb_eff);
  suite_add_tcase(s, clipbrd);
  return s;
}

/*int main(){
  sdl_init();
  
  SRunner* sr = srunner_create(selection_effects_suite());
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
*/
