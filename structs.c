#include "defines.c"
struct mat {
  float m[4][4];
};
struct Point {
  float x, y, z, w;
};
struct Triangle {
  struct Point p[3];
  int intensity;
};

struct Mesh {
  int size;
  struct Triangle Triangles[MAX_MESH_SIZE];
};
