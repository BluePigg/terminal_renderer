#include "modules/block.h"
#include "modules/square.h"
#include "modules/t_renderer.h"
#include "modules/text.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// left -> right -> up -> down
int direction = 1;

int body_length = 0;
int game_over = 0;

int wall_ids[4] = {0, 1, 2, 3};
Block *snake_bodies;

int main() {
  srand(time(NULL));

  T_Renderer *render = init_trender(66, 22);

  del_scr();
  hide_cursor();

  Square topwall = init_square(render, 0, 0, 0, 3, render->width, 1, '#');
  Square leftwall = init_square(render, 1, 0, 0, 3, 1, render->height, '#');
  Square rightwall =
      init_square(render, 2, render->width - 1, 0, 3, 1, render->height, '#');
  Square bottomwall =
      init_square(render, 3, 0, render->height - 1, 3, render->width, 1, '#');

  Block snake =
      init_block(render, 4, render->width / 4, render->height / 2, 2, 'S');

  Block apple = init_block(render, 5, render->width / 2 + render->width / 4,
                           render->height / 2, 1, '@');

  while (1) {
    int key = -1;
    while (kbhit()) {
      key = getch();
    }
    if (key != -1) {
      switch (key) {
      case 'a':
        direction = direction != 1 ? 0 : 1;
        break;
      case 'd':
        direction = direction != 0 ? 1 : 0;
        break;
      case 'w':
        direction = direction != 3 ? 2 : 3;
        break;
      case 's':
        direction = direction != 2 ? 3 : 2;
        break;
      }
    }

    for (int i = body_length - 1; i >= 0; i--) {
      Block reference = i == 0 ? snake : snake_bodies[i - 1];
      mv_block(render, &snake_bodies[i], reference.x, reference.y);
    }

    mv_block(render, &snake,
             snake.x + (direction < 2 ? (direction == 0 ? -1 : 1) : 0),
             snake.y + (direction > 1 ? (direction == 2 ? -1 : +1) : 0));

    int collide = check_collision(render, snake.id);

    clear_scr(render, '.');

    update_buffer(render);

    if (collide != -1 && collide != 5) {
      char *msg = cat_ch_to_int("GAME OVER, SCORE: ", body_length);
      message(render, msg, get_txt_length(msg));
      game_over = 1;
    } else if (collide == 5) {
      body_length++;
      snake_bodies =
          (Block *)realloc(snake_bodies, body_length * sizeof(Block));
      snake_bodies[body_length - 1] =
          init_block(render, new_id(render), -1, -1, 1, 'O');

      while (1) {
        mv_block(render, &apple, rand() % render->width,
                 rand() % render->height);
        if (check_collision(render, apple.id) == -1) {
          break;
        }
      }
    }

    render_buffer(render);

    if (game_over) {
      break;
    }

    usleep(1000 * 100);
  }
  free_trender(render);
}
