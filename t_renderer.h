#ifndef T_RENDERER_H
#define T_RENDERER_H

// Terminal Renderer Struct
typedef struct T_Renderer {
  int width;
  int height;
  char *buffer;
  int **objects;
  int objects_size;
} T_Renderer;

// Function Prototypes
T_Renderer *init_trender(int width, int height);
void free_trender(T_Renderer *render);
void hide_cursor();
void clear_scr(T_Renderer *render, char chr);
void del_scr();
double rot_x(int *obj, double dx, double dy);
double rot_y(int *obj, double dx, double dy);
void draw_chr(T_Renderer *render, int x, int y, char chr);
void update_buffer(T_Renderer *render);
void render_buffer(T_Renderer *render);
int draw_square(T_Renderer *render, int id, int x, int y, int z, int x_size,
                int y_size, int rotation, char chr);

#endif // T_RENDERER_H
