#include "t_renderer.h"

#ifndef SQUARE_H
#define SQUARE_H
typedef struct Square {
  int id;
  int x;
  int y;
  int z;
  int x_size;
  int y_size;
  int rotation;
  int chr;
} Square;
int *parse_sqr_iptr(Square sqr);
Square init_square(T_Renderer *render, int id, int x, int y, int z, int x_size,
                   int y_size, char chr);
int *parse_sqr_iptr(Square sqr);
void mv_square(T_Renderer *render, Square *sqr, int nx, int ny);
void resize_square(T_Renderer *render, Square *sqr, int nx, int ny);
#endif // !square
