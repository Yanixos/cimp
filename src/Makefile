SOURCES = main.c parser.c script.c linker.c selection.c selection_effects.c image_effect.c win_img.c
OBJS = $(SOURCES:%.c=%.o)
INCLUDES = -l readline -lSDL2_image -lSDL2_image -lpng -lm
CFLAGS= -Wall $(shell sdl2-config --cflags --libs)

cimp : $(OBJS)
	gcc -o $@ $(OBJS) $(INCLUDES) $(CFLAGS)
		
%.o : %.c
	@echo "building $(@:%.o=%)..."
	gcc -c $< -o $@ $(CFLAGS)

main.o: main.c parser.h	
parser.o: parser.c parser.h linker.h
script.o: script.c script.h 
linker.o: linker.c linker.h script.h win_img.h image_effect.h selection.h selection_effects.h
selection.o: selection.c selection.h win_img.h
selection_effects.o: selection_effects.c selection_effects.h selection.h win_img.h
image_effects.o: image_effects.c image_effects.h win_img.h
win_img.o: win_img.c win_img.h 

clean:
	rm cimp $(OBJS)
