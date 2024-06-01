// #include "snake.c"
#include "engine.c"
#include <pthread.h>
#include <unistd.h>
int main() {
  struct Point p1 = {50, 70};
  struct Point p2 = {14, 5};
  struct Point p3 = {7, 10};
  struct Triangle t1 = {p1, p2, p3};
  struct Mesh mesh = {0};
  mesh.Triangles[0] = t1;
  mesh.size++;
  initScreen();
  while (1) {

    drawMesh(mesh);
    clearScreen();
    usleep(16*1000);
  }
  return 0;
  //  startGame();
}
