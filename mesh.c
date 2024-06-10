#include "triangles.c"
void drawMesh(struct Mesh *mesh, int fill) {
  sortTri(mesh->Triangles);
  place = 0;
    drawTriangle(mesh, fill);
}
