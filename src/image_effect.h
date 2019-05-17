#ifndef IMAGE_EFFECT_H
#define IMAGE_EFFECT_H

extern void apply_vertical(int id);
extern void apply_horizontal(int id);
extern void apply_rotate(int id,int angle);
extern void apply_resize(int id,int w,int h);
extern void apply_cut(int id,int i1,int j1,int i2,int j2);
extern void apply_enlarge(int id,int gate,int r,int g,int b);

#endif
