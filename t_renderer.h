#include <stdlib.h>

#ifndef T_RENDERER_H
#define T_RENDERER_H

// Terminal Renderer Struct
typedef struct T_Renderer {
  int width;
  int height;
  char *buffer;
  int *zbuffer;
  //{id번쨰 값: {square (1) (type),x,y,z,x_size,y_size,rotation,chr},{ ... }}
  int **objects;
  int objects_size;
} T_Renderer;

// Function Prototypes
T_Renderer *init_trender(int width, int height);
void free_trender(T_Renderer *render);
void hide_cursor();
void clear_scr(T_Renderer *render, char chr);
void del_scr();
void draw_chr(T_Renderer *render, int x, int y, char chr);
void update_buffer(T_Renderer *render);
void render_buffer(T_Renderer *render);
int new_id(T_Renderer *render);
void create_object_space(T_Renderer *render, int id, int size);
int kbhit();
int getch();
void message(T_Renderer *render, char *msg, size_t msg_length);
int check_collision(T_Renderer *render, int id);
#endif // T_RENDERER_H
