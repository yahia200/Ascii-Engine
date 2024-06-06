#include <math.h>
#include <ncurses.h>

#define LIGHT 0
#define MEDIUM 2
#define DARK 2
#define MAX_MESH_SIZE 20
#define MAX_WIDTH 379
#define MAX_HIEGHT 176
char brightness[] = ".*@";
float fThetax, fThetaz, fThetay;

struct mat {
  float m[4][4];
};

float matProj[4][4], matRotZ[4][4], matRotX[4][4], matRotY[4][4];

struct Point {
  float x, y, z;
};

void MultiplyMatrixVector(struct Point *i, struct Point *o, float m[4][4]) {
  o->x = i->x * m[0][0] + i->y * m[1][0] + i->z * m[2][0] + m[3][0];
  o->y = i->x * m[0][1] + i->y * m[1][1] + i->z * m[2][1] + m[3][1];
  o->z = i->x * m[0][2] + i->y * m[1][2] + i->z * m[2][2] + m[3][2];
  float w = i->x * m[0][3] + i->y * m[1][3] + i->z * m[2][3] + m[3][3];

  if (w != 0.0f) {
    o->x /= w;
    o->y /= w;
    o->z /= w;
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
    mvwaddch(stdscr, (int)(point.y / 2), (int)(point.x), brightness[level]);
  }
}

void drawLine(struct Point po1, struct Point po2) {
  struct Point p1 = {(int)po1.x, (int)po1.y, (int)po1.z};
  struct Point p2 = {(int)po2.x, (int)po2.y, (int)po2.z};
  int dx = (int)(fabsf(p2.x - p1.x)), sx = p1.x < p2.x ? 1 : -1;
  int dy = (int)(-fabsf(p2.y - p1.y)), sy = p1.y < p2.y ? 1 : -1;
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

void drawTriangle(struct Triangle *triangle) {
  struct Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX,
      triRotatedZXY;
  MultiplyMatrixVector(&triangle->p[0], &triRotatedZ.p[0], matRotZ);
  MultiplyMatrixVector(&triangle->p[1], &triRotatedZ.p[1], matRotZ);
  MultiplyMatrixVector(&triangle->p[2], &triRotatedZ.p[2], matRotZ);

  MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], matRotX);
  MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], matRotX);
  MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], matRotX);

  MultiplyMatrixVector(&triRotatedZX.p[0], &triRotatedZXY.p[0], matRotY);
  MultiplyMatrixVector(&triRotatedZX.p[1], &triRotatedZXY.p[1], matRotY);
  MultiplyMatrixVector(&triRotatedZX.p[2], &triRotatedZXY.p[2], matRotY);

  triTranslated = triRotatedZXY;
  triTranslated.p[0].z = triRotatedZXY.p[0].z + 3.0f;
  triTranslated.p[1].z = triRotatedZXY.p[1].z + 3.0f;
  triTranslated.p[2].z = triRotatedZXY.p[2].z + 3.0f;
  MultiplyMatrixVector(&triTranslated.p[0], &triProjected.p[0], matProj);
  MultiplyMatrixVector(&triTranslated.p[1], &triProjected.p[1], matProj);
  MultiplyMatrixVector(&triTranslated.p[2], &triProjected.p[2], matProj);
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
  drawLine(triProjected.p[0], triProjected.p[1]);
  drawLine(triProjected.p[1], triProjected.p[2]);
  drawLine(triProjected.p[2], triProjected.p[0]);
}

void drawMesh(struct Mesh *mesh) {
  for (int i = 0; i < mesh->size; i++) {
    drawTriangle(&mesh->Triangles[i]);
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

void initMat() {
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)MAX_HIEGHT / (float)MAX_WIDTH;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  matProj[0][0] = fAspectRatio * fFovRad;
  matProj[1][1] = fFovRad;
  matProj[2][2] = fFar / (fFar - fNear);
  matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj[2][3] = 1.0f;
  matProj[3][3] = 0.0f;
}

void rotate() {
  matRotZ[0][0] = cosf(fThetaz);
  matRotZ[0][1] = sinf(fThetaz);
  matRotZ[1][0] = -sinf(fThetaz);
  matRotZ[1][1] = cosf(fThetaz);
  matRotZ[2][2] = 1;
  matRotZ[3][3] = 1;
  
  matRotX[0][0] = 1;
  matRotX[1][1] = cosf(fThetax * 0.5f);
  matRotX[1][2] = sinf(fThetax * 0.5f);
  matRotX[2][1] = -sinf(fThetax * 0.5f);
  matRotX[2][2] = cosf(fThetax * 0.5f);
  matRotX[3][3] = 1;
  
  matRotY[0][0] = cosf(fThetay);
  matRotY[0][2] = -sinf(fThetay);
  matRotY[2][0] = sinf(fThetay);
  matRotY[1][1] = 1.0f;
  matRotY[2][2] = cosf(fThetay);
  matRotY[3][3] = 1.0f;

  fThetax += 0.0;
  fThetaz += 0.0;
  fThetay += 0.1;
}
