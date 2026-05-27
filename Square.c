#include "t_renderer.h"
#include <stdlib.h>

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

int *parse_sqr_iptr(Square sqr) {
  int *int_ptr = (int *)calloc(7, sizeof(int));
  int_ptr[0] = 1;
  int_ptr[1] = sqr.x;
  int_ptr[2] = sqr.y;
  int_ptr[3] = sqr.z;
  int_ptr[4] = sqr.x_size;
  int_ptr[5] = sqr.y_size;
  int_ptr[6] = sqr.chr;
  return int_ptr;
}

void mv_square(T_Renderer *render, Square *sqr, int nx, int ny) {
  sqr->x = nx;
  sqr->y = ny;
  if (render->objects[sqr->id] != NULL) {
    free(render->objects[sqr->id]);
  }
  render->objects[sqr->id] = parse_sqr_iptr(*sqr);
}

void resize_square(T_Renderer *render, Square *sqr, int nx, int ny) {
  sqr->x_size = nx;
  sqr->y_size = ny;
  if (render->objects[sqr->id] != NULL) {
    free(render->objects[sqr->id]);
  }
  render->objects[sqr->id] = parse_sqr_iptr(*sqr);
}

Square init_square(T_Renderer *render, int id, int x, int y, int z, int x_size,
                   int y_size, char chr) {
  Square sqr;
  create_object_space(render, id, 7);

  sqr.id = id;
  sqr.x = x;
  sqr.y = y;
  sqr.z = z;
  sqr.x_size = x_size;
  sqr.y_size = y_size;
  sqr.chr = (int)chr;

  render->objects[id] = parse_sqr_iptr(sqr);

  return sqr;
}
