#include <stdio.h>
#include <check.h>
#include <SDL2/SDL.h>
#include "selection_tests.h"
#include "selection_effects_tests.h"
#include "win_img_tests.h"
#include "../src/win_img.h"

int main(){
  sdl_init();
  
  SRunner* sr = srunner_create(NULL);
  srunner_add_suite(sr, selection_suite());
  srunner_add_suite(sr, selection_effects_suite());
  srunner_add_suite(sr, win_img_suite());
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
