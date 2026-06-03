#include "t_renderer.h"

#ifndef TEXT_H
#define TEXT_H
typedef struct Text {
  int id;
  int x;
  int y;
  int z;
  int x_size;
  int y_size;
  int outline;
  long txt_ptr;
  int txt_length;
} Text;
char *cvt_obj_to_txt(int *obj);
int *parse_txt_iptr(Text txt);
void mv_txt(T_Renderer *render, Text *txt, int nx, int ny);
void resize_txt(T_Renderer *render, Text *txt, int nx, int ny);
Text init_text(T_Renderer *render, int id, int x, int y, int z, int outline,
               char *txt_ptr);
int get_txt_length(char *txt);
void rewrite_txt(T_Renderer *render, Text *txt, char *txt_ptr);
void switch_outline(T_Renderer *render, Text *txt, int type);

#endif
