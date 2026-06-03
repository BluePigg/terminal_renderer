#include "modules/block.h"
#include "modules/square.h"
#include "modules/t_renderer.h"
#include "modules/text.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
  srand(time(NULL));
  T_Renderer *render = init_trender(66, 22);

  del_scr();
  hide_cursor();
  Square moving_square =
      init_square(render, new_id(render), 10, 5, 1, 10, 10, '#');
  Block blk1 = init_block(render, new_id(render), 10, 10, 2, '@');
  Square static_square =
      init_square(render, new_id(render), 40, 5, 1, 5, 6, '%');
  Text txt = init_text(render, new_id(render), 10, 5, 1, 1, "Hello, World!");

  int a = 0;
  while (1) {
    int key = -1;
    while (kbhit()) {
      key = getch();
    }

    if (key != -1) {
      if (key == 'd') {
        mv_square(render, &moving_square, moving_square.x + 1, moving_square.y);
      } else if (key == 'a') {
        mv_square(render, &moving_square, moving_square.x - 1, moving_square.y);
      } else if (key == 'f') {
        int ran = rand();
        int ran_length = 1;
        int ran_cp = ran;
        while (ran_cp > 10) {
          ran_cp /= 10;
          ran_length++;
        }
        char *msg = (char *)calloc(ran_length + get_txt_length("Hello, World!"),
                                   sizeof(char));
        snprintf(msg,
                 (ran_length + get_txt_length("Hello, World!")) * sizeof(char),
                 "Hello, World!%d", ran);
        rewrite_txt(render, &txt, msg);
      } else if (key == 'w') {
        mv_square(render, &moving_square, moving_square.x, moving_square.y - 1);
      } else if (key == 's') {
        mv_square(render, &moving_square, moving_square.x, moving_square.y + 1);
      } else if (key == 'g') {
        switch_outline(render, &txt, txt.outline == 1 ? 0 : 1);
      } else if (key == 'h') {
        txt.z = txt.z != -1 ? -1 : 1;
        if (render->objects[txt.id] != NULL) {
          free(render->objects[txt.id]);
        }
        render->objects[txt.id] = parse_txt_iptr(txt);
      } else if (key == 'j') {
        txt.z = txt.z != -2 ? -2 : 1;
        if (render->objects[txt.id] != NULL) {
          free(render->objects[txt.id]);
        }
        render->objects[txt.id] = parse_txt_iptr(txt);
      } else if (key == 'k') {
        txt.z = txt.z != 0 ? 0 : 1;
        if (render->objects[txt.id] != NULL) {
          free(render->objects[txt.id]);
        }
        render->objects[txt.id] = parse_txt_iptr(txt);
      }
    }
    mv_block(render, &blk1, moving_square.x, moving_square.y - 3);
    clear_scr(render, '.');
    mv_square(render, &moving_square, moving_square.x + a, moving_square.y);

    int collision = check_collision(render, moving_square.id);
    if (collision != -1) {
      mv_square(render, &moving_square, moving_square.x, moving_square.y + 1);
    }
    update_buffer(render);

    char msg[20];
    snprintf(msg, sizeof(msg), "Collide detected: %d", collision);
    if (collision != -1) {
      message(render, msg, sizeof(msg));
    }

    render_buffer(render);

    usleep(1000 * 100);
  }
  free_trender(render);
}
