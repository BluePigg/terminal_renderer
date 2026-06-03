#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "t_renderer.h"
#include "text.h"

#ifndef RAD_C
#define RAD_C 0.0174533
#endif /* ifndef RAD_C                                                         \
#define RAD_C 0.0174533 */

void show_cursor();
void hide_cursor();
void handle_sigint(int sig);

char *cat_ch_to_int(char *msg, int target_int) {
  int msg_length = get_txt_length(msg);
  int tint_length = 1;
  int tint_cp = target_int;
  while (tint_cp >= 10) {
    tint_cp /= 10;
    tint_length++;
  }
  int nsize = msg_length + tint_length + 1;
  char *nmsg = (char *)calloc(nsize, sizeof(char));
  snprintf(nmsg, nsize * sizeof(char), "%s%d", msg, target_int);
  return nmsg;
}

// Terminal Renderer

struct termios orig_termios;

void reset_terminal_mode() { tcsetattr(0, TCSANOW, &orig_termios); }

void set_conio_terminal_mode() {
  struct termios new_termios;

  /* take two copies - one for now, one for later */
  tcgetattr(0, &orig_termios);
  memcpy(&new_termios, &orig_termios, sizeof(new_termios));

  /* register cleanup handler, and set the new terminal mode */
  atexit(reset_terminal_mode);
  cfmakeraw(&new_termios);
  new_termios.c_lflag |= ISIG;
  tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit() {
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
  int r;
  unsigned char c;
  if ((r = read(0, &c, sizeof(c))) < 0) {
    return r;
  } else {
    return c;
  }
}

void create_object_space(T_Renderer *render, int id, int size) {
  while (id >= render->objects_size) {
    int **temp =
        realloc(render->objects, (render->objects_size + 1) * sizeof(int *));
    if (temp == NULL) {
      printf("Error because realloc failed.");
    } else {
      *(temp + render->objects_size) = NULL;
      render->objects = temp;
      render->objects_size = render->objects_size + 1;
    }
  }
}

T_Renderer *init_trender(int width, int height) {
  T_Renderer *render = (T_Renderer *)malloc(sizeof(T_Renderer));
  render->buffer = (char *)calloc(width * height, sizeof(char));
  render->zbuffer = (int *)calloc(width * height, sizeof(int));
  render->width = width;
  render->height = height;
  render->objects = NULL;
  render->objects_size = 0;

  set_conio_terminal_mode();
  signal(SIGINT, handle_sigint);

  return render;
}

void free_trender(T_Renderer *render) {
  if (render != NULL) {
    if (render->objects != NULL) {
      for (int i = 0; i < render->objects_size; i++) {
        if (render->objects[i] != NULL) {
          free(render->objects[i]);
        }
      }
      free(render->objects);
    }
    free(render->buffer);
    free(render);
  }
}

void show_cursor() { printf("\033[?25h"); }
void hide_cursor() { printf("\033[?25l"); }

void handle_sigint(int sig) {
  show_cursor();

  exit(0);
}

void clear_scr(T_Renderer *render, char chr) {
  for (int i = 0; i < render->width * render->height; i++) {
    render->buffer[i] = chr;
  }
}

void del_scr() { printf("\033[2J"); }

void draw_chr(T_Renderer *render, int x, int y, char chr) {
  render->buffer[x + y * render->width] = chr;
}

int check_out_of_screen(T_Renderer *render, int x, int y) {
  return x < 0 || x >= render->width || y < 0 || y >= render->height;
}

// z 0 = visible but cannot collide
// z -1 = invisible but can collide
// z -2 = invisible and cannot collide
void update_buffer(T_Renderer *render) {
  memset(render->zbuffer, 0, render->width * render->height * sizeof(int));
  int *zbuffer = render->zbuffer;
  for (int i = 0; i < render->objects_size; i++) {
    int *obj = render->objects[i];
    if (obj == NULL || obj[3] < 0) {
      continue;
    }
    if (obj[0] == 0) {
      if (check_out_of_screen(render, obj[1], obj[2]))
        continue;
      int idx = obj[1] + obj[2] * render->width;
      if (zbuffer[idx] <= obj[3]) {
        zbuffer[idx] = obj[3];
        draw_chr(render, obj[1], obj[2], obj[4]);
      }
    } else if (obj[0] == 1) {
      for (int y = obj[2]; y < obj[2] + obj[5]; y++) {
        for (int x = obj[1]; x < obj[1] + obj[4]; x++) {
          if (check_out_of_screen(render, x, y))
            continue;
          int idx = x + y * render->width;
          if (zbuffer[idx] <= obj[3]) {
            zbuffer[idx] = obj[3];
            draw_chr(render, x, y, obj[6]);
          }
        }
      }
    } else if (obj[0] == 2) {
      int WIDTH = render->width;
      int HEIGHT = render->height;

      // int x_center = obj[1] - (obj[9] / 2);
      int x_center = obj[1];
      int y_center = obj[2];

      char *msg = cvt_obj_to_txt(obj);

      if (obj[6] != 0) {
        x_center = obj[1];
        for (int i = 0; i < obj[9] + 2; i++) {
          if (!check_out_of_screen(render, x_center + i - 1, y_center - 1) &&
              zbuffer[x_center + i - 1 + (y_center - 1) * WIDTH] <= obj[3]) {
            draw_chr(render, x_center + i - 1, y_center - 1,
                     obj[6] == 1 ? '-' : '#');
          }
          if (!check_out_of_screen(render, x_center + i - 1, y_center + 1) &&
              zbuffer[x_center + i - 1 + (y_center + 1) * WIDTH] <= obj[3]) {
            draw_chr(render, x_center + i - 1, y_center + 1,
                     obj[6] == 1 ? '-' : '#');
          }
        }
        if (!check_out_of_screen(render, x_center - 1, y_center) &&
            zbuffer[x_center - 1 + y_center * WIDTH] <= obj[3]) {
          draw_chr(render, x_center - 1, y_center, obj[6] == 1 ? '|' : '#');
        }
        if (!check_out_of_screen(render, x_center + obj[9], y_center) &&
            zbuffer[x_center + obj[9] + y_center * WIDTH] <= obj[3]) {
          draw_chr(render, x_center + obj[9], y_center,
                   obj[6] == 1 ? '|' : '#');
        }
      }
      for (int i = 0; i < obj[9]; i++) {
        if (!check_out_of_screen(render, x_center + i, y_center) &&
            zbuffer[x_center + i + y_center * WIDTH] <= obj[3]) {
          draw_chr(render, x_center + i, y_center, msg[i]);
        }
      }
    }
  }
}

int blk_to_blk(int *self, int *target) {
  return self[1] == target[1] && self[2] == target[2];
}

int blk_to_sqr(int *blk, int *sqr) {
  return sqr[1] <= blk[1] && blk[1] <= sqr[1] + sqr[4] - 1 &&
         sqr[2] <= blk[2] && blk[2] <= sqr[2] + sqr[5] - 1;
}

int sqr_to_sqr(int *self, int *target) {
  int smin_x = self[1];
  int smin_y = self[2];
  int smax_x = self[1] + self[4] - 1;
  int smax_y = self[2] + self[5] - 1;

  int tmin_x = target[1];
  int tmin_y = target[2];
  int tmax_x = target[1] + target[4] - 1;
  int tmax_y = target[2] + target[5] - 1;

  return smax_x >= tmin_x && smin_x <= tmax_x && smax_y >= tmin_y &&
         smin_y <= tmax_y;
}

// return target id
int check_collision(T_Renderer *render, int id) {
  int **objs_copy = (int **)malloc(render->objects_size * sizeof(int **));

  memcpy(objs_copy, render->objects, render->objects_size * sizeof(int *));
  for (int i = 0; i < render->objects_size; i++) {
    if (render->objects[i] == NULL) {
      continue;
    }
    int obj_size = 5;
    if (render->objects[i][0] == 1) {
      obj_size = 7;
    } else if (render->objects[i][0] == 2) {
      obj_size = 10;
    }
    int *obj_copy = (int *)malloc(obj_size * sizeof(int));
    memcpy(obj_copy, render->objects[i], obj_size * sizeof(int));
    objs_copy[i] = obj_copy;

    if (obj_copy[0] == 2 && obj_copy[6] == 1) {
      int x_center = obj_copy[1];

      obj_copy[1] = x_center - 1;
      obj_copy[2] = obj_copy[2] - 1;
    } else if (obj_copy[0] == 2 && obj_copy[6] == 0) {
      obj_copy[4] -= 2;
      obj_copy[5] -= 2;
    }
  }

  int *self = objs_copy[id];

  if (self == NULL) {
    for (int i = 0; i < render->objects_size; i++) {
      if (objs_copy[i] == NULL)
        continue;
      free(objs_copy[i]);
    }
    free(objs_copy);

    return -1;
  }

  int res = -1;
  for (int i = 0; i < render->objects_size; i++) {
    int *target = objs_copy[i];
    if (target == NULL || i == id || target[3] == -2 || self[3] == -2 ||
        target[3] == 0 || self[3] == 0)
      continue;
    if (self[0] == 0) {
      if (target[0] == 0) {
        if (blk_to_blk(self, target))
          res = i;
      } else if (target[0] == 1 || target[0] == 2) {
        if (blk_to_sqr(self, target))
          res = i;
      }
    } else if (self[0] == 1 || self[0] == 2) {
      if (target[0] == 0) {
        if (blk_to_sqr(target, self))
          res = i;
      } else if (target[0] == 1 || target[0] == 2) {
        if (sqr_to_sqr(self, target))
          res = i;
      }
    }
  }

  for (int i = 0; i < render->objects_size; i++) {
    if (objs_copy[i] == NULL)
      continue;
    free(objs_copy[i]);
  }
  free(objs_copy);
  return res;
}

void render_buffer(T_Renderer *render) {
  printf("\033[H");
  for (int y = 0; y < render->height; y++) {
    for (int x = 0; x < render->width; x++) {
      printf("%c", render->buffer[x + y * render->width]);
    }
    printf("\r\n");
  }
  fflush(stdout);
}

int new_id(T_Renderer *render) {
  for (int i = 0; i < render->objects_size; i++) {
    if (render->objects[i] == NULL) {
      return i;
    }
  }
  return render->objects_size;
}

void message(T_Renderer *render, char *msg, size_t msg_length) {
  int WIDTH = render->width;
  int HEIGHT = render->height;

  int x_center = WIDTH / 2 - (msg_length / 2);
  int y_center = HEIGHT / 2;

  for (int i = 0; i < msg_length + 2; i++) {
    draw_chr(render, x_center + i - 1, y_center - 1, '-');
    draw_chr(render, x_center + i - 1, y_center + 1, '-');
  }
  draw_chr(render, x_center - 1, y_center, '|');
  draw_chr(render, x_center + msg_length, y_center, '|');
  for (int i = 0; i < msg_length; i++) {
    draw_chr(render, x_center + i, y_center, msg[i]);
  }
}
