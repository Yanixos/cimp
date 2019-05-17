all: tests_runner

tests_runner: tests_runner.c selection_tests.o selection_effects_tests.o
	gcc -o tests_runner tests_runner.c selection_tests.o selection_effects.o ../src/selection_effects.o ../src/selection.o ../src/win_img.o `sdl2-config --cflags --libs` ../src/win_img.a -lSDL2_image -lpng -lcheck_pic -pthread -lrt -lm -lsubunit

selection_tests.o: selection_tests.c selection_tests.h
	gcc -o selection_tests.o -c selection_tests.c `sdl2-config --cflags --libs` -lSDL2_image -lpng -lcheck_pic -pthread -lrt -lm -lsubunit

selection_effects_tests.o: selection_effects_tests.c selection_effects_tests.h
	gcc -o selection_effects_tests.o -c selection_effects_tests.c `sdl2-config --cflags --libs` -lSDL2_image -lpng -lcheck_pic -pthread -lrt -lm -lsubunit

clean:
	rm -f tests_runner selection_tests.o selection_effects_tests.o
