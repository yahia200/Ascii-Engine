// #include "snake.c"
#include "engine.c"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int main() {
  struct Mesh mesh;
  struct Point p1 = {-1.0f, -1.0f, -1.0f};

  struct Point p2 = {-1.0f, 1.0f, -1.0f};
  
  struct Point p3 = {1.0f, 1.0f, -1.0f};
  
  struct Point p4 = {1.0f, -1.0f, -1.0f};
  
  struct Point p5 = {1.0f, 1.0f, 1.0f};
  
  struct Point p6 = {1.0f, -1.0f, 1.0f};
  
  struct Point p7 = {-1.0f, -1.0f, 1.0f};
  
  struct Point p8 = {-1.0f, 1.0f, 1.0f};
  
  struct Triangle t1 = {p1,p2,p3};
  
  struct Triangle t2 = {p1,p3,p4};

  struct Triangle t3 = {p4,p3,p5};

  struct Triangle t4 = {p4,p5,p6};

  struct Triangle t5 = {p6,p5,p8};

  struct Triangle t6 = {p6,p8,p7};

  struct Triangle t7 = {p7,p8,p2};

  struct Triangle t8 = {p7,p2,p1};

  struct Triangle t9 = {p2,p8,p5};

  struct Triangle t10 = {p2,p5,p3};

  struct Triangle t11 = {p6,p7,p1};

  struct Triangle t12 = {p6,p1,p4};
  
  mesh.Triangles[0] = t1;
  mesh.Triangles[1] = t2;
  mesh.Triangles[2] = t3;
  mesh.Triangles[3] = t4;
  mesh.Triangles[4] = t5;
  mesh.Triangles[5] = t6;
  mesh.Triangles[6] = t7;
  mesh.Triangles[7] = t8;
  mesh.Triangles[8] = t9;
  mesh.Triangles[9] = t10;
  mesh.Triangles[10] = t11;
  mesh.Triangles[11] = t12;
  mesh.size = 12;
 initScreen();
  rotate();
  initMat();
   while (1) {
     drawMesh(&mesh);
    clearScreen();
    rotate();
    usleep(16*1000);

   }


  endwin();

  return 0;
}
