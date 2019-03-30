#ifndef SELECTION_H
#define SELECTION_H
enum mode {ADD, SUB, OVERWRITE};
//appeler apres la creation d'une nouvelle fenetre
extern void new_selection_node(SDL_Window* w);
//
extern void refresh_selection_list(SDL_Window* wl, int wl_size);
//selection de tous les pixels d'une fenetre
extern void select_all(SDL_Window* window);
//déselection de tout les pixels d'une fenetre
extern void deselect_all(SDL_Window* window);
//selectionner une region rectangulaire à partir des coordonnées des 2 points d'extrémité
extern void select_rect_coord(SDL_Window* window, enum mode mode, int startX, int startY, int endX, int endY);
//selectionner une region rectangulaire avec la souris
extern void select_rect(SDL_Window* window, enum mode mode);
//selection libre avec la souris
extern void select_free(SDL_Window* window, enum mode mode);
//selection des pixel d'une couleur donnée, avec marge de tolérance "maxdiff" en pourcentage [0-100]
//maxdiff=0: sans marge de tolérance
extern void select_color(SDL_Window* window, Uint32 _color, int maxdiff, enum mode mode);
//visualiser la partie selectionnée sur une fenetre
extern void draw_selected_pixels(SDL_Window* window, short render);
//retourne 1 si le pixel est selectionné dans une fenetre donnée
extern short is_selected(int pixel_x, int pixel_y, SDL_Window *window);
//calcule la distance entre 2 couleurs
extern int color_diff(Uint8 r1, Uint8 g1, Uint8 b1, Uint8 r2, Uint8 g2, Uint8 b2);
#endif
