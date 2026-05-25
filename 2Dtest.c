#include "t_renderer.h"
#include <unistd.h>

int main() {
  T_Renderer *render = init_trender(66, 22);

  del_scr();
  hide_cursor();
  draw_square(render, 1, 20, 2, 1, 3, 3, '@');

  int a = 0;
  while (1) {
    clear_scr(render, '.');
    draw_square(render, 0, a, 2, 0, 5, 6, '#');
    a++;
    update_buffer(render);
    render_buffer(render);

    usleep(1000 * 100);
  }
  free_trender(render);
}
