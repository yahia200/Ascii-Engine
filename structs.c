#include "defines.c"
struct mat {
  float m[4][4];
};
struct Point {
  float x, y, z;
};
struct Triangle {
  struct Point p[3];
};

struct Mesh {
  int size;
  struct Triangle Triangles[MAX_MESH_SIZE];
};
