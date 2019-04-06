#ifndef SELECTION_EFF_H
#define SELECTION_EFF_H
//appliquer l'effet négative sur une image
extern void apply_negative(SDL_Window* window, SDL_Surface* surface);
//appliquer l'effet niveaux de gris sur une image
extern void apply_grayscale(SDL_Window* window, SDL_Surface* surface);
//appliquer l'effet noir et blanc sur une image
//sep: le seuil de séparation entre le noir et le blanc
//sep entre 0 et 100
extern void apply_blackwhite(SDL_Window* window, SDL_Surface* surface, int sep);
//changer la luminosité d'une image
//brightness: facteur de luminosité
//brightness entre 0 et 200, par défaut égale à 100
extern void apply_brightness(SDL_Window* window, SDL_Surface* surface, int brightness);
//changer le contraste d'une image
//contrast: facteur de contraste
//contrast entre 0 et 200, par défault égale à 100
extern void apply_contrast(SDL_Window* window, SDL_Surface* surface, int contrast);
//appliquer l'effet de flou sur une image
//radius: le rayon de l'effet
extern void apply_blur(SDL_Window* window, SDL_Surface* surface, int radius);
//appliquer l'effet de pixelisation sur une image
//size: la taille du pixel de l'effet
extern void apply_pixel(SDL_Window* window, SDL_Surface* surface, int size);
//copier la zone selectionnée d'une image
extern void copy(SDL_Window* window, SDL_Surface* surface);
//couper la zone selectionnée d'une image
//background_color: la couleur qui remplacera les pixels coupés
extern void cut(SDL_Window* window, SDL_Surface* surface, Uint32 background_color);
//coller le presse-papier dans une image
//à partir des coordonnées (x, y)
extern void paste(SDL_Window* window, SDL_Surface* surface, int x, int y);
#endif
