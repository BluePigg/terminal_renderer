#include "t_renderer.h"
#include <stdint.h>
#include <stdlib.h>

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

char *cvt_obj_to_txt(int *obj) {
  if (obj[0] != 2)
    return NULL;
  uint64_t big_numb = (uint64_t)((uint32_t)obj[7] | (uint64_t)obj[8] << 32);
  char *txt_ptr = (char *)big_numb;

  return txt_ptr;
}

int *parse_txt_iptr(Text txt) {
  int *int_ptr = (int *)calloc(10, sizeof(int));
  int_ptr[0] = 2;
  int_ptr[1] = txt.x;
  int_ptr[2] = txt.y;
  int_ptr[3] = txt.z;
  int_ptr[4] = txt.x_size;
  int_ptr[5] = txt.y_size;
  int_ptr[6] = txt.outline;

  int_ptr[7] = (txt.txt_ptr & 0xFFFFFFFF);
  int_ptr[8] = ((txt.txt_ptr >> 32) & 0xFFFFFFFF);

  int_ptr[9] = txt.txt_length;
  return int_ptr;
}

int get_txt_length(char *txt) {
  int count = 0;
  while (txt[count] != '\0') {
    count++;
  }
  return count;
}

void mv_txt(T_Renderer *render, Text *txt, int nx, int ny) {
  txt->x = nx;
  txt->y = ny;
  if (render->objects[txt->id] != NULL) {
    free(render->objects[txt->id]);
  }
  render->objects[txt->id] = parse_txt_iptr(*txt);
}

void rewrite_txt(T_Renderer *render, Text *txt, char *txt_ptr, int txt_length) {
  txt->txt_ptr = (long)txt_ptr;
  txt->txt_length = txt_length;
  txt->x_size = txt_length + 2;
  txt->y_size = 3;
  if (render->objects[txt->id] != NULL) {
    free(render->objects[txt->id]);
  }
  render->objects[txt->id] = parse_txt_iptr(*txt);
}

void switch_outline(T_Renderer *render, Text *txt) {
  txt->outline = txt->outline == 0 ? 1 : 0;
  if (render->objects[txt->id] != NULL) {
    free(render->objects[txt->id]);
  }
  render->objects[txt->id] = parse_txt_iptr(*txt);
}

Text init_text(T_Renderer *render, int id, int x, int y, int z, int outline,
               char *txt_ptr, int txt_length) {
  Text txt;
  create_object_space(render, id, 10);

  txt.id = id;
  txt.x = x;
  txt.y = y;
  txt.z = z;
  txt.x_size = txt_length + 2;
  txt.y_size = 3;
  txt.outline = outline;
  txt.txt_ptr = (long)txt_ptr;
  txt.txt_length = txt_length;

  render->objects[id] = parse_txt_iptr(txt);

  return txt;
}
