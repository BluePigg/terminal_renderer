#include "t_renderer.h"

#ifndef BLOCK_H
#define BLOCK_H
typedef struct Block {
  int id;
  int x;
  int y;
  int z;
  int chr;
} Block;

int *parse_blk_iptr(Block blk);
Block init_block(T_Renderer *render, int id, int x, int y, int z, char chr);
void mv_block(T_Renderer *render, Block *blk, int nx, int ny);
#endif // !square
