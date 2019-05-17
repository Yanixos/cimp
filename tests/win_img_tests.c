#include <stdio.h>
#include <check.h>
#include <SDL2/SDL.h>
#include "../src/win_img.h"
#include "win_img_tests.h"
#include "../src/selection.h"

START_TEST(open_new_window){

  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  ck_assert_msg(surf !=NULL,"open image in new window failed");
 
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(open_old_window){

  SDL_Surface * surf1 = open_new("../img/test1.bmp");
  refresh_selection_list();
  SDL_Surface * surf2 = open_old("../img/test1.bmp",1);
  refresh_selection_list();
  
  ck_assert_msg(surf2!=NULL,"open image in old window failed");
 
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(save_bmp_img){

  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int rep=save_bmp(1,"tmp_test.bmp");
  ck_assert_msg(!rep,"saving bmp image failed");
  system("rm -f tmp_test.bmp");
 
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(save_png_img){

  SDL_Surface * surf = open_new("../img/test1.bmp");
  refresh_selection_list();
  
  int rep=save_png(1,"tmp_test.png");
  ck_assert_msg(!rep,"saving png image failed");
  system("rm -f tmp_test.png");
 
  close_window(1);
  refresh_selection_list();
} END_TEST

START_TEST(back_ground_color){
  int r,g,b;
  set_bg_color(255,255,255);
  get_bg_color(&r,&g,&b);
  ck_assert_msg(!(r==255 &&(g==255 && b==255)),"setting bachground clolr failed");
} END_TEST


Suite* win_img_suite(){
  Suite* s = suite_create("win_img test suite");
  
  TCase* rect = tcase_create("open image");
  tcase_add_test(rect, open_new_window);
  tcase_add_test(rect, open_old_window);
  
  TCase* saves= tcase_create("saving image");
  tcase_add_test(saves,save_bmp_img);
  tcase_add_test(saves, save_png_img);
  
  TCase* bg = tcase_create("set bg_color");
  tcase_add_test(bg,back_ground_color);
  
  suite_add_tcase(s, rect);
  suite_add_tcase(s, saves);
  suite_add_tcase(s, bg);
  
  return s;
}

/*int main(){
  sdl_init();
  
  SRunner* sr = srunner_create(selection_suite());
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}*/
