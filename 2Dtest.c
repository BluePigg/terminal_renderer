#include "t_renderer.h"
#include <unistd.h>

int main() {
  T_Renderer *render = init_trender(66, 22);

  del_scr();
  hide_cursor();
  draw_square(render, 1, 20, 10, 1, 3, 3, 0, '@');

  int a = 0;
  while (1) {
    clear_scr(render, '.');
    draw_square(render, 0, a, (int)(0.5 * a + 0.5), 0, 6, 6, a * 10, '#');
    a++;
    update_buffer(render);
    render_buffer(render);

    usleep(1000 * 100);
  }
  free_trender(render);
}
