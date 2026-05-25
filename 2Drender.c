#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show_cursor();
void hide_cursor();
void handle_sigint(int sig);

// Terminal Renderer
typedef struct T_Renderer {
  int width;
  int height;
  char *buffer;
  //{id번쨰 값: {x,y,z,x_size,y_size,chr},{ ... }}
  int **objects;
  int objects_size;
} T_Renderer;

T_Renderer *init_trender(int width, int height) {
  T_Renderer *render = (T_Renderer *)malloc(sizeof(T_Renderer));
  render->buffer = (char *)calloc(width * height, sizeof(char));
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

void update_buffer(T_Renderer *render) {
  int *zbuffer;
  zbuffer = (int *)calloc(render->width * render->height, sizeof(int));
  for (int i = 0; i < render->objects_size; i++) {
    int *obj = render->objects[i];
    if (obj == NULL) {
      continue;
    }
    for (int y = obj[1]; y < obj[1] + obj[4]; y++) {
      for (int x = obj[0]; x < obj[0] + obj[3]; x++) {
        if (x < 0 || x >= render->width || y < 0 || y >= render->height)
          continue;
        int idx = x + y * render->width;
        if (zbuffer[idx] <= obj[2]) {
          zbuffer[idx] = obj[2];
          draw_chr(render, x, y, obj[5]);
        }
      }
    }
  }
  free(zbuffer);
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
                int y_size, char chr) {
  while (id >= render->objects_size) {
    int **temp =
        realloc(render->objects, (render->objects_size + 1) * sizeof(int *));
    if (temp == NULL) {
      return id;
    } else {
      *(temp + render->objects_size) = (int *)calloc(6, sizeof(int));
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
  obj[5] = (int)chr;

  return id;
}
