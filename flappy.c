#include "modules/block.h"
#include "modules/square.h"
#include "modules/t_renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int alive = 1;
int score = 0;

double bird_y = 0.;
double velocity = 0;
double gravity = 0.05;
double jump_force = -0.6;

int pipe_count = 4;
int pipe_space = 6;
int pipe_width = 5;
int initial_pipe_x;
int pipe_interval;
int space_from_edge;
Square **pipes;

int get_space_pos(T_Renderer *render) {
  int width = render->width;
  int space_pos =
      rand() % (render->height - space_from_edge) + space_from_edge / 2;
  return space_pos;
}

int main() {
  srand(time(NULL));
  T_Renderer *render = init_trender(66, 22);

  initial_pipe_x = render->width + render->width / 8;
  pipe_interval = render->width / 2;
  space_from_edge = render->height / 2;

  del_scr();
  hide_cursor();

  Block bird =
      init_block(render, 0, render->width / 5, render->height / 2, 2, '@');

  Block topwall = init_block(render, 1, bird.x, -1, 0, ' ');
  Block bottomwall = init_block(render, 2, bird.x, render->height, 0, ' ');

  bird_y = bird.y;
  clear_scr(render, ' ');
  update_buffer(render);
  render_buffer(render);
  while (!kbhit() || getch() != ' ') {
    usleep(1);
  }

  pipes = (Square **)malloc(sizeof(Square *) * pipe_count);
  for (int i = 0; i < pipe_count; i++) {
    int width = render->width;
    int space_pos = get_space_pos(render);
    pipes[i] = (Square *)malloc(sizeof(Square) * 2);
    pipes[i][0] =
        init_square(render, new_id(render), initial_pipe_x + i * pipe_interval,
                    0, 1, pipe_width, space_pos - pipe_space / 2, '#');
    pipes[i][1] = init_square(
        render, new_id(render), initial_pipe_x + i * pipe_interval,
        space_pos + pipe_space / 2, 1, pipe_width, render->width, '#');
  }

  while (alive) {
    int key = -1;
    while (kbhit()) {
      key = getch();
    }
    if (key != -1 && key == ' ') {
      velocity = jump_force - gravity;
    }

    clear_scr(render, ' ');

    for (int i = 0; i < pipe_count; i++) {
      if (pipes[i][0].x == bird.x) {
        score++;
      }
      mv_square(render, &pipes[i][0], pipes[i][0].x - 1, pipes[i][0].y);
      mv_square(render, &pipes[i][1], pipes[i][1].x - 1, pipes[i][1].y);

      if (pipes[i][0].x < -pipe_width) {
        int new_x = pipe_interval * (pipe_count);
        int new_space_pos = get_space_pos(render);
        mv_square(render, &pipes[i][0], new_x, 0);
        resize_square(render, &pipes[i][0], pipe_width,
                      new_space_pos - pipe_space / 2);
        mv_square(render, &pipes[i][1], new_x, new_space_pos + pipe_space / 2);
      }
    }

    velocity += gravity;
    bird_y += velocity;
    mv_block(render, &bird, bird.x, (int)(bird_y + 0.5));

    if (check_collision(render, bird.id) != -1) {
      alive = 0;
    }

    update_buffer(render);

    if (!alive) {
      int score_len = 1;
      int score_copy = score;
      while (score_copy > 10) {
        score_copy /= 10;
        score_len++;
      }
      char *msg = (char *)calloc(7 + score_len, sizeof(char));
      snprintf(msg, sizeof(char) * (7 + score_len), "SCORE:%d", score);
      message(render, msg, sizeof(char) * (7 + score_len));
    }

    render_buffer(render);

    usleep(1000 * 30);
  }

  free_trender(render);
}
