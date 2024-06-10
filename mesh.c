#include "triangles.c"
void drawMesh(struct Mesh *mesh, int fill) {
  sortTri(mesh->Triangles);
  place = 0;
  for (int i = 0; i < mesh->size; i++) {
    drawTriangle(&mesh->Triangles[i], fill);
  }
}

