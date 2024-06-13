#include "triangles.c"
#include <pthread.h>
#include <threads.h>
int f;
void *firstDraw() {
  struct Mesh m;
  m.size = 0;
  for (int i = 0; i < mesh.size; i++) {
    copyTriangle(&mesh.Triangles[i], &m.Triangles[i]);
    m.size++;
  }
  sortTri(m.Triangles, m.size);
  place = 0;
  drawTriangle(&m, f);
  return 0;
}

void *secondDraw() {
  struct Mesh m;
  m.size = 0;
  for (int i = 0; i < mesh.size / 2; i++) {
    copyTriangle(&mesh.Triangles[i + (mesh.size / 2)], &m.Triangles[i]);
    m.size++;
  }
  sortTri(m.Triangles, m.size);
  place = 0;
  drawTriangle(&m, f);
  return 0;
}

void drawMesh(int fill) {
  f = fill;
  pthread_t first, second;
  //sortTriy(mesh.Triangles, mesh.size);
  // pthread_create(&first, NULL, firstDraw, NULL);
  //  pthread_create(&second, NULL, secondDraw, NULL);
  // thrd_join(first, NULL);
  // thrd_join(second, NULL);
  sortTri(mesh.Triangles, mesh.size);
  drawTriangle(&mesh, fill);
}
