#include <math.h>
#include <ncurses.h>

#define LIGHT 0
#define MEDIUM 2
#define DARK 2
#define MAX_MESH_SIZE 20
#define MAX_WIDTH 50
#define MAX_HIEGHT 20
char brightness[] = ".*@";

struct mat {
  float m[4][4];
};

struct mat matProj, matRotZ, matRotX;

struct Point {
  float x, y, z;
};

void MultiplyMatrixVector(struct Point i, struct Point o, struct mat m) {
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    o.x /= w;
    o.y /= w;
    o.z /= w;
  }
}

struct Triangle {
  struct Point p[3];
};

struct Mesh {
  int size;
  struct Triangle Triangles[MAX_MESH_SIZE];
};

void drawPoint(struct Point point, int level) {
  if (point.x >= 0 && point.x < MAX_WIDTH && point.y >= 0 &&
      point.y < MAX_HIEGHT) {
    mvwaddch(stdscr, point.y, point.x, brightness[level]);
  }
}

void drawLine(struct Point p1, struct Point p2) {
  int dx = fabsf(p2.x - p1.x), sx = p1.x < p2.x ? 1 : -1;
  int dy = -fabsf(p2.y - p1.y), sy = p1.y < p2.y ? 1 : -1;
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
  struct Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
  move(0, 0);
  printw("%f    %f |%f    %f|%f    %f", triangle.p[0].x, triangle.p[0].y,
         triangle.p[1].x, triangle.p[1].y, triangle.p[2].x, triangle.p[2].y);
  MultiplyMatrixVector(triangle.p[0], triRotatedZ.p[0], matRotZ);
  MultiplyMatrixVector(triangle.p[1], triRotatedZ.p[1], matRotZ);
  MultiplyMatrixVector(triangle.p[2], triRotatedZ.p[2], matRotZ);
  move(1, 0);
  printw("%f    %f |%f    %f|%f    %f", triRotatedZ.p[0].x, triRotatedZ.p[0].y,
         triRotatedZ.p[1].x, triRotatedZ.p[1].y, triRotatedZ.p[2].x,
         triRotatedZ.p[2].y);
  MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
  MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
  MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);
  move(2, 0);
  printw("%f    %f |%f    %f|%f    %f", triRotatedZX.p[0].x,
         triRotatedZX.p[0].y, triRotatedZX.p[1].x, triRotatedZX.p[1].y,
         triRotatedZX.p[2].x, triRotatedZX.p[2].y);
  triTranslated = triRotatedZX;
  triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
  triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
  triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;
  move(3, 0);
  printw("%f    %f |%f    %f|%f    %f", triTranslated.p[0].x,
         triTranslated.p[0].y, triTranslated.p[1].x, triTranslated.p[1].y,
         triTranslated.p[2].x, triTranslated.p[2].y);
  MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
  MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
  MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
  move(4, 0);
  printw("%f    %f |%f    %f|%f    %f", triProjected.p[0].x,
         triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
         triProjected.p[2].x, triProjected.p[2].y);
  triProjected.p[0].x += 1.0f;
  triProjected.p[0].y += 1.0f;
  triProjected.p[1].x += 1.0f;
  triProjected.p[1].y += 1.0f;
  triProjected.p[2].x += 1.0f;
  triProjected.p[2].y += 1.0f;
  triProjected.p[0].x *= 0.5f * (float)MAX_WIDTH;
  triProjected.p[0].y *= 0.5f * (float)MAX_HIEGHT;
  triProjected.p[1].x *= 0.5f * (float)MAX_WIDTH;
  triProjected.p[1].y *= 0.5f * (float)MAX_HIEGHT;
  triProjected.p[2].x *= 0.5f * (float)MAX_WIDTH;
  triProjected.p[2].y *= 0.5f * (float)MAX_HIEGHT;
  move(5, 0);
  printw("%f    %f |%f    %f|%f    %f", triProjected.p[0].x,
         triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
         triProjected.p[2].x, triProjected.p[2].y);
  getch();
  drawLine(triProjected.p[0], triProjected.p[1]);
  drawLine(triProjected.p[1], triProjected.p[2]);
  drawLine(triProjected.p[2], triProjected.p[0]);
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

void initScreen() {
  initscr();
  noecho();
  curs_set(0);
}

void fillMat(struct mat m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m.m[i][j] = 0;
    }
  }
}

void initMat() {
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)MAX_HIEGHT / (float)MAX_WIDTH;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  float fTheta = 0.0f;
  fillMat(matProj);
  matProj.m[0][0] = fAspectRatio * fFovRad;
  matProj.m[1][1] = fFovRad;
  matProj.m[2][2] = fFar / (fFar - fNear);
  matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;
  fillMat(matRotZ);
  matRotZ.m[0][0] = cosf(fTheta);
  matRotZ.m[0][1] = sinf(fTheta);
  matRotZ.m[1][0] = -sinf(fTheta);
  matRotZ.m[1][1] = cosf(fTheta);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;
  fillMat(matRotX);
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(fTheta * 0.5f);
  matRotX.m[1][2] = sinf(fTheta * 0.5f);
  matRotX.m[2][1] = -sinf(fTheta * 0.5f);
  matRotX.m[2][2] = cosf(fTheta * 0.5f);
  matRotX.m[3][3] = 1;
}


void printMatrix(struct mat m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printw("%f\t", m.m[i][j]);
    }
    printw("\n");
  }
}