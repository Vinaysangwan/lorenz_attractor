#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// #############################################################################
//                           defines                                            
// #############################################################################
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

#define da_append(arr, val)                                                     \
  do {                                                                          \
    if ((arr).len >= (arr).capacity)                                            \
    {                                                                           \
      if ((arr).capacity == 0) (arr).capacity = 256;                            \
      else (arr).capacity *= 2;                                                 \
                                                                                \
      (arr).items = realloc((arr).items, sizeof(*(arr).items) * (arr).capacity);\
    }                                                                           \
                                                                                \
    (arr).items[(arr).len++] = (val);                                           \
  } while(0)

// #############################################################################
//                           structs                                            
// #############################################################################
typedef struct
{
  Vector3 *items;
  size_t len;
  size_t capacity;
} Points;

// #############################################################################
//                           functions                                            
// #############################################################################
int main(void)
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lorenz Attractor");
  SetTargetFPS(60);

  Camera3D camera = {
    .projection = CAMERA_ORTHOGRAPHIC,
    .fovy = 70.0f,
    .position = {0.0f, 100.0f, 20.0f},
    .up = {0.0f, 0.0f, 1.0f},
    .target = {0.0f, 0.0f, 20.0f}
  };

  Points points = {0};

  float x = 0.01f;
  float y = 0.0f;
  float z = 0.0f;

  float sig = 10.0f;
  float rho = 28.0f;
  float beta = 8.0f / 3.0f;

  while(!WindowShouldClose())
  {
    UpdateCamera(&camera, CAMERA_ORBITAL);
    
    float dt = 0.01f;
    
    float dx = sig * (y - x);
    float dy = x * (rho - z) - y;
    float dz = x * y - beta * z;    

    x += dx * dt;
    y += dy * dt;
    z += dz * dt;

    da_append(points, ((Vector3){x, y, z}));

    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(10, 10);

    BeginMode3D(camera);

    for (size_t i=1; i<points.len; ++i)
    {
      // DrawPoint3D(p, WHITE);
      double angle = PI / 180.0 * (double)i;
      Color color = {
        .r = (unsigned char)(sinf(angle) * 255),
        .g = (unsigned char)(cosf(angle) * 255),
        .b = 100,
        .a = 255
      };
      DrawLine3D(points.items[i - 1], points.items[i], color);
    }

    EndMode3D();

    EndDrawing();
  }

  CloseWindow();
  free(points.items);
  
  return 0;
}
