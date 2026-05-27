#include "t_renderer.h"
#include <stdlib.h>

typedef struct Block {
  int id;
  int x;
  int y;
  int z;
  int chr;
} Block;

int *parse_blk_iptr(Block blk) {
  int *int_ptr = (int *)calloc(5, sizeof(int));
  int_ptr[0] = 0;
  int_ptr[1] = blk.x;
  int_ptr[2] = blk.y;
  int_ptr[3] = blk.z;
  int_ptr[4] = blk.chr;
  return int_ptr;
}
void mv_block(T_Renderer *render, Block *blk, int nx, int ny) {
  blk->x = nx;
  blk->y = ny;
  if (render->objects[blk->id] != NULL) {
    free(render->objects[blk->id]);
  }
  render->objects[blk->id] = parse_blk_iptr(*blk);
}

Block init_block(T_Renderer *render, int id, int x, int y, int z, char chr) {
  Block blk;

  create_object_space(render, id, 5);

  blk.id = id;
  blk.x = x;
  blk.y = y;
  blk.z = z;
  blk.chr = chr;

  render->objects[id] = parse_blk_iptr(blk);

  return blk;
}
