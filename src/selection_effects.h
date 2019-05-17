#ifndef SELECTION_EFF_H
#define SELECTION_EFF_H
extern void apply_fill(int wid, int r, int g, int b);
extern void apply_replace_color(int wid, int sr, int sg, int sb, int dr, int dg, int db, int maxdiff);
//appliquer l'effet négative sur une image
extern void apply_negative(int wid);
//appliquer l'effet niveaux de gris sur une image
extern void apply_grayscale(int wid);
//appliquer l'effet noir et blanc sur une image
//sep: le seuil de séparation entre le noir et le blanc
//sep entre 0 et 100
extern void apply_blackwhite(int wid, int sep);
//changer la luminosité d'une image
//brightness: facteur de luminosité
//brightness entre 0 et 200, par défaut égale à 100
extern void apply_brightness(int wid, int brightness);
//changer le contraste d'une image
//contrast: facteur de contraste
//contrast entre 0 et 200, par défault égale à 100
extern void apply_contrast(int wid, int contrast);
//appliquer l'effet de flou sur une image
//radius: le rayon de l'effet
extern void apply_blur(int wid, int radius);
//appliquer l'effet de pixelisation sur une image
//size: la taille du pixel de l'effet
extern void apply_pixel(int wid, int size);
//copier la zone selectionnée d'une image
extern void copy(int wid);
//couper la zone selectionnée d'une image
//background_color: la couleur qui remplacera les pixels coupés
extern void cut(int wid, int r, int g, int b);
//coller le presse-papier dans une image
//à partir des coordonnées (x, y)
extern void paste(int wid, int x, int y);
#endif
