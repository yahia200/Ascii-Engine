#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define LIGHT 0
#define MEDIUM 1
#define DARK 2
#define MAX_MESH_SIZE 20
#define MAX_WIDTH 237
#define MAX_HIEGHT 237
char brightness[] = ".*@";

struct Point {
  int x, y;
};

struct Triangle {
  struct Point p1;
  struct Point p2;
  struct Point p3;
};

struct Mesh {
  int size;
  struct Triangle Triangles[MAX_MESH_SIZE];
};

void drawPoint(struct Point point, int level) {
  if (point.x >= 0 && point.x < MAX_WIDTH && point.y >= 0 && point.y < MAX_HIEGHT) {
    move(point.y, point.x);
    addch('*');
    //mvwaddch(stdscr, point.y, point.x, brightness[level]);
  }
}

void drawLine(struct Point p1, struct Point p2) {
  int dx = abs(p2.x - p1.x), sx = p1.x < p2.x ? 1 : -1;
  int dy = -abs(p2.y - p1.y), sy = p1.y < p2.y ? 1 : -1;
  int err = dx + dy, e; /* error value e_xy */

  for (;;) {
    drawPoint(p1, 1);
    if (p1.x == p2.x && p1.y == p2.y)
      break;
    e = 2 * err;
    if (e >= dy) {
      err += dy;
      p1.x += sx;
    }
    if (e <= dx) {
      err += dx;
      p1.y += sy;
    }
  }
}

void drawTriangle(struct Triangle triangle) {
  drawLine(triangle.p1, triangle.p2);
  drawLine(triangle.p2, triangle.p3);
  drawLine(triangle.p3, triangle.p1);
}

void drawMesh(struct Mesh mesh) {
  for (int i = 0; i < mesh.size; i++) {
    drawTriangle(mesh.Triangles[i]);
  }
}

void clearScreen() {
  refresh();
  clear();
}

void initScreen(){
  initscr();
  noecho();
  curs_set(0);

}
