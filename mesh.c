#include "triangles.c"
#include <stdlib.h>
void drawMesh(struct Mesh *mesh, int fill) {
  for (int i = 0; i < mesh->size; i++) {
    drawTriangle(&mesh->Triangles[i], fill);
  }
}
void initMesh(struct Mesh *m) {

  qsort(m->Triangles, m->size, sizeof(struct Triangle), comparePoints);
}
