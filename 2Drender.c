#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef RAD_C
#define RAD_C 0.0174533
#endif /* ifndef RAD_C                                                         \
#define RAD_C 0.0174533 */

void show_cursor();
void hide_cursor();
void handle_sigint(int sig);

// Terminal Renderer
typedef struct T_Renderer {
  int width;
  int height;
  char *buffer;
  int *zbuffer;
  //{id번쨰 값: {x,y,z,x_size,y_size,rotation,chr},{ ... }}
  int **objects;
  int objects_size;
} T_Renderer;

T_Renderer *init_trender(int width, int height) {
  T_Renderer *render = (T_Renderer *)malloc(sizeof(T_Renderer));
  render->buffer = (char *)calloc(width * height, sizeof(char));
  render->zbuffer = (int *)calloc(width * height, sizeof(int));
  render->width = width;
  render->height = height;
  render->objects = NULL;
  render->objects_size = 0;

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

double rot_x(int *obj, double dx, double dy) {
  return dx * cos(obj[5] * RAD_C) + dy * sin(obj[5] * RAD_C);
}

double rot_y(int *obj, double dx, double dy) {
  return -dx * sin(obj[5] * RAD_C) + dy * cos(obj[5] * RAD_C);
}

void update_buffer(T_Renderer *render) {
  memset(render->zbuffer, 0, render->width * render->height * sizeof(int));
  int *zbuffer = render->zbuffer;
  for (int i = 0; i < render->objects_size; i++) {
    int *obj = render->objects[i];
    if (obj == NULL) {
      continue;
    }
    double cx = obj[0] + obj[3] / 2.0;
    double cy = obj[1] + obj[4] / 2.0;
    for (double o_y = obj[1]; o_y < obj[1] + obj[4]; o_y += 0.5) {
      for (double o_x = obj[0]; o_x < obj[0] + obj[3]; o_x += 0.5) {
        double dx = o_x - cx;
        double dy = o_y - cy;
        int x = (int)(cx + rot_x(obj, dx, dy) + 0.5);
        int y = (int)(cy + rot_y(obj, dx, dy) + 0.5);
        if (x < 0 || x >= render->width || y < 0 || y >= render->height)
          continue;
        int idx = x + y * render->width;
        if (zbuffer[idx] <= obj[2]) {
          zbuffer[idx] = obj[2];
          draw_chr(render, x, y, obj[6]);
        }
      }
    }
  }
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

int draw_square(T_Renderer *render, int id, int x, int y, int z, int x_size,
                int y_size, int rotation, char chr) {
  while (id >= render->objects_size) {
    int **temp =
        realloc(render->objects, (render->objects_size + 1) * sizeof(int *));
    if (temp == NULL) {
      return id;
    } else {
      *(temp + render->objects_size) = (int *)calloc(7, sizeof(int));
      render->objects = temp;
      render->objects_size = render->objects_size + 1;
    }
  }
  int *obj = render->objects[id];
  obj[0] = x;
  obj[1] = y;
  obj[2] = z;
  obj[3] = x_size;
  obj[4] = y_size;
  obj[5] = rotation;
  obj[6] = (int)chr;

  return id;
}
