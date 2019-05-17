#include <stdio.h>
#include <check.h>
#include <SDL2/SDL.h>
#include "../src/selection.h"
#include "../src/win_img.h"
#include "selection_tests.h"

START_TEST(select_rect_test){
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
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      //les pixel dans le rectangle doivent etre selectionnes
      if((x>=minX)&&(x<maxX)&&(y>=minY)&&(y<maxY)){
	ck_assert_msg(is_selected(x, y, get_w_by_id(1)), "pixel %d %d should be selected", x, y);
      }
      //les pixels hors le rectangle ne doivent pas etre selectionnes
      else{
	ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
      }
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(select_add_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();

  //rectangle 1
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;

  //rectangle 2
  int minX2 = 15;
  int maxX2 = 25;
  int minY2 = 15;
  int maxY2 = 25;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);
  
  //ajout d'un autre rectangle a la selection
  select_rect_coord(1, ADD, minX2, minY2, maxX2, maxY2);
  
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      //les pixel dans le rectangle 1 doivent etre selectionnes
      if((x>=minX)&&(x<maxX)&&(y>=minY)&&(y<maxY)){
	ck_assert_msg(is_selected(x, y, get_w_by_id(1)), "pixel %d %d should be selected", x, y);
      }
      //les pixel dans le rectangle 2 doivent aussi etre selectionnes
      else if((x>=minX2)&&(x<maxX2)&&(y>=minY2)&&(y<maxY2)){
	ck_assert_msg(is_selected(x, y, get_w_by_id(1)), "pixel %d %d should be selected", x, y);
      }
      //les pixels hors les 2 rectangles ne doivent pas etre selectionnes
      else{
	ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
      }
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
  
} END_TEST

START_TEST(select_delete_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();

  //rectangle 1
  int minX = 10;
  int maxX = 20;
  int minY = 10;
  int maxY = 20;

  //rectangle 2
  int minX2 = 15;
  int maxX2 = 25;
  int minY2 = 15;
  int maxY2 = 25;
  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  //selection d'un rectangle
  select_rect_coord(1, OVERWRITE, minX, minY, maxX, maxY);
  
  //suppression d'un rectangle de la selection
  select_rect_coord(1, SUB, minX2, minY2, maxX2, maxY2);
  
  for(int x = 0; x<w;x++){
    for(int y = 0; y<h;y++){
      //les pixel dans le rectangle 1 doivent etre selectionnes
      if((x>=minX)&&(x<maxX)&&(y>=minY)&&(y<maxY)){
	//mais pas ceux du rectangle 2
	if((x>=minX2)&&(x<maxX2)&&(y>=minY2)&&(y<maxY2)){
	  ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
	}
	else{
	  ck_assert_msg(is_selected(x, y, get_w_by_id(1)), "pixel %d %d should be selected", x, y);
	}
      }
      //les pixels hors les 2 rectangles ne doivent pas etre selectionnes
      else{
	ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
      }
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
  
} END_TEST

START_TEST(deselect_all_test){
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
  deselect_all(1);
  
  for(int x = 0;x<w;x++){
    for(int y = 0;y<h;y++){
      //les pixel dans le rectangle ne doivent pas etre selectionnes
      if((x>=minX)&&(x<maxX)&&(y>=minY)&&(y<maxY)){
	ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
      }
      //de meme pour les pixels hors le rectangle
      else{
	ck_assert_msg(!is_selected(x, y, get_w_by_id(1)), "pixel %d %d should not be selected", x, y);
      }
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
  
} END_TEST

START_TEST(select_all_test){
  //ouverture d'une fenetre de test
  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();

  int w = get_sf_by_id(1)->w;
  int h = get_sf_by_id(1)->h;
  select_all(1);
  
  for(int x = 0;x<w;x++){
    for(int y = 0;y<h;y++){
      //tous les pixels doivent etre selectionnes
      ck_assert_msg(is_selected(x, y, get_w_by_id(1)), "pixel %d %d should be selected", x, y);
    }
  }

  //fermeture de la fenetre de test
  close_window(1);
  refresh_selection_list();
  
} END_TEST

Suite* selection_suite(){
  Suite* s = suite_create("selection test suite");
  
  TCase* rect = tcase_create("rectangular selection");
  tcase_add_test(rect, select_rect_test);
  
  TCase* modes = tcase_create("selection area operations");
  tcase_add_test(modes, select_add_test);
  tcase_add_test(modes, select_delete_test);
  
  TCase* globop = tcase_create("selection global operations");
  tcase_add_test(globop, select_all_test);
  tcase_add_test(globop, deselect_all_test);
  
  suite_add_tcase(s, rect);
  suite_add_tcase(s, modes);
  suite_add_tcase(s, globop);
  return s;
}

/*int main(){
  sdl_init();
  
  SRunner* sr = srunner_create(selection_suite());
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}*/
